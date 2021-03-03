require "mkmf-rice"

$CXXFLAGS += " -std=c++17"

ext = File.expand_path(".", __dir__)
datasketches = File.expand_path("../../vendor/datasketches-cpp", __dir__)

$srcs = Dir["#{ext}/*.cpp"]

%w(common cpc fi hll kll sampling theta tuple).each do |v|
  $INCFLAGS += " -I#{datasketches}/#{v}/include"
end

create_makefile("datasketches/ext")
