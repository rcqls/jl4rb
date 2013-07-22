/**********************************************************************

  jl4rb.c

**********************************************************************/
#include <stdio.h>
#include <string.h>

#include "ruby.h"
//next macros already exist in ruby and are undefined here
#undef T_FLOAT
#undef NORETURN 
#include "julia.h"
#include "jl4rb.h"
//#define WITH_DL_LOAD_PATH

/************* INIT *********************/


VALUE Julia_init(VALUE obj, VALUE args)
{
  char **argv,*julia_home_dir;
  int i,argc;
  VALUE tmp;

  argc=RARRAY_LEN(args) + 1;
  tmp=rb_ary_entry(args,0);
  julia_home_dir=StringValuePtr(tmp);
  //printf("First initialization with julia_home_dir=%s\n",julia_home_dir);
   

  // As in jl_init from jlapi.c
  libsupport_init();
  char *image_file = jl_locate_sysimg(julia_home_dir);
  //printf("image-file=%s\n",image_file);
  julia_init(image_file);

  jl_set_const(jl_core_module, jl_symbol("JULIA_HOME"),
               jl_cstr_to_string(julia_home));
  jl_module_export(jl_core_module, jl_symbol("JULIA_HOME"));
  
  jl_eval_string("Base.reinit_stdio()");
  jl_eval_string("Base.fdwatcher_reinit()");
#ifdef WITH_DL_LOAD_PATH 
  jl_eval_string("Base.init_dl_load_path()");
  //VERY IMPORTANT; do no use 'joinpath' since it requires libpcre which needs DL_LOAD_PATH ) 
  jl_eval_string("push!(DL_LOAD_PATH,join([ENV[\"JL4RB_HOME\"],\"lib\",\"julia\"],Base.path_separator))");
#endif
  jl_eval_string("Base.init_load_path()");
  jl_eval_string("Random.librandom_init()");
  jl_eval_string("Base.check_blas()");
  jl_eval_string("LinAlg.init()");
  jl_eval_string("Sys.init()");
  jl_eval_string("Base.init_sched()");
  jl_eval_string("Base.init_head_sched()");
  return Qtrue;
}

//Maybe try to use cpp stuff to get the output inside julia system (ccall,cgen and cgutils)
VALUE jl_value_to_VALUE(jl_value_t *res) {
  size_t i=0,k,nd,d;
  VALUE res2;
  jl_value_t *tmp;
  jl_function_t *call;

  if(res!=NULL) {
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
    //if(jl_is_bool(res)) 
    {
      return rb_str_new2(jl_typename_str(res));
    }
    else 
    if(strcmp(jl_typeof_str(res),"Nothing")==0)
    //if(jl_is_bool(res)) 
    {
      return Qnil;
    }
    else
    if(strcmp(jl_typeof_str(res),"Complex")==0)
    //if(jl_is_bool(res)) 
    {
      res2 = rb_eval_string("require('complex');Complex.new(0,0)");
      rb_iv_set(res2,"@real",jl_value_to_VALUE(jl_get_field(res, "re")));
      rb_iv_set(res2,"@image",jl_value_to_VALUE(jl_get_field(res, "im")));
      return res2;
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
      res2 = jl_value_to_VALUE(jl_get_field(res, "pattern"));
      return res2;
    }
    else
    if(strcmp(jl_typeof_str(res),"ASCIIString")==0 || strcmp(jl_typeof_str(res),"UTF8String")==0)
    {
      //printf("value=%s\n",jl_bytestring_ptr(res));
      return rb_str_new2(jl_bytestring_ptr(res));
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
        res2 = rb_ary_new2(d);
        for(i=0;i<d;i++) {
          rb_ary_store(res2,i,jl_value_to_VALUE(jl_arrayref((jl_array_t *)res,i)));
        }
        return res2;
      }
      //TODO: multidim array ruby equivalent???? Is it necessary 
      
    }
    res2=rb_str_new2("__unconverted(");
    rb_str_cat2(res2, jl_typeof_str(res));
    rb_str_cat2(res2, ")__");
    // jl_function_t *call=(jl_function_t*)jl_get_global(jl_base_module, jl_symbol("show"));
    // if (call) jl_call1(call,res);
    // else printf("call failed!\n");

    return res2;
  }
  // No result (command incomplete or syntax error)
  res2=rb_str_new2("__incomplete");
  // if(jl_exception_occurred()!=NULL) {
  //   printf("error exception\n");
  // //   rb_str_cat2(res2, "(");
  // //   rb_str_cat2(res2, jl_typeof_str(jl_exception_occurred()));
  // //   rb_str_cat2(res2, ")");
  //    jl_exception_clear();
  // }
  rb_str_cat2(res2, "__");
  return res2;
}

/***************** EVAL **********************/

VALUE Julia_eval(VALUE obj, VALUE cmd)
{
  char *cmdString;
  jl_value_t *res;
   
  cmdString=StringValuePtr(cmd);
  res=jl_eval_string(cmdString);
  return jl_value_to_VALUE(res);
}

void
Init_jl4rb()
{
  VALUE mJulia;

  mJulia = rb_define_module("Julia");

  rb_define_module_function(mJulia, "initJL", Julia_init, 1);

  rb_define_module_function(mJulia, "evalLine", Julia_eval, 1);

}
