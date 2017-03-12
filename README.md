# Julia for Ruby

This is an attempt to embed the julia language in ruby.
Actually, very basic julia types are converted to ruby objects.

## Install

Clone this git and
```{.bash}
export JULIA_DIR=<your julia directory> # or put it in your .bashrc (or equivalent)
## for linux ubuntu
export JULIA_DIR=/usr
## for MacOSX
export JULIA_DIR=/Applications/Julia-0.5.app/Contents/Resources/julia
```

```{.bash}
rake package
[sudo] gem install pkg/jl4rb-???.gem
```

## Test

Then, in a irb console:

```{.ruby execute="false"}
require 'jl4rb'			# => true
Julia << 'LOAD_PATH'	# => ["/Applications/Julia-0.5.app/Contents/Resources/julia/local/share/julia/site/v0.5", "/Applications/Julia-0.5.app/Contents/Resources/julia/share/julia/site/v0.5"]
Julia << 'Libdl.DL_LOAD_PATH'
## required for RDatasets because libz.dylib not found for MacOSX
Julia << 'push!(Libdl.DL_LOAD_PATH,"/usr/lib");push!(Libdl.DL_LOAD_PATH,"/usr/local/lib")'
```

## Example
```{.ruby execute="false"}
require 'jl4rb'
# Julia.init since automatically called once
Julia << 'using RDatasets' # A bit slow, julia and RDatasets initializations

# A multilines call
a=Julia << <<JLEND
iris=dataset("datasets","iris")
convert(Array,iris[2])
JLEND

# a is then a ruby object
p a

# another call
Julia << 'map(string,names(iris))'
```

## Remark

Maybe, this (or something similar) needs to be added in your .bash_profile for Mac users:

	export DYLD_FALLBACK_LIBRARY_PATH=$JULIA_DIR/lib/julia:/usr/lib
