# Julia for Ruby

This is an attempt to embed the julia language in ruby.
Actually, very basic julia types are converted to ruby objects.

## Install

```{.bash}
[sudo] gem install jl4rb
```

## Test

Then, in a irb console:

```{.ruby execute="false"}
require 'jl4rb'			# => true
Julia << 'LOAD_PATH'
```

## Example

Once you haved installed `RDatasets` package in `julia`.

```{.ruby execute="false"}
require 'jl4rb'
Julia << 'using RDatasets'  

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