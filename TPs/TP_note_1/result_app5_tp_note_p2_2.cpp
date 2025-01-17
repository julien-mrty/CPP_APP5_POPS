#include <iostream>
#include <utility>
#include <array>


struct spec {
  int dims[7];
};

template<typename T, spec D> 
struct unit {
  T value;
};

constexpr spec mass_spec = {1, 0, 0, 0, 0, 0, 0};
template<typename T>
using unit_mass = unit<T, mass_spec>;

constexpr spec time_spec = {0, 1, 0, 0, 0, 0, 0};
template<typename T>
using unit_time = unit<T, time_spec>;

constexpr spec length_spec = {0, 0, 1, 0, 0, 0, 0};
template<typename T>
using unit_length = unit<T, length_spec>;

constexpr spec speed_spec = {0, -1, 1, 0, 0, 0, 0};
template<typename T>
using unit_speed = unit<T, speed_spec>;

constexpr spec newton_spec = {1, -2, 1, 0, 0, 0, 0};
template<typename T>
using unit_newton = unit<T, newton_spec>;

constexpr spec siemens_spec = {-1, 3, -2, 0, 2, 0, 0};
template<typename T>
using unit_siemens = unit<T, siemens_spec>;


template<
  typename T1, spec spec1,
  typename T2, spec spec2
>
auto operator+(
  unit<T1, spec1> const& lhs,
  unit<T2, spec2> const& rhs
)
{
  static_assert(spec1.dims[0]==spec2.dims[0] && 
                spec1.dims[1]==spec2.dims[1] && 
                spec1.dims[2]==spec2.dims[2] && 
                spec1.dims[3]==spec2.dims[3] && 
                spec1.dims[4]==spec2.dims[4] && 
                spec1.dims[5]==spec2.dims[5] && 
                spec1.dims[6]==spec2.dims[6],
                "Incompatible units for operator+");
  using ResultType = decltype(lhs.value + rhs.value);
  return unit<ResultType, spec1>{ lhs.value + rhs.value };
}

template<
  typename T1, spec spec1,
  typename T2, spec spec2
>
auto operator-(
  unit<T1, spec1> const& lhs,
  unit<T2, spec2> const& rhs
)
{
  static_assert(spec1.dims[0]==spec2.dims[0] && 
                spec1.dims[1]==spec2.dims[1] && 
                spec1.dims[2]==spec2.dims[2] && 
                spec1.dims[3]==spec2.dims[3] && 
                spec1.dims[4]==spec2.dims[4] && 
                spec1.dims[5]==spec2.dims[5] && 
                spec1.dims[6]==spec2.dims[6],
                "Incompatible units for operator+");
  using ResultType = decltype(lhs.value - rhs.value);
  return unit<ResultType, spec1>{ lhs.value - rhs.value };
}


constexpr spec spec_add(const spec& s1, const spec& s2)
{
  spec result = {};
  for (int i = 0; i < 7; ++i) {
      result.dims[i] = s1.dims[i] + s2.dims[i];
  }

  return result; 
}

template<
  typename T1, spec spec1,
  typename T2, spec spec2
>
auto operator*(
  unit<T1, spec1> const& lhs,
  unit<T2, spec2> const& rhs
)
{
  constexpr spec final_spec = spec_add(spec1, spec2);
  using ResultType = decltype(lhs.value * rhs.value);
  return unit<ResultType, final_spec>{ lhs.value * rhs.value };
}


constexpr spec spec_sub(const spec& s1, const spec& s2)
{
  spec result = {};
  for (int i = 0; i < 7; ++i) {
      result.dims[i] = s1.dims[i] - s2.dims[i];
  }

  return result; 
}

template<
  typename T1, spec spec1,
  typename T2, spec spec2
>
auto operator/(
  unit<T1, spec1> const& lhs,
  unit<T2, spec2> const& rhs
)
{
  constexpr spec final_spec = spec_sub(spec1, spec2);
  using ResultType = decltype(lhs.value / rhs.value);
  return unit<ResultType, final_spec>{ lhs.value / rhs.value };
}




int main() {

  return 0;
}