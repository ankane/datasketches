#include <sstream>

#include <theta_sketch.hpp>
#include <theta_union.hpp>
#include <theta_intersection.hpp>
#include <theta_a_not_b.hpp>

#include <rice/Constructor.hpp>
#include <rice/Module.hpp>

using datasketches::theta_sketch;
using datasketches::update_theta_sketch;
using datasketches::compact_theta_sketch;
using datasketches::theta_union;
using datasketches::theta_intersection;
using datasketches::theta_a_not_b;

using Rice::Arg;

void init_theta(Rice::Module& m) {
  Rice::define_class_under<theta_sketch>(m, "ThetaSketch")
    .define_method("empty?", &theta_sketch::is_empty)
    .define_method("estimate", &theta_sketch::get_estimate)
    .define_method("lower_bound", &theta_sketch::get_lower_bound)
    .define_method("upper_bound", &theta_sketch::get_upper_bound)
    .define_method(
      "serialize",
      *[](theta_sketch& self) {
        std::ostringstream oss;
        self.serialize(oss);
        return oss.str();
      });

  Rice::define_class_under<update_theta_sketch, theta_sketch>(m, "UpdateThetaSketch")
    .define_singleton_method(
      "new",
      *[]() {
        update_theta_sketch::builder builder;
        return builder.build();
      })
    .define_method("compact", &update_theta_sketch::compact)
    .define_method(
      "update",
      *[](update_theta_sketch& self, Rice::Object datum) {
        if (FIXNUM_P(datum.value())) {
          return self.update(from_ruby<int64_t>(datum));
        } else if (datum.is_a(rb_cNumeric)) {
          return self.update(from_ruby<double>(datum));
        } else {
          return self.update(datum.to_s().str());
        }
      })
    .define_method(
      "estimate",
      *[](update_theta_sketch& self) {
        return self.get_estimate();
      })
    .define_singleton_method(
      "deserialize",
      *[](std::string& is) {
        std::istringstream iss(is);
        return update_theta_sketch::deserialize(iss);
      });

  Rice::define_class_under<compact_theta_sketch, theta_sketch>(m, "CompactThetaSketch")
    .define_singleton_method(
      "deserialize",
      *[](std::string& is) {
        std::istringstream iss(is);
        return compact_theta_sketch::deserialize(iss);
      });

  Rice::define_class_under<theta_union>(m, "ThetaUnion")
    .define_singleton_method(
      "new",
      *[]() {
        theta_union::builder builder;
        return builder.build();
      })
    .define_method("update", &theta_union::update)
    .define_method("result", &theta_union::get_result);

  Rice::define_class_under<theta_intersection>(m, "ThetaIntersection")
    .define_constructor(Rice::Constructor<theta_intersection>())
    .define_method("update", &theta_intersection::update)
    .define_method("result", &theta_intersection::get_result)
    .define_method("result?", &theta_intersection::has_result);

  Rice::define_class_under<theta_a_not_b>(m, "ThetaANotB")
    .define_constructor(Rice::Constructor<theta_a_not_b>())
    .define_method("compute", &theta_a_not_b::compute, (Arg("a"), Arg("b"), Arg("ordered") = true));
}
