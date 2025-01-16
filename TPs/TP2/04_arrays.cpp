#include <cassert>
#include <cstdint>
#include <iostream>
#include <memory>
#include <stdexcept>

template <typename T, std::size_t N> class small_array {
  T data[N];

public:
  small_array() = default;
  small_array(small_array const &) = default;
  small_array(small_array &&) = default;
  ~small_array() = default;
  small_array &operator=(small_array const &) = default;
  small_array &operator=(small_array &&) = default;
  T &operator[](std::size_t i) noexcept {
    assert(i < N);
    return data[i];
  }
  T const &operator[](std::size_t i) const noexcept {
    assert(i < N);
    return data[i];
  }
  T &at(std::size_t i) {
    if (i >= N)
      throw std::runtime_error("out-of-bound access");
    return data[i];
  }
  T const &at(std::size_t i) const {
    if (i >= N)
      throw std::runtime_error("out-of-bound access");
    return data[i];
  }
};

template <typename T, std::size_t N> class large_array {
  std::unique_ptr<small_array<T, N>> data;

public:
  large_array() : data(new small_array<T, N>) {}
  ~large_array() = default;
  large_array(large_array const &t) : data(new small_array<T, N>(*t.data)) {}
  large_array(large_array &&) = default;
  /*
  large_array &operator=(large_array const &t) {
    *data = *t.data; // can be interrupted during the copy
    return *this;
  }
  */
  large_array &operator=(large_array const &t) {
    large_array u = t; // extra allocation
    data.swap(u.data); // cannot be interrupted
    return *this;
  }
  large_array &operator=(large_array &&) = default;
  T &operator[](std::size_t i) noexcept {
    assert(i < N);
    return (*data)[i];
  }
  T const &operator[](std::size_t i) const noexcept {
    assert(i < N);
    return (*data)[i];
  }
  T &at(std::size_t i) {
    if (i >= N)
      throw std::runtime_error("out-of-bound access");
    return (*data)[i];
  }
  T const &at(std::size_t i) const {
    if (i >= N)
      throw std::runtime_error("out-of-bound access");
    return (*data)[i];
  }
  void swap(large_array &t) { data.swap(t.data); }
};

template <bool, class T, class F> struct if_;
template <class T, class F> struct if_<true, T, F> { typedef T type; };
template <class T, class F> struct if_<false, T, F> { typedef F type; };

template <typename T, std::size_t N>
using my_array = typename if_<(sizeof(small_array<T, N>) > 16),
                              large_array<T, N>, small_array<T, N>>::type;

int main() {
  small_array<int, 4> t;
  t[2] = 42;
  small_array<int, 4> const u = t;
  for (std::size_t i = 0; i < 4; ++i) {
    std::cout << '[' << i << "] = " << u.at(i) << '\n';
  }
  large_array<int, 1000 * 1000 * 10> v;
  v[2] = 42;
  large_array<int, 1000 * 1000 * 10> w(v), x;
  x = w;
  x.swap(w);
  for (std::size_t i = 0; i < 4; ++i) {
    std::cout << '[' << i << "] = " << x.at(i) << '\n';
  }
  my_array<int, 4> z;
  z[2] = 42;
}
