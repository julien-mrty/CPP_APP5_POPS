#ifndef UNITS_DERIVED_HPP
#define UNITS_DERIVED_HPP

#include "unit.hpp"

/*
  Ici, on déclare des "spec" spécifiques 
  correspondant aux unités de base et dérivées.
*/

// Ordre : M, S, L, K, I, MO, CD

// Masse (kg) -> [1,0,0,0,0,0,0]
constexpr spec massSpec    = spec({1,0,0,0,0,0,0});

// Longueur (m) -> [0,0,1,0,0,0,0]
constexpr spec lengthSpec  = spec({0,0,1,0,0,0,0});

// Vitesse (m.s^-1) -> [0,-1,1,0,0,0,0]
constexpr spec speedSpec   = spec({0,-1,1,0,0,0,0});

// Force (Newton) -> kg.m.s^-2 -> [1,-2,1,0,0,0,0]
constexpr spec newtonSpec  = spec({1,-2,1,0,0,0,0});

// Conductance (Siemens) -> kg^-1.m^-2.s^3.A^2 -> [-1,3,-2,0,2,0,0]
constexpr spec siemensSpec = spec({-1,3,-2,0,2,0,0});

//------------------------------------------------------
// Alias de types
//------------------------------------------------------
template<typename T> using mass    = unit<T, massSpec>;
template<typename T> using length  = unit<T, lengthSpec>;
template<typename T> using speed   = unit<T, speedSpec>;
template<typename T> using newton  = unit<T, newtonSpec>;
template<typename T> using siemens = unit<T, siemensSpec>;

#endif // UNITS_DERIVED_HPP
