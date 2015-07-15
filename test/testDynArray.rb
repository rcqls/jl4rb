require 'R4rb'
require 'jl4rb'
require File.expand_path("~/Github/dyndoc/lib/dyndoc/common/dynArray")

Array.initR
Julia.init

a=[] < 'jl:[2*i for i=1:4]'

p a

a > 'R:b'

R4rb << 'print(b)' 
