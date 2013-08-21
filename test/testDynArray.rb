require 'jl4rb'
require 'R4rb'

module Dyndoc
	
	class Array < Array

		attr_accessor :jl, :r
		def initialize
			Array.initR
			Julia.init
			super
			@jl=Julia::Vector.new ids(:jl)
			@r=R4rb::RVector.new ids(:r)
		end

		def ids(lang)
			case lang  
			when :jl
				"_dynStack_"+self.object_id.abs.to_s
			when :r
				".dynStack."+self.object_id.abs.to_s
			end
		end

		# when modified from ruby, other languages are updated
		def []=(key,val)
			super
			@jl < self
			@r < self
			self
		end

		def [](key)
			if self!=@jl.value
			@jl > self
			@r < self
			super
		end

		def replace(ary)
			super(ary)
			@jl < self
			@r < self
			self
		end


	end

end 