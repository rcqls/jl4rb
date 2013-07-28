require 'rubygems'
require 'rake'
require 'rubygems/package_task'

PKG_NAME='jl4rb'
PKG_VERSION='0.0.1'
PKG_FILES=FileList[
    'Rakefile','jl4rb.gemspec',
    'ext/jl4rb/*.c',
    'ext/jl4rb/extconf.rb',
    'ext/jl4rb/MANIFEST',
    'lib/**/*.rb', 
    'test/**/*.rb'
]

spec = Gem::Specification.new do |s|
    s.platform = Gem::Platform::RUBY
    s.summary = "Julia for ruby"
    s.name = PKG_NAME
    s.version = PKG_VERSION
    s.requirements << 'none'
    s.require_paths = ["lib","ext/jl4rb"]
    s.files = PKG_FILES.to_a
    s.extensions = ["ext/jl4rb/extconf.rb"]
    s.licenses = ['MIT', 'GPL-2']
    s.description = <<-EOF
  R is embedded in ruby with some communication support .
  EOF
    s.author = "CQLS"
    s.email= "rdrouilh@gmail.com"
    s.homepage = "http://cqls.upmf-grenoble.fr"
    s.rubyforge_project = nil
end

## this allows to produce some parameter for task like  Gem::PackageTask (without additional argument!)
opt={};ARGV.select{|e| e=~/\=/ }.each{|e| tmp= e.split("=");opt[tmp[0]]=tmp[1]}

## rake ... pkgdir=<path to provide> to update PKGDIR
PKGDIR=opt["pkgdir"] || ENV["RUBYGEMS_PKGDIR"]  || "pkg"
## gem task!!!
desc "Create #{PKG_NAME+'-'+PKG_VERSION+'.gem'}" 
Gem::PackageTask.new(spec) do |pkg|
  pkg.package_dir=PKGDIR
  pkg.need_zip = false
  pkg.need_tar = false
end

## clean task
desc "Remove #{File.join(PKGDIR,PKG_NAME+'-'+PKG_VERSION+'.gem')}"
task :clean do |t|
  rm File.join(PKGDIR,PKG_NAME+'-'+PKG_VERSION+'.gem') if File.exists? File.join(PKGDIR,PKG_NAME+'-'+PKG_VERSION+'.gem')
  rm_rf File.join(PKGDIR,PKG_NAME+'-'+PKG_VERSION) if File.exists? File.join(PKGDIR,PKG_NAME+'-'+PKG_VERSION)
end


## install task with doc
desc "Install #{File.join(PKGDIR,PKG_NAME+'-'+PKG_VERSION+'.gem')}"
task :install_with_doc do |t|
  `gem install #{File.join(PKGDIR,PKG_NAME+'-'+PKG_VERSION+'.gem')} --local`
end

## quick install task
desc "Quick install #{File.join(PKGDIR,PKG_NAME+'-'+PKG_VERSION+'.gem')}"
task :install do |t|
  `gem install #{File.join(PKGDIR,PKG_NAME+'-'+PKG_VERSION+'.gem')} --local --no-rdoc --no-ri`
  rm_rf File.join(PKGDIR,PKG_NAME+'-'+PKG_VERSION) if File.exists? File.join(PKGDIR,PKG_NAME+'-'+PKG_VERSION)
end


## binary task (mainly for Windows binary)
spec_bin=Gem::Specification.new do |s|
    s.platform = Gem::Platform::CURRENT
    s.summary = "Julia for ruby"
    s.name = PKG_NAME
    s.version = PKG_VERSION
    s.requirements << 'none'
    s.require_paths = ["lib"]
    s.files = Dir['lib/**/*.rb'] + Dir['lib/*.so']
    s.required_ruby_version = '>= 1.8.0'
    s.description = <<-EOF
  R is embedded in ruby with some communication support .
  EOF
    s.author = "CQLS"
    s.email= "rdrouilh@gmail.com"
    s.homepage = "http://cqls.upmf-grenoble.fr"
    s.rubyforge_project = nil
    s.has_rdoc = false
end

task :gem_bin do |t|
  `cp ext/jl4rb/*.so lib` 
  Gem::Builder.new(spec_bin).build
end
