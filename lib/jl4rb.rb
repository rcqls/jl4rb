require 'jl4rb.bundle' if File.exists? File.join(File.dirname(__FILE__),'jl4rb.bundle') 
require 'jl4rb.so' if File.exists? File.join(File.dirname(__FILE__),'jl4rb.so')

# loading ruby files
require 'jl4rb/jl2rb_init'
require 'jl4rb/jl2rb_eval'
