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

#endif