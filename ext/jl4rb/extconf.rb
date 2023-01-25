require "mkmf"
require 'fileutils'

$prefix_include,$prefix_lib=[],[]
$prefix= File.dirname(`julia -e 'print(Sys.BINDIR)'`)

[$prefix+"/include/julia",$prefix+"/usr/include",$prefix+"/src",$prefix+"/src/support"].each do |incl|
    $prefix_include << incl if File.exist? incl
end

([$prefix+"/lib/julia",$prefix+"/usr/lib",$prefix+"/lib"]+(RUBY_PLATFORM=~/(?:mingw|msys)/ ? [$prefix+"/bin"] : [])).each do |lib|
    $prefix_lib << lib if File.exist? lib
end

def jl4rb_makefile(incs,libs)

    jl_share=`julia -e 'print(joinpath(Sys.BINDIR, Base.DATAROOTDIR, "julia"))'`
    $CFLAGS     =`julia #{jl_share}/julia-config.jl --cflags`.strip + (RUBY_VERSION.split(".")[0] > "2" ? " -fdeclspec" : "")
    $LDFLAGS    =`julia #{jl_share}/julia-config.jl --ldflags`.strip
    $LIBS    = `julia #{jl_share}/julia-config.jl --ldlibs`.strip

    puts [$CFLAGS, $LDFLAGS, $LIBS]

    header = nil

    jl4r_name="jl4rb"
    $objs = [jl4r_name+".o"]

    dir_config("jl4rb")
    # 
    p ( {:CFLAGS => $CFLAGS, :LDFLAGS => $LDFLAGS, :LIBS => $LIBS })
    create_makefile(jl4r_name)
end
#p [$prefix_include,$prefix_lib]
jl4rb_makefile($prefix_include,$prefix_lib)
