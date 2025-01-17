#ifndef UNIT_HPP
#define UNIT_HPP

#include "spec.hpp"
#include <type_traits>

/*
  Nouvelle définition de unit : 
  template<typename T, spec D> struct unit
  où D contient les exposants via la structure spec.
*/

template<typename T, spec D>
struct unit
{
    T value;

    // Constructeur par défaut
    constexpr unit() : value{} {}

    // Constructeur explicite
    constexpr explicit unit(const T& v) : value(v) {}
};

//-----------------------------------------------------
// Opérateur + : addition d'unités de même dimension
//-----------------------------------------------------
template<typename T, spec D>
constexpr auto operator+(
    unit<T, D> const& lhs,
    unit<T, D> const& rhs
)
{
    // Ici, D est identique, donc les exposants sont identiques
    // => pas besoin d'un static_assert car le template l'empêche
    return unit<T, D>(lhs.value + rhs.value);
}

//-----------------------------------------------------
// Opérateur - : soustraction d'unités de même dimension
//-----------------------------------------------------
template<typename T, spec D>
constexpr auto operator-(
    unit<T, D> const& lhs,
    unit<T, D> const& rhs
)
{
    return unit<T, D>(lhs.value - rhs.value);
}

//-----------------------------------------------------
// Opérateur * : multiplication => addition des exposants
//-----------------------------------------------------
template<typename T, spec D1, spec D2>
constexpr auto operator*(
    unit<T, D1> const& lhs,
    unit<T, D2> const& rhs
)
{
    constexpr spec newSpec = spec_add(D1, D2);
    return unit<T, newSpec>(lhs.value * rhs.value);
}

//-----------------------------------------------------
// Opérateur / : division => soustraction des exposants
//-----------------------------------------------------
template<typename T, spec D1, spec D2>
constexpr auto operator/(
    unit<T, D1> const& lhs,
    unit<T, D2> const& rhs
)
{
    constexpr spec newSpec = spec_sub(D1, D2);
    return unit<T, newSpec>(lhs.value / rhs.value);
}

#endif // UNIT_HPP
