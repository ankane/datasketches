require_relative "lib/datasketches/version"

Gem::Specification.new do |spec|
  spec.name          = "datasketches"
  spec.version       = DataSketches::VERSION
  spec.summary       = "Sketch data structures for Ruby"
  spec.homepage      = "https://github.com/ankane/datasketches"
  spec.license       = "Apache-2.0"

  spec.author        = "Andrew Kane"
  spec.email         = "andrew@chartkick.com"

  spec.files         = Dir["*.{md,txt}", "LICENSE", "NOTICE", "{ext,lib,vendor}/**/*"]
  spec.require_path  = "lib"
  spec.extensions    = ["ext/datasketches/extconf.rb"]

  spec.required_ruby_version = ">= 2.6"

  spec.add_dependency "rice", ">= 2.2"
end
