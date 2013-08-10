# Julia for Ruby

This is an attempt to embed the julia language in ruby. 
Actually, very basic julia types are converted to ruby objects.


## Requirement

[julia-api4rcqls](https://github.com/rcqls/julia-api4rcqls): since the current project is a work in progress 
and very experimental, we prefer to use the julia-api4rcqls tools. 
More, julia-api4rcqls is used both in [jl4R](https://github.com/rcqls/jl4R) and [jl4rb](https://github.com/rcqls/jl4rb) projects.


## Install

Clone this git and

	rake package
	[sudo] gem install pkg/jl4rb-???.gem
		

## Test

First, in a terminal or in your .bashrc (or equivalent):

	export JLAPI_HOME=<your julia home>

Then, in a irb console:

```{.ruby execute="false"}
require 'jl4rb'			# => true
Julia << 'LOAD_PATH'	# => [<your julia home>/local/share/julia/site/v0.2", "<your julia home>/share/julia/site/v0.2"]
```

## Example
```{.ruby execute="false"}
require 'jl4rb'
# Julia.init since automatically called once
Julia << 'using RDatasets' # A bit slow, julia and RDatasets initializations

# A multilines call 
a=Julia << <<JLEND
iris=data("datasets","iris")
vector(iris[2])
JLEND

# a is then a ruby object
p a

# another call
Julia << 'colnames(iris)'
```

## Troubles

1. User linux needs to add jl_bytestring_ptr to julia.expmap to be able to compile jl4rb package. Maybe some source headers need to be copied inside the include/julia directory. 

## Remark

1. Now tested on Windows thanks to the [julia-api4rcqls](https://github.com/rcqls/julia-api4rcqls) project extended for Windows.

