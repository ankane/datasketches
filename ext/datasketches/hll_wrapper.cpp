#include <sstream>

#include <hll.hpp>

#include <rice/rice.hpp>

using datasketches::hll_sketch;
using datasketches::hll_union;

void init_hll(Rice::Module& m) {
  Rice::define_class_under<hll_sketch>(m, "HllSketch")
    .define_constructor(Rice::Constructor<hll_sketch, int>())
    .define_method("lg_config_k", &hll_sketch::get_lg_config_k)
    .define_method("compact?", &hll_sketch::is_compact)
    .define_method("empty?", &hll_sketch::is_empty)
    .define_method("composite_estimate", &hll_sketch::get_composite_estimate)
    .define_method(
      "update",
      [](hll_sketch& self, Rice::Object datum) {
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
      [](hll_sketch& self) {
        return self.get_estimate();
      })
    .define_method(
      "serialize_compact",
      [](hll_sketch& self) {
        std::ostringstream oss;
        self.serialize_compact(oss);
        return oss.str();
      })
    .define_method(
      "serialize_updatable",
      [](hll_sketch& self) {
        std::ostringstream oss;
        self.serialize_updatable(oss);
        return oss.str();
      })
    // TODO change to summary?
    .define_method(
      "to_string",
      [](hll_sketch& self) {
        return self.to_string();
      })
    .define_singleton_function(
      "deserialize",
      [](std::string& is) {
        std::istringstream iss(is);
        return hll_sketch::deserialize(iss);
      });

  Rice::define_class_under<hll_union>(m, "HllUnion")
    .define_constructor(Rice::Constructor<hll_union, int>())
    .define_method(
      "update",
      [](hll_union& self, hll_sketch& sketch) {
        self.update(sketch);
      })
    .define_method(
      "estimate",
      [](hll_union& self) {
        return self.get_estimate();
      })
    .define_method(
      "result",
      [](hll_union& self) {
        return self.get_result();
      });
}
