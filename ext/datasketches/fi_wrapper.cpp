#include <sstream>

#include <frequent_items_sketch.hpp>

#include <rice/rice.hpp>

template<typename T>
void bind_fi_sketch(Rice::Module& m, const char* name) {
  Rice::define_class_under<datasketches::frequent_items_sketch<T>>(m, name)
    .define_constructor(Rice::Constructor<datasketches::frequent_items_sketch<T>, uint16_t>())
    .define_method("empty?", &datasketches::frequent_items_sketch<T>::is_empty)
    .define_method("num_active_items", &datasketches::frequent_items_sketch<T>::get_num_active_items)
    .define_method("total_weight", &datasketches::frequent_items_sketch<T>::get_total_weight)
    .define_method("estimate", &datasketches::frequent_items_sketch<T>::get_estimate)
    .define_method("lower_bound", &datasketches::frequent_items_sketch<T>::get_lower_bound)
    .define_method("upper_bound", &datasketches::frequent_items_sketch<T>::get_upper_bound)
    .define_method("maximum_error", &datasketches::frequent_items_sketch<T>::get_maximum_error)
    .define_method(
      "update",
      [](datasketches::frequent_items_sketch<T>& self, const T item) {
        self.update(item);
      })
    .define_method(
      "serialize",
      [](datasketches::frequent_items_sketch<T>& self) {
        std::ostringstream oss;
        self.serialize(oss);
        return oss.str();
      })
    // TODO change to summary?
    .define_method(
      "to_string",
      [](datasketches::frequent_items_sketch<T>& self) {
        return self.to_string();
      })
    .define_singleton_function(
      "deserialize",
      // TODO figure out segfault
      [](std::string is) {
        std::istringstream iss(is);
        return datasketches::frequent_items_sketch<T>::deserialize(iss);
      });
}

void init_fi(Rice::Module& m) {
  bind_fi_sketch<std::string>(m, "FrequentStringsSketch");
}
