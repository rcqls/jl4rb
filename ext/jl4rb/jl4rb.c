/**********************************************************************

  jl4rb.c

**********************************************************************/

#include "julia.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

//#define WITH_JULIA_RELEASE

//#ifdef WITH_JULIA_RELEASE
//#else
//#include "julia-api.h"
//#endif

//-| next macros already exist in ruby and are undefined here
//#undef T_FLOAT
#undef NORETURN
#include "ruby.h"
#include "ruby/version.h"

#define length(a) jl_array_size(a,0)

/************* INIT *********************/


VALUE Julia_init()
{
  jl_init();
  return Qtrue;
}

VALUE Julia_exit(VALUE exitcode) {
  jl_atexit_hook(exitcode);
  return Qtrue;
}

//Maybe try to use cpp stuff to get the output inside julia system (ccall,cgen and cgutils)
//-| TODO: after adding in the jlapi.c jl_is_<C_type> functions replace the strcmp!
VALUE jl_value_to_VALUE(jl_value_t *res) {
  size_t i=0,k,nd,d;
  VALUE resRb;
  jl_value_t *tmp;
  jl_function_t *call;

  if(res!=NULL) { //=> get a result
    //printf("typeof=%s\n",jl_typeof_str(res));
    if(strcmp(jl_typeof_str(res),"Int64")==0 || strcmp(jl_typeof_str(res),"Int32")==0)
    //if(jl_is_long(res)) //does not work because of DLLEXPORT
    {
      //printf("elt=%d\n",jl_unbox_long(res));
      return INT2FIX(jl_unbox_long(res));
    }
    else
    if(strcmp(jl_typeof_str(res),"Float64")==0)
    //if(jl_is_float64(res))
    {
      return rb_float_new(jl_unbox_float64(res));
    }
    else
    if(strcmp(jl_typeof_str(res),"Float32")==0)
    //if(jl_is_float64(res))
    {
      return rb_float_new(jl_unbox_float32(res));
    }
    else
    if(strcmp(jl_typeof_str(res),"Bool")==0)
    //if(jl_is_bool(res))
    {
      return (jl_unbox_bool(res) ? Qtrue : Qfalse);
    }
    else
    if(strcmp(jl_typeof_str(res),"DataType")==0)
    {
      return rb_str_new2(jl_typename_str(res));
    }
    else
    if(strcmp(jl_typeof_str(res),"Nothing")==0)
    {
      return Qnil;
    }
    else
    if(strcmp(jl_typeof_str(res),"Complex")==0)
    //if(jl_is_bool(res))
    {
      resRb = rb_eval_string("require('complex');Complex.new(0,0)");
      rb_iv_set(resRb,"@real",jl_value_to_VALUE(jl_get_field(res, "re")));
      rb_iv_set(resRb,"@image",jl_value_to_VALUE(jl_get_field(res, "im")));
      return resRb;
    }
    else
    if(strcmp(jl_typeof_str(res),"Regex")==0)
    //if(jl_is_bool(res))
    {
      // call=(jl_function_t*)jl_get_global(jl_base_module, jl_symbol("show"));
      // printf("ici\n");
      // if (call) tmp=jl_call1(call,res);
      // else printf("call failed!\n");
      // printf("ici\n");
      resRb = jl_value_to_VALUE(jl_get_field(res, "pattern"));
      return resRb;
    }
    else
    if(strcmp(jl_typeof_str(res),"String")==0 || strcmp(jl_typeof_str(res),"ASCIIString")==0 || strcmp(jl_typeof_str(res),"UTF8String")==0)
    {
      //printf("value=%s\n",jl_string_ptr(res));
      return rb_str_new2(jl_string_ptr(res));
    }
    else
    if(strcmp(jl_typeof_str(res),"Array")==0 )
    //if(jl_is_array(res))
    {
      nd = jl_array_rank(res);
      //printf("array_ndims=%d\n",(int)nd);
      if(nd==1) {//Vector
        d = jl_array_size(res, 0);
        //printf("array_dim[1]=%d\n",(int)d);
        resRb = rb_ary_new2(d);
        for(i=0;i<d;i++) {
          rb_ary_store(resRb,i,jl_value_to_VALUE(jl_arrayref((jl_array_t *)res,i)));
        }
        return resRb;
      }
      //TODO: multidim array ruby equivalent???? Is it necessary

    }
    else
    if(strcmp(jl_typeof_str(res),"Tuple")==0 )
    //if(jl_is_array(res))
    {
      d=jl_nfields(res); //Before O.3: d=jl_tuple_len(res);
      resRb = rb_ary_new2(d);
      for(i=0;i<d;i++) {
        //rb_ary_store(resRb,i,jl_value_to_VALUE(jl_tupleref(res,i)));
        rb_ary_store(resRb,i,jl_value_to_VALUE(jl_fieldref(res,i)));
      }
      return resRb;
    }
    //printf("unconverted!!!");
    resRb=rb_str_new2("__unconverted(");
    rb_str_cat2(resRb, jl_typeof_str(res));
    rb_str_cat2(resRb, ")__\n");
    //printf("%s\n",jl_string_ptr(jl_eval_string("\"$(ans)\"")));
    // jl_function_t *call=(jl_function_t*)jl_get_global(jl_base_module, jl_symbol("show"));
    // if (call) jl_call1(call,res);
    // else printf("call failed!\n");

    return resRb;
  }
  //=> No result (command incomplete or syntax error)
//#ifndef WITH_JULIA_RELEASE
//  jlapi_print_stderr(); //If this happens but this is really not sure!
//#endif
  //printf("incomplete!!!");
  resRb=rb_str_new2("__incomplete");
  if(jl_exception_occurred()!=NULL) {
    rb_str_cat2(resRb, "(");
      rb_str_cat2(resRb,jl_typeof_str(jl_exception_occurred()));
    jl_value_t* err=jl_get_field(jl_exception_occurred(),"msg");
    if(err!=NULL) printf("%s: %s\n",jl_typeof_str(jl_exception_occurred()),jl_string_ptr(err));
    jl_exception_clear();
    rb_str_cat2(resRb, ")");
  }
  rb_str_cat2(resRb, "__");
  return resRb;
}

/***************** EVAL **********************/

VALUE Julia_eval(VALUE obj, VALUE cmd, VALUE print_stdout)
{
  char *cmdString;
  jl_value_t *res;
  VALUE resRb;

  cmdString=StringValuePtr(cmd);
  //printf("cmd=%s\n",cmdString);
//#ifndef WITH_JULIA_RELEASE
  //This flush redirected stdout before printing
  //if(print_stdout!=Qnil) jlapi_get_stdout();
//#endif
  //jl_gc_disable();
  res=jl_eval_string(cmdString);
  //printf("cmd=%s\n",cmdString);
  if (jl_exception_occurred()) {
            //jl_show(jl_stderr_obj(), jl_exception_occurred());
            jl_call2(jl_get_function(jl_base_module, "show"), jl_stderr_obj(), jl_exception_occurred());
            jl_printf(jl_stderr_stream(), "\n");
            resRb=Qnil;
  } else {
    //JL_GC_PUSH1(&res);
    //printf("cmd=%s\n",cmdString);
  //jl_set_global(jl_base_module, jl_symbol("ans"),res);
//#ifndef WITH_JULIA_RELEASE
//  if(print_stdout!=Qnil) jlapi_print_stdout();
//#endif
  resRb=jl_value_to_VALUE(res);
  //JL_GC_POP();
  }
  //jl_gc_enable();
  return resRb;
}

VALUE Julia_exec(VALUE obj, VALUE cmd, VALUE get_stdout)
{
  char *cmdString,*outString;
  jl_value_t *res;
  VALUE out;

  cmdString=StringValuePtr(cmd);
  res=jl_eval_string(cmdString);
  jl_set_global(jl_base_module, jl_symbol("ans"),res);
  //if(get_stdout!=Qnil) {
  //  outString=jlapi_get_stdout();
  //  jl_set_global(jl_base_module, jl_symbol("rbout"),jl_cstr_to_string(outString));
  //  return  rb_str_new2(outString);
  //} else
  return Qnil;
}


/// Util

int util_isVector(jl_value_t *ans)
{
  return (strcmp(jl_typeof_str(ans),"Array")==0) && (jl_array_rank(ans)==1);
}

int util_isVariable(VALUE self)
{
  VALUE tmp;
  tmp=rb_iv_get(self,"@type");
  return strcmp(StringValuePtr(tmp),"var")==0;
}

jl_value_t *util_getVar(VALUE self)
{
  jl_value_t *ans;
  char *name;
  VALUE tmp;

  tmp=rb_iv_get(self,"@name");
  name=StringValuePtr(tmp);
  ans=jl_eval_string(name);
  //printf("name=%s,vector=%d\n",name,util_isVector(ans));
  if(!util_isVector(ans)) return NULL;
  return ans;
}

//with argument!! necessarily an expression and not a variable
jl_value_t *util_getExpr_with_arg(VALUE self)
{
  jl_value_t *ans;
  VALUE tmp;
  char *cmd;

  //printf("getVar:%s\n",name);
  tmp=rb_str_dup(rb_iv_get(self,"@arg"));
  tmp=rb_str_cat2(rb_str_dup(rb_iv_get(self,"@name")),StringValuePtr(tmp));
  cmd=StringValuePtr(tmp);
  ans=jl_eval_string(cmd);
  if(ans==NULL) return ans;
  //if(!util_isVector(ans)) return NULL;
  return ans;
}


VALUE util_jl_value_to_VALUE(jl_value_t *ans)
{
  return jl_value_to_VALUE(ans);
}


//-| Transform only a Vector or Atom
//-| Todo: when not a vector, avoid transform to vector first.
jl_value_t* util_VALUE_to_jl_value(VALUE arr)
{
  jl_value_t *ans,*elt,*array_type;
  VALUE res,class,tmp;
  int i,n=0,vect=1;

  if(!rb_obj_is_kind_of(arr,rb_cArray)) {
    n=1;
    res = rb_ary_new2(1);
    rb_ary_push(res,arr);
    arr=res;
    vect=0;
  } else {
    n=RARRAY_LEN(arr);
  }

  class=rb_class_of(rb_ary_entry(arr,0));
  
  if(class==rb_cFloat) {
    //-| This is maybe faster and can be developped in julia-api as jl_vector_float64(n) for example.
    array_type=jl_apply_array_type((jl_value_t*)jl_float64_type, 1);
    ans=(jl_value_t*)jl_alloc_array_1d(array_type,n);
    for(i=0;i<n;i++) {
      elt=jl_box_float64(NUM2DBL(rb_ary_entry(arr,i)));
      jl_arrayset((jl_array_t*)ans,elt,i);
    }
#if RUBY_API_VERSION_CODE >= 20400
  } else if(class==rb_cInteger) {
#else
  } else if(class==rb_cFixnum || class==rb_cBignum) {
#endif
    array_type=jl_apply_array_type((jl_value_t*)jl_long_type, 1);
    ans=(jl_value_t*)jl_alloc_array_1d(array_type,n);
    for(i=0;i<n;i++) {
      elt=jl_box_long(NUM2INT(rb_ary_entry(arr,i)));
      jl_arrayset((jl_array_t*)ans,elt,i);
    }
  } else if(class==rb_cTrueClass || class==rb_cFalseClass) {
    array_type=jl_apply_array_type((jl_value_t*)jl_bool_type, 1);
    ans=(jl_value_t*)jl_alloc_array_1d(array_type,n);
    for(i=0;i<n;i++) {
      elt=jl_box_bool(rb_class_of(rb_ary_entry(arr,i))==rb_cFalseClass ? 0 : 1);
      jl_arrayset((jl_array_t*)ans,elt,i);
    }
  } else if(class==rb_cString) {
    array_type=jl_apply_array_type((jl_value_t*)jl_string_type, 1);
    ans=(jl_value_t*)jl_alloc_array_1d(array_type,n);
    for(i=0;i<n;i++) {
      tmp=rb_ary_entry(arr,i);
      elt=jl_cstr_to_string(StringValuePtr(tmp));
      jl_arrayset((jl_array_t*)ans,elt,i);
    }
  } else ans=NULL;
  if(!vect && ans) ans=jl_arrayref((jl_array_t*)ans,0);
  return ans;
}

VALUE JuliaVect_initialize(VALUE self, VALUE name)
{
  rb_iv_set(self,"@name",name);
  rb_iv_set(self,"@type",rb_str_new2("var"));
  rb_iv_set(self,"@arg",rb_str_new2(""));
  return self;
}

VALUE JuliaVect_isValid(VALUE self)
{
  jl_value_t* ans;
  char *name;

  ans = util_getVar(self);

  if(!util_isVector(ans)) {
    VALUE tmp;
    tmp=rb_iv_get(self,"@name");
    name = StringValuePtr(tmp);
    rb_warn("%s is not a R vector !!!",name);
    return Qfalse;
  }
  return Qtrue;
}

VALUE JuliaVect_length(VALUE self)
{
  jl_value_t* ans;
  char *name;

  ans = util_getVar(self);

  if(ans==NULL) {
    //printf("Sortie de length avec nil\n");
    return Qnil;
  }

  return INT2NUM(length(ans));
}

VALUE JuliaVect_get(VALUE self)
{
  jl_value_t* ans;
  VALUE res;
  char *name;
  int n,i;
  VALUE res2;

  ans = util_getVar(self);

  if(ans==NULL) {
    //printf("Sortie de get avec nil\n");
    return Qnil;
  }

  res=util_jl_value_to_VALUE(ans);
  if(length(ans)==1) res=rb_ary_entry(res,0);
  return res;
}

VALUE JuliaVect_get_with_arg(VALUE self)
{
  jl_value_t* ans;
  VALUE res;
  char *name;
  int n,i;
  VALUE res2;

  ans = util_getExpr_with_arg(self);

  if(ans==NULL) {
    //printf("Sortie de get avec nil\n");
    return Qnil;
  }
  res=util_jl_value_to_VALUE(ans);

//printf("JuliaVect_get_with_arg: length(ans)=%d\n",length(ans));
 if (length(ans)==1) res=rb_ary_entry(res,0);

  return res;
}



// faster than self.to_a[index]
// VALUE JuliaVect_aref(VALUE self, VALUE index)
// {
//   jl_value_t* ans;
//   VALUE res;
//   int n,i;
//   i = FIX2INT(index);

//   ans = util_getVar(self);
//   n=length(ans);
//   printf("i=%d and n=%d\n",i,n);
//   if(i<n) {
//      res=jl_value_to_VALUE(jl_arrayref((jl_array_t *)ans,i));
//   } else {
//     res = Qnil;
//   }
//   return res;
// }

// VALUE JuliaVect_set(VALUE self,VALUE arr)
// {
//   jl_value_t* ans;
//   char *name;
//   VALUE tmp;

//   ans=util_VALUE_to_jl_value(arr);

//   tmp=rb_iv_get(self,"@name");
//   name = StringValuePtr(tmp);
//   jl_set_global(jl_main_module, jl_symbol(name),ans);

//   return self;
// }

VALUE JuliaVect_set(VALUE self,VALUE arr)
{
  VALUE tmp;
  char *cmd;
  // defineVar(install(".rubyExport"),util_VALUE2jl_value_t*(arr),R_GlobalEnv);
  // tmp=rb_iv_get(self,"@arg");
  // util_eval1string(rb_str_cat2(rb_str_cat2(rb_str_dup(rb_iv_get(self,"@name")),StringValuePtr(tmp)),"<-.rubyExport"));
  jl_set_global(jl_main_module, jl_symbol("_ruby_export_"),util_VALUE_to_jl_value(arr));
  tmp=rb_str_dup(rb_iv_get(self,"@name"));
  tmp=rb_str_cat2(tmp,"=_ruby_export_");
  cmd=StringValuePtr(tmp);
  // printf("cmd=%s\n",cmd);
  // jl_eval_string("show(_ruby_export_)");
  // printf("cmd->done\n");
  jl_eval_string(cmd);
  return self;
}

VALUE JuliaVect_assign(VALUE obj, VALUE name,VALUE arr)
{
  jl_value_t* ans;
  char *tmp;

  ans=util_VALUE_to_jl_value(arr);

  tmp = StringValuePtr(name);
  jl_set_global(jl_main_module, jl_symbol(tmp),ans);

  return Qnil;
}

VALUE JuliaVect_set_with_arg(VALUE self,VALUE arr)
{
  VALUE tmp;
  char *cmd;
  // defineVar(install(".rubyExport"),util_VALUE2jl_value_t*(arr),R_GlobalEnv);
  // tmp=rb_iv_get(self,"@arg");
  // util_eval1string(rb_str_cat2(rb_str_cat2(rb_str_dup(rb_iv_get(self,"@name")),StringValuePtr(tmp)),"<-.rubyExport"));
  jl_set_global(jl_main_module, jl_symbol("_ruby_export_"),util_VALUE_to_jl_value(arr));
  tmp=rb_str_dup(rb_iv_get(self,"@arg"));
  tmp=rb_str_cat2(rb_str_dup(rb_iv_get(self,"@name")),StringValuePtr(tmp));
  tmp=rb_str_cat2(tmp,"=_ruby_export_");
  cmd=StringValuePtr(tmp);
  jl_eval_string(cmd);
  return self;
}

void
Init_jl4rb()
{
  VALUE mJulia;

  mJulia = rb_define_module("Julia");

  rb_define_module_function(mJulia, "initJL", Julia_init, 0);

  rb_define_module_function(mJulia, "exitJL", Julia_exit, 1);

  rb_define_module_function(mJulia, "evalLine", Julia_eval, 2);

  rb_define_module_function(mJulia, "execLine", Julia_exec, 2);

  VALUE cJuliaVect;

  cJuliaVect = rb_define_class_under(mJulia,"Vector",rb_cObject);

  rb_define_module_function(cJuliaVect, "assign", JuliaVect_assign, 2);

  rb_define_method(cJuliaVect,"initialize",JuliaVect_initialize,1);

  rb_define_method(cJuliaVect,"get",JuliaVect_get,0);
  rb_define_alias(cJuliaVect,"to_a","get");
  rb_define_alias(cJuliaVect,"value","get");

  rb_define_method(cJuliaVect,"set",JuliaVect_set,1);
  rb_define_alias(cJuliaVect,"<","set");
  rb_define_alias(cJuliaVect,"value=","set");

  //method "arg=" defined in eval.rb!! @arg initialized in method "initialize"
  rb_define_method(cJuliaVect,"get_with_arg",JuliaVect_get_with_arg,0);
  rb_define_alias(cJuliaVect,"value_with_arg","get_with_arg");
  rb_define_method(cJuliaVect,"set_with_arg",JuliaVect_set_with_arg,1);
  rb_define_alias(cJuliaVect,"value_with_arg=","set_with_arg");

  rb_define_method(cJuliaVect,"valid?",JuliaVect_isValid,0);
  rb_define_method(cJuliaVect,"length",JuliaVect_length,0);
  //rb_define_method(cJuliaVect,"[]",JuliaVect_aref,1);

  rb_define_attr(cJuliaVect,"name",1,1);
  rb_define_attr(cJuliaVect,"type",1,1);

}
