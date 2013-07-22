# Julia for Ruby

This is an attempt to embed the julia language in ruby.

## Install

		rake package
		[sudo] gem install [pkg/]jl4rb-???.gem

## Test

First,

		export JL4RB_HOME=<your julia home>

or set it in your .bashrc (or equivalent). Then, in a irb console:

		require 'jl4rb'
		Julia << 'LOAD_PATH'

## Troubles

1. htableh.inc in src/support directory is missing (copy it in include/julia of your julia directory) 

2. If (like me, on MacOSX) the result of the previous test is not the LOAD_PATH output expected, the reason may come from the fact that in the initialization of julia libpcre is required and failed to be loaded properly. Then, set

		LD_LIBRARY_PATH=<your julia home>/lib/julia

If you don't want to set LD_LIBRARY_PATH, another solution is to change the base/client.jl file as follows: 

+ split init_load_path into 2 functions

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

+ in _start function:

		init_load_path()
		init_dl_load_path() #to be added!

Then, install the gem with additional option

		[sudo] gem install [pkg/]jl4rb-???.gem -- --enable-dl-load-path
