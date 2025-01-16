// SUjet APP5 2025-01-17, non noté

#include <iostream>
#include <algorithm>

// On définit une **Séquence de Type** comme un *template* variadique de classe
// dont les paramètres *templates* encode le contenu d'une liste de types.
// Ces séquences de types sont utilisées comme abstraction dans certaines 
// opérations de méta-programmation.

// Soit le code suivant définissant une telle séquence de type :

template<typename... Types>
struct type_list
{};


// Implémentez les fonctions "constexpr" suivantes permettant de manipuler 
// une instance de "type_list".


// "size" renvoie le nombre de types dans une instance de "type_list"
template<typename... Types>
constexpr auto size(type_list<Types...>)
{
  return ---;
}

// "operator+" opérateur permettant de concaténer deux "type_list"
template<typename... T1, typename... T2>
constexpr auto operator+(type_list<T1...>, type_list<T2...>)
{ 
  // Compléter ici
}

// "largest" qui renvoie la valeur maximale de la taille en octet des types 
// d'une "type_list"
template<--->
--- largest---
{
  // Compléter ici
}

// "all_of" qui renvoie "true" indiquant si tous les types de la type_list sont
// identiques au type "T" passé en template
template<--->
--- all_of(---)
{
  // Compléter ici
}

// "any_of" qui renvoie "true" si au moins un type "T" donné est présent dans 
// une instance de "type_list"
template<--->
--- any_of(---)
{
  // Compléter ici
}

// "none_of" qui renvoie "true" si aucun type de la "type_list" ne correspond
// au type "T" donné
template<--->
--- none_of(---)
{
  // Compléter ici
}

// "find" qui renvoie la première position (=l'index) d'un type "T" donné dans
// une "type_list". Si "T" n'est pas présent, la taille de la "type_list" est renvoyée
template<--->
--- find(---)
{
  // Compléter ici
}

// "largest_index" pareil que "find", sauf qu'on veut la dernière position
// (=index) et non la première
template<--->
--- largest_index(---)
{
  // Compléter ici
}


// "unroll" qui renvoie une "type_list" constituée de toutes 
// les combinaisons possibles entre une "type_list<T1>" et une "type_list<T2...>""
template<--->
--- unroll(---)
{ 
  // Compléter ici
}

// "cartesian_product" qui prend deux "type_list" et en fait le produit cartésien
template<--->
--- cartesian_product(---)
{ 
  // Compléter ici
}

// "to_tuple" qui convertir un "type_list" en "std::tuple"
template<--->
--- to_tuple(---)
{ 
  // Compléter ici
}

// Une structure "reducer" qui implémente un opérateur % faisant la réduction
// d'une valeur de type U et Z avec une fonction de type F.
template<typename Z, typename F>
struct reducer
{
  // Compléter ici
};

// Une fonction "reduce" qui applique une fonction de type F pour réduire tous
// les éléments d'une "type_list", initialisée à la valeur z valeur de type Z.
template<typename... T, typename F, typename Z>
constexpr auto reduce(type_list<T...>, F f, Z z)
{ 
  // Compléter ici
}

// Nous n'avons pas eu le temps d'implémenter des tests exhaustifs
// Implémentez les vôtres !
int main()
{
  constexpr type_list<int,float,char[19], double, char, void**> x;

  std::cout << reduce(x, []<typename T>(type_list<T> t, std::size_t i) 
  { return sizeof(T) + i ;}, 0ULL) << "\n";
}