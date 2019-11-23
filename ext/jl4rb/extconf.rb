require "mkmf"
require 'fileutils'

$prefix_include,$prefix_lib=[],[]
$prefix= File.dirname(`julia -e 'print(Sys.BINDIR)'`)

[$prefix+"/include/julia",$prefix+"/usr/include",$prefix+"/src",$prefix+"/src/support"].each do |incl|
    $prefix_include << incl if File.exists? incl
end

([$prefix+"/lib/julia",$prefix+"/usr/lib",$prefix+"/lib"]+(RUBY_PLATFORM=~/(?:mingw|msys)/ ? [$prefix+"/bin"] : [])).each do |lib|
    $prefix_lib << lib if File.exists? lib
end

def jl4rb_makefile(incs,libs)

    jl_share=`julia -e 'print(joinpath(Sys.BINDIR, Base.DATAROOTDIR, "julia"))'`
    $CFLAGS     =`julia #{jl_share}/julia-config.jl --cflags`.strip
    $LDFLAGS    =`julia #{jl_share}/julia-config.jl --ldflags`.strip
    $libs    =`julia #{jl_share}/julia-config.jl --ldlibs`.strip


    header = nil

    rb4r_name="jl4rb"
    $objs = [rb4r_name+".o"]

    dir_config("R4rb")
    p ( {:CFLAGS => $CFLAGS, :LDFLAGS => $LDFLAGS, :libs => $libs })
    create_makefile(rb4r_name)
end

jl4rb_makefile($prefix_include,$prefix_lib)
