require "bundler/setup"
Bundler.require(:default)
require "minitest/autorun"
require "minitest/pride"

class Minitest::Test
  def setup
    puts "\n\n#{self.class.name}##{name}"
  end
end
