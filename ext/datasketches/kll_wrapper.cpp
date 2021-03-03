#include <sstream>

#include <kll_sketch.hpp>

#include <rice/rice.hpp>

using datasketches::kll_sketch;

namespace Rice::detail
{
  template<>
  struct From_Ruby<std::vector<int>>
  {
    static std::vector<int> convert(Object x)
    {
      auto a = Rice::Array(x);
      std::vector<int> vec(a.size());
      for (long i = 0; i < a.size(); i++) {
        vec[i] = From_Ruby<int>::convert(a[i]);
      }
      return vec;
    }
  };

  template<>
  struct From_Ruby<std::vector<float>>
  {
    static std::vector<float> convert(Object x)
    {
      auto a = Rice::Array(x);
      std::vector<float> vec(a.size());
      for (long i = 0; i < a.size(); i++) {
        vec[i] = From_Ruby<float>::convert(a[i]);
      }
      return vec;
    }
  };

  template<>
  struct From_Ruby<std::vector<double>>
  {
    static std::vector<double> convert(Object x)
    {
      auto a = Rice::Array(x);
      std::vector<double> vec(a.size());
      for (long i = 0; i < a.size(); i++) {
        vec[i] = From_Ruby<double>::convert(a[i]);
      }
      return vec;
    }
  };

  template<>
  struct To_Ruby<std::vector<int>>
  {
    static Object convert(std::vector<int> const & x)
    {
      auto a = Rice::Array();
      for (size_t i = 0; i < x.size(); i++) {
        a.push(x[i]);
      }
      return a;
    }
  };

  template<>
  struct To_Ruby<std::vector<float>>
  {
    static Object convert(std::vector<float> const & x)
    {
      auto a = Rice::Array();
      for (size_t i = 0; i < x.size(); i++) {
        a.push(x[i]);
      }
      return a;
    }
  };

  template<>
  struct To_Ruby<std::vector<double>>
  {
    static Object convert(std::vector<double> const & x)
    {
      auto a = Rice::Array();
      for (size_t i = 0; i < x.size(); i++) {
        a.push(x[i]);
      }
      return a;
    }
  };
}

template<typename T>
void bind_kll_sketch(Rice::Module& m, const char* name) {
  Rice::define_class_under<kll_sketch<T>>(m, name)
    .define_constructor(Rice::Constructor<kll_sketch<T>, uint16_t>(), Rice::Arg("k")=kll_sketch<T>::DEFAULT_K)
    .define_method("empty?", &kll_sketch<T>::is_empty)
    .define_method("n", &kll_sketch<T>::get_n)
    .define_method("num_retained", &kll_sketch<T>::get_num_retained)
    .define_method("estimation_mode?", &kll_sketch<T>::is_estimation_mode)
    .define_method("min_value", &kll_sketch<T>::get_min_value)
    .define_method("max_value", &kll_sketch<T>::get_max_value)
    .define_method("quantile", &kll_sketch<T>::get_quantile)
    .define_method(
      "quantiles",
      [](kll_sketch<T>& self, Rice::Object obj) {
        if (obj.is_a(rb_cArray)) {
          auto fractions = Rice::detail::From_Ruby<std::vector<double>>::convert(obj);
          return self.get_quantiles(&fractions[0], fractions.size());
        } else {
          return self.get_quantiles(Rice::detail::From_Ruby<size_t>::convert(obj));
        }
      })
    .define_method(
      "rank",
      [](kll_sketch<T>& self, const T item) {
        return self.get_rank(item);
      })
    .define_method(
      "pmf",
      [](kll_sketch<T>& self, std::vector<T> split_points) {
        return self.get_PMF(&split_points[0], split_points.size());
      })
    .define_method(
      "cdf",
      [](kll_sketch<T>& self, std::vector<T> split_points) {
        return self.get_CDF(&split_points[0], split_points.size());
      })
    .define_method(
      "merge",
      [](kll_sketch<T>& self, const kll_sketch<T>& other) {
        self.merge(other);
      })
    .define_method(
      "update",
      [](kll_sketch<T>& self, const T item) {
        self.update(item);
      })
    .define_method(
      "serialize",
      [](kll_sketch<T>& self) {
        std::ostringstream oss;
        self.serialize(oss);
        return oss.str();
      })
    // TODO change to summary?
    .define_method(
      "to_string",
      [](kll_sketch<T>& self) {
        return self.to_string();
      })
    .define_singleton_function(
      "deserialize",
      [](std::string& is) {
        std::istringstream iss(is);
        return kll_sketch<T>::deserialize(iss);
      });
}

void init_kll(Rice::Module& m) {
  bind_kll_sketch<int>(m, "KllIntsSketch");
  bind_kll_sketch<float>(m, "KllFloatsSketch");
}
