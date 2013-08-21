require 'jl4rb'

Julia.init

aa=Julia::Vector.new "a"

aa < [1,4,3]

p "a".to_jl

# In ruby
p aa[1]

# In Julia
aa.set_arg("[2]")

p aa.get_with_arg

aa.set_with_arg (12)

p "a".to_jl

aa[1]=33.0

p "a".to_jl
