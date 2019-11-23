require 'rubygems'
require 'rubygems/package_task'

PKG_NAME='jl4rb'
PKG_VERSION='0.2.2'
PKG_FILES=FileList[
    'Rakefile','jl4rb.gemspec',
    'ext/jl4rb/*.c',
    'ext/jl4rb/extconf.rb',
    'ext/jl4rb/*.h',
    'ext/jl4rb/MANIFEST',
    'lib/**/*.rb',
    'test/**/*.rb'
]

Gem::Specification.new do |s|
  s.platform = Gem::Platform::RUBY
  s.summary = "Julia for ruby"
  s.name = PKG_NAME
  s.version = PKG_VERSION
  s.requirements << 'none'
  s.require_paths = ["lib","ext/jl4rb"]
  s.files = PKG_FILES.to_a
  s.extensions = ["ext/jl4rb/extconf.rb"]
  s.licenses = ['MIT', 'GPL-2.0']
  s.description = <<-EOF
R is embedded in ruby with some communication support .
EOF
  s.author = "CQLS"
  s.email= "rdrouilh@gmail.com"
  s.homepage = "http://cqls.upmf-grenoble.fr"
  s.rubyforge_project = nil
end
