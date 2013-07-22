# Julia for Ruby

This is an attempt to embed the julia language in ruby. Actually, very basic julia types are converted to ruby objects.


## Install

Clone this git and

	rake package
	[sudo] gem install pkg/jl4rb-???.gem
		

## Test

First, in a terminal or in your .bashrc (or equivalent):

	export JL4RB_HOME=<your julia home>

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

1. htableh.inc in src/support directory is missing (copy it in include/julia of your julia directory) 

2. If (like me, on MacOSX) the result of the previous test is wrong, the reason may come from the fact that in the initialization of julia libpcre is required and failed to be loaded properly. Then, set

	LD_LIBRARY_PATH=<your julia home>/lib/julia

If you don't want to set LD_LIBRARY_PATH, another solution is to change the base/client.jl file as follows: 

+ split init_load_path into 2 functions

```{.julia execute="false"}
	function init_load_path()
		vers = "v$(VERSION.major).$(VERSION.minor)"
		global const LOAD_PATH = ByteString[
			abspath(JULIA_HOME,"..","local","share","julia","site",vers),
			abspath(JULIA_HOME,"..","share","julia","site",vers)
		]
	end
	 
	function init_dl_load_path()
		global const DL_LOAD_PATH = ByteString[]
	end
```

+ in _start function:

```{.julia execute="false"}
	function _start()
		...
		init_load_path()
		init_dl_load_path() #to be added!
		...
	end
```

It is then possible to initialize julia like this (done with --enable-dl-load-path gem install option):

```{.julia execute="false"}
	init_dl_load_path()
	push!(DL_LOAD_PATH,"<your julia home>/julia/lib")
	init_load_path()
```

Once julia recompiled to provide the updated lib/julia/sys.ji, install the gem with this additional option:

	[sudo] gem install [pkg/]jl4rb-???.gem -- --enable-dl-load-path
