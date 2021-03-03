#include <sstream>

#include <var_opt_sketch.hpp>

#include <rice/rice.hpp>

using datasketches::var_opt_sketch;

template<typename T>
void bind_vo_sketch(Rice::Module &m, const char* name) {
  Rice::define_class_under<var_opt_sketch<T>>(m, "VarOptSketch")
    .define_constructor(Rice::Constructor<var_opt_sketch<T>, uint32_t>())
    .define_method("k", &var_opt_sketch<T>::get_k)
    .define_method("n", &var_opt_sketch<T>::get_n)
    .define_method("num_samples", &var_opt_sketch<T>::get_num_samples)
    .define_method("empty?", &var_opt_sketch<T>::is_empty)
    .define_method("reset", &var_opt_sketch<T>::reset)
    .define_method(
      "samples",
      [](var_opt_sketch<T>& self) {
        auto a = Rice::Array();
        for (auto item : self) {
          auto t = Rice::Array();
          t.push(item.first);
          t.push(item.second);
          a.push(t);
        }
        return a;
      })
    .define_method(
      "update",
      [](var_opt_sketch<T>& self, const T item) {
        self.update(item);
      });
}

void init_vo(Rice::Module& m) {
  bind_vo_sketch<Rice::Object>(m, "VarOptSketch");
}
