require 'jl4rb'

Julia.init

# julia variable
if true
	aa=Julia::Vector.new "a"

	aa < [1,4,3]

	p "a".to_jl

	Julia << "a=[4,7,3,9]"

	p aa.value
end

if true
	Julia << "b=Dict()"

	bb=Julia::Vector.new ""
	bb << 'b["toto"]'

	bb < [1,4,66]

	p 'b["toto"]'.to_jl

	Julia << 'b["toto"]=[4,7,3,99]'

	p bb.value
end