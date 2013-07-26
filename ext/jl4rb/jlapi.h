#ifndef JLAPI_H
#define JLAPI_H
extern char *julia_home;

//No header jlapi.h in julia => added here locally 
DLLEXPORT char *jl_locate_sysimg(char *jlhome);
DLLEXPORT void *jl_eval_string(char *str);
DLLEXPORT void jl_init(char *julia_home_dir);
DLLEXPORT const char *jl_typename_str(jl_value_t *v);
DLLEXPORT const char *jl_typeof_str(jl_value_t *v);
DLLEXPORT const char *jl_bytestring_ptr(jl_value_t *s);
DLLEXPORT void *jl_array_eltype(jl_value_t *a);
DLLEXPORT int jl_array_rank(jl_value_t *a);
DLLEXPORT size_t jl_array_size(jl_value_t *a, int d);
static size_t array_nd_index(jl_array_t *a, jl_value_t **args, size_t nidxs, char *fname);
DLLEXPORT jl_value_t *jl_get_field(jl_value_t *o, char *fld);
DLLEXPORT jl_value_t *jl_exception_occurred(void);
DLLEXPORT void jl_exception_clear(void);

/************* Tools ********************/

//-| jl4rb_init is an small adaptation of jl_init provided in jlapi.c
//-| Main change: 
//-| -> use of DL_LOAD_PATH (slight modification of init_load_path in client.jl)
//-| -> redirection of STDIN, STDOUT, STDERR
//-|    Have to solve a strange behavior: Base.reinit_stdio fail to reload STDIN, STDOUT,STDERR (=> print failed)

void jlapi_init(char *julia_home_dir) {
  libsupport_init();
  char *image_file = jl_locate_sysimg(julia_home_dir);
  printf("image-file=%s\n",image_file);
  julia_init(image_file);

  jl_set_const(jl_core_module, jl_symbol("JULIA_HOME"),
               jl_cstr_to_string(julia_home));
  jl_module_export(jl_core_module, jl_symbol("JULIA_HOME"));
  
 
  //-| Called first to fix the DL_LOAD_PATH needed to (dl)open library (libpcre for example)
  //-| Replacement of Base.init_load_path()
  jl_set_global(jl_base_module,jl_symbol("DL_LOAD_PATH"),jl_eval_string("ByteString[join([JULIA_HOME,\"..\",\"lib\",\"julia\"],Base.path_separator)]"));
  //-| DL_LOAD_PATH is a global constant already defined before and then not overloaded by julia
  //-| Only LOAD_PATH would be initialized (needs libpcre because of abspath)!
  jl_eval_string("Base.init_load_path()");
  jl_eval_string("Base.reinit_stdio()");
  //-| STDIN, STDOUT and STDERR not properly loaded
  //-| I prefer redirection of STDOUT and STDERR in IOBuffer (maybe STDIN ???)
  jl_set_global(jl_base_module,jl_symbol("STDIN"),jl_eval_string("Base.init_stdio(ccall(:jl_stdin_stream ,Ptr{Void},()),0)"));
  jl_set_global(jl_base_module,jl_symbol("STDOUT"),jl_eval_string("IOBuffer()"));
  jl_set_global(jl_base_module,jl_symbol("STDERR"),jl_eval_string("IOBuffer()"));
  //-| 2 next lines fails even it is if no more necessary
  // jl_set_global(jl_base_module,jl_symbol("STDOUT"),jl_eval_string("Base.init_stdio(ccall(:jl_stdout_stream,Ptr{Void},()),1)"));
  // jl_set_global(jl_base_module,jl_symbol("STDERR"),jl_eval_string("Base.init_stdio(ccall(:jl_stderr_stream,Ptr{Void},()),2)"));
  jl_eval_string("Base.fdwatcher_reinit()");
  jl_eval_string("Base.Random.librandom_init()");
  jl_eval_string("Base.check_blas()");
  jl_eval_string("LinAlg.init()");
  jl_eval_string("Sys.init()");
  jl_eval_string("Base.init_sched()");
  jl_eval_string("Base.init_head_sched()"); 
  jl_eval_string("println(\"Julia initialized!\")");
}

//-| Get STDOUT, STDERR IOBuffer as string

char *jlapi_get_stdout() {
  jl_value_t *out;
  
  out=jl_eval_string("seek(STDOUT, 0);jl4rb_out = takebuf_string(STDOUT);truncate(STDOUT, 0);jl4rb_out");
  return jl_bytestring_ptr(out);
}

char *jlapi_get_stderr() {
  jl_value_t *out;
  
  out=jl_eval_string("seek(STDERR, 0);jl4rb_out = takebuf_string(STDERR);truncate(STDERR, 0);jl4rb_out");
  return jl_bytestring_ptr(out);
}

//-| Print STDOUT, STDERR IOBuffer
void jlapi_print_stdout() {
  char *outString;
  
  outString=jlapi_get_stdout();
  if(strlen(outString)) printf("%s\n",outString);
}

void jlapi_print_stderr() {
  char *outString;
   
  outString= jlapi_get_stderr();
  if(strlen(outString)) printf("%s\n",outString);
}



#endif