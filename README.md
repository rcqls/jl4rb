# Julia for Ruby

This is an attempt to embed the julia language in ruby. Actually, very basic julia types are converted to ruby objects.


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

If the last result is unexpected, see the Troubles section.

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

1. htableh.inc in src/support directory is missing (copy it in include/julia of your julia directory). 
*Update*: htableh.inc is now in the package (ext/jl4rb) until the julia core solve the problem.

2. User linux needs to add jl_bytestring_ptr to julia.expmap to be able to compile jl4rb package. Maybe some source headers need to be copied inside the include/julia directory. 

3. Notice that now there is no more issue on the setting of LD_LIBRARY_PATH since DL_LOAD_PATH is initialized when first calling Julia.init and just before the initialization of LOAD_PATH (which from time to time needs DL_LOAD_PATH already set).

