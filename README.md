# Julia for Ruby

This is an attempt to embed the julia language in ruby. 
Actually, very basic julia types are converted to ruby objects.

## Install

Clone this git and

	export JULIA_DIR=<your julia directory> # or put it in your .bashrc (or equivalent)

	rake package
	[sudo] gem install pkg/jl4rb-???.gem
		

## Test

Then, in a irb console:

```{.ruby execute="false"}
require 'jl4rb'			# => true
Julia << 'LOAD_PATH'	# => [<your julia home>/local/share/julia/site/v0.3", "<your julia home>/share/julia/site/v0.3"]
```

## Example
```{.ruby execute="false"}
require 'jl4rb'
# Julia.init since automatically called once
Julia << 'using RDatasets' # A bit slow, julia and RDatasets initializations

# A multilines call 
a=Julia << <<JLEND
iris=dataset("datasets","iris")
array(iris[2])
JLEND

# a is then a ruby object
p a

# another call
Julia << 'map(string,names(iris))'
``` 

