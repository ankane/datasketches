#include <sstream>

#include <cpc_sketch.hpp>
#include <cpc_union.hpp>

#include <rice/rice.hpp>

using datasketches::cpc_sketch;
using datasketches::cpc_union;

using datasketches::CPC_DEFAULT_LG_K;
using datasketches::DEFAULT_SEED;

using Rice::Arg;

void init_cpc(Rice::Module& m) {
  Rice::define_class_under<cpc_sketch>(m, "CpcSketch")
    .define_constructor(Rice::Constructor<cpc_sketch, uint8_t, uint64_t>(), Rice::Arg("lg_k")=CPC_DEFAULT_LG_K, Rice::Arg("seed")=DEFAULT_SEED)
    .define_method("lg_k", &cpc_sketch::get_lg_k)
    .define_method("empty?", &cpc_sketch::is_empty)
    .define_method("lower_bound", &cpc_sketch::get_lower_bound)
    .define_method("upper_bound", &cpc_sketch::get_upper_bound)
    .define_method(
      "update",
      [](cpc_sketch& self, Rice::Object datum) {
        if (FIXNUM_P(datum.value())) {
          return self.update(Rice::detail::From_Ruby<int64_t>::convert(datum));
        } else if (datum.is_a(rb_cNumeric)) {
          return self.update(Rice::detail::From_Ruby<double>::convert(datum));
        } else {
          return self.update(datum.to_s().str());
        }
      })
    .define_method(
      "estimate",
      [](cpc_sketch& self) {
        return self.get_estimate();
      })
    .define_method(
      "serialize",
      [](cpc_sketch& self) {
        std::ostringstream oss;
        self.serialize(oss);
        return oss.str();
      })
    // TODO change to summary?
    .define_method(
      "to_string",
      [](cpc_sketch& self) {
        return self.to_string();
      })
    .define_singleton_method(
      "deserialize",
      [](std::string& is) {
        std::istringstream iss(is);
        return cpc_sketch::deserialize(iss);
      });

  Rice::define_class_under<cpc_union>(m, "CpcUnion")
    .define_constructor(Rice::Constructor<cpc_union, uint8_t, uint64_t>(), Rice::Arg("lg_k"), Rice::Arg("seed")=DEFAULT_SEED)
    .define_method("result", &cpc_union::get_result)
    .define_method(
      "update",
      [](cpc_union& self, cpc_sketch& sketch) {
        self.update(sketch);
      });
}
