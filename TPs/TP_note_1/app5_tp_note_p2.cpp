#include <iostream>
#include <utility>

/*
Le Système international d'unités (abrégé en SI), inspiré du 
système métrique, est le système d'unités le plus largement 
employé au monde.

Le Système international comporte sept unités de base, 
destinées à mesurer des grandeurs physiques indépendantes
 et possédant chacune un symbole :

  + La masse, mesurée en kilogramme (kg).
  + Le temps, mesuré en second (s).
  + La longueur, mesuré en mètre (m).
  + La température, mesurée en kelvin (K).
  + L'intensité électrique, mesurée en ampère (A).
  + La quantité de matière, mesurée en mole   (mol).
  + L'intensité lumineuse, mesurée en candela  (cd). 

A partir de ces unités de bases, il est possible de construire 
des unités dérivées. Par exemple : 

  + La frequence, exprimée en s^-1
  + La vitesse, exprimée en m.s^-1
  + L'energie, exprimée en kg.m^2.s^−2 

On remarque que ses unités dérivées sont toutes des produits de 
puissance d'unité de base.

On se propose d'implémenter un systeme permettant de calculer des
grandeurs avec des unités en utilisant le type des objets unités
pour empecher des erreurs comme kg + m.
*/

/*
  ETAPE 1 - STRUCTURE UNIT

  Ecrivez une structure unit qui prend en parametre template un type T
  et 7 entiers, chacune représentant une unité de base. Vous les
  ordonnerez de façon à suivre l'ordre de la liste donnée plus haut.

  Cette structure contient une valeur de type T.
*/

/*
  ETAPE 2 - UNITES ET UNITES DERIVEES
  En utilisant le modele ci dessous, définissez des types
  representant les grandeurs demandées.

  Indice : https://fr.wikipedia.org/wiki/Syst%C3%A8me_international_d%27unit%C3%A9s#Unit%C3%A9s_d%C3%A9riv%C3%A9es
*/

// Masse : kg
template<typename T>
using mass = unit<T,1,0,0,0,0,0,0>;

// Longeur : m
template<typename T>
using length = unit<T,0,0,1,0,0,0,0>;

// Vitesse m.s-1
template<typename T>
using speed = ;

// Force exprimée en Newton
template<typename T>
using newton = ;

// conductance électrique
template<typename T>
using siemens = ;

/*
  ETAPE 3 - OPERATIONS

  Ecrivez les operateurs +,-,* et / entre deux unités 
  du même type mais avec des unités cohérentes.

  On peut multipliez ou divisé du temps par une masse mais
  on ne peut pas ajouter des Kelvin à des metres.
*/


/*
  ETAPE 4 - SIMPLIFICATION
  A partir de C++20, n'importequelle structure constexpr peut servir
  de parametres template.

  Définissez une structure spec contenant un tableau de 7 entiers et les opérations et
  constrcuteurs que vous jugerez nécessaire et implémentez dans un deuxieme fichier la
  totalité des opérations et types précédent en partant du principe que unit devient

  template<typename T, spec D> struct unit;
*/

int main()
{
  // POUR CHAQUE ETAPE, ECRIVEZ LES TESTS QUE VOUS JUGEREZ SUFFISANT
}