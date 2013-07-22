Gem::Specification.new do |s|
    s.platform = Gem::Platform::CURRENT
    s.summary = "Julia for ruby"
    s.name = "jl4rb"
    s.version = '0.0.1'
    s.requirements << 'none'
    s.require_paths = ["lib"]
    s.files = Dir['lib/**/*.rb'] + Dir['lib/*.so']
    s.required_ruby_version = '>= 1.8.0'
    s.description = <<-EOF
  Julia embedded in ruby with some communication support .
  EOF
    s.author = "CQLS"
    s.email= "rdrouilh@gmail.com"
    s.homepage = "http://cqls.upmf-grenoble.fr"
    s.rubyforge_project = nil
    s.has_rdoc = false
end
