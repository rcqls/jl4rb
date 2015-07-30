require "mkmf"
require 'fileutils' #if RUBY_VERSION < "1.9"


$prefix_include,$prefix_lib=[],[]
if RUBY_PLATFORM =~ /linux/ and File.exists? "/usr/include/"+RUBY_PLATFORM+"/julia"
    $prefix_include << "/usr/include/"+RUBY_PLATFORM+"/julia"
    $prefix_lib << "/usr/lib/"+RUBY_PLATFORM+"/julia"
else
    #$prefix=ENV["JLAPI_HOME"] || File.join(ENV["HOME"],".jlapi/julia") || ENV["JULIA_HOME"]
    $prefix=ENV["JULIA_DIR"]
    #p  $prefix
    [$prefix+"/include/julia",$prefix+"/usr/include",$prefix+"/src",$prefix+"/src/support"].each do |incl| 
    	$prefix_include << incl if File.exists? incl
    end

    ([$prefix+"/lib/julia",$prefix+"/usr/lib"]+(RUBY_PLATFORM=~/(?:mingw|msys)/ ? [$prefix+"/bin"] : [])).each do |lib|
    	$prefix_lib << lib if File.exists? lib
    end
end

def jl4rb_makefile(incs,libs) 
    #$CFLAGS = "-I"+inc+" -I."
    # $CFLAGS = (enable_config("julia-release") ? "-DWITH_JULIA_RELEASE " : "")+ "-I"+inc+" -I."
    # $LDFLAGS = " -Wl,-rpath,"+lib+" -L"+lib if lib
    # $libs = (enable_config("julia-release") ? " -ljulia-release" : " -ljulia-api" )
 
 	$CFLAGS =   incs.map{|inc| "-I"+inc}.join(" ")+" -I."
    $LDFLAGS = " "+libs.map{|lib| "-Wl,-rpath,"+lib+" -L"+lib}.join(" ") if libs
    $libs =  " -ljulia" 
 

    header = nil

    rb4r_name="jl4rb"
    $objs = [rb4r_name+".o"]

    dir_config("R4rb")
    create_makefile(rb4r_name)
end

jl4rb_makefile($prefix_include,$prefix_lib)

 
