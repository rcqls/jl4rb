require "mkmf"
require 'fileutils' #if RUBY_VERSION < "1.9"

$prefix=ENV["JLAPI_HOME"] || File.join(ENV["HOME"],".jlapi/julia") || ENV["JULIA_HOME"]
#p  $prefix
$prefix_include=$prefix+"/include/julia"
$prefix_lib=$prefix+"/lib/julia"

def jl4rb_makefile(inc,lib) 
    #$CFLAGS = "-I"+inc+" -I."
    $CFLAGS = (enable_config("dl-load-path") ? "-DWITH_DL_LOAD_PATH " : "")+ "-I"+inc+" -I."
    $LDFLAGS = " -Wl,-rpath,"+lib+" -L"+lib if lib
    $libs = " -ljulia-api"
 
    header = nil

    rb4r_name="jl4rb"
    $objs = [rb4r_name+".o"]

    dir_config("R4rb")
    create_makefile(rb4r_name)
end

jl4rb_makefile($prefix_include,$prefix_lib)

 
