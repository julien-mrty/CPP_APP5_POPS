#include <iostream>

/*
  Implémentez le traits sanitize_type qui retire tous les qualificateurs const et 
  les références de toute forme d'un type donné.
*/
template <typename T> struct sanitize_type 
{
  using type = T;
};

template <typename T> struct sanitize_type<T const> 
{
  using type = typename sanitize_type<T>::type;
};

template <typename T> struct sanitize_type<T &> 
{
  using type = typename sanitize_type<T>::type;
};

template <typename T> struct sanitize_type<T &&> 
{
  using type = typename sanitize_type<T>::type;
};

/*
  Implémentez le traits remove_pointers qui retire tous les pointeurs d'un type donné. 
  Attention aux éventuels const !
*/
template <typename T> struct remove_pointers 
{
  using type = T;
};

template <typename T> struct remove_pointers<T *> 
{
  using type = typename remove_pointers<T>::type;
};

template <typename T> struct remove_pointers<T const *> 
{
  using type = typename remove_pointers<T>::type;
};

/*
  Implémentez le traits volume qui calcule à la compilation la quantité d'éléments 
  contenu dans un type tableau de n'importe quel nombre de dimensions. On considérera
  que le volume d'un type non-tableau est égal à 1.
*/
template <typename T> struct volume 
{
  static const std::size_t value = 1;
};

template <typename T, std::size_t N> struct volume<T[N]> 
{
  static const std::size_t value = N * volume<T>::value;
};

int main()
{
  // Tests repris depuis ceux de Julie D, merci !

  static_assert(std::is_same_v<sanitize_type<const int>::type,int>);
  static_assert(std::is_same_v<sanitize_type<const int&>::type,int>);
  static_assert(std::is_same_v<sanitize_type<int>::type,int>);
  static_assert(std::is_same_v<sanitize_type<int&>::type,int>);
  static_assert(std::is_same_v<sanitize_type<int&&>::type,int>);

  static_assert(std::is_same_v<remove_pointers<int*>::type,int>);
  static_assert(std::is_same_v<remove_pointers<const int*>::type,int>);

  double a[26];
  double b[23][4];
  static_assert(volume<decltype(a)>::value == 26);
  static_assert(volume<double>::value == 1);
  static_assert(volume<decltype(b)>::value == 23*4);
}