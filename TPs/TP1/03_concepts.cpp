#include <iostream>
#include <array>
#include <vector>
#include <cstdint>
#include <cassert>
#include <concepts>

/**
  Exercice 2 - Manipulation de pixel

  On souhaite ecrire une petite bibliothÃ¨que pour traiter des opérations 
  colorimétriques sur des pixels.

  Q2.1  Implémentez le plus simplement possible: 
      - une structure grey_pixel qui contient la représentation d'un pixel
        en niveau de gris sur un entier 8 bits non signé
      - une structure rgb_pixel qui contient les trois composantes Rouge, Vert
        et Bleu d'un pixel sous la forme de trois entiers 8 bits non signés
      - une classe rgba_pixel qui contient les trois composantes Rouge, Vert
        et Bleu ainsi qu'un niveau de transparence Alpha sous forme de quatre
        réels simple précision. Cette classe devra fournir un constrcuteur qui
        vérifie (en utilisant assert) que les valeurs qui lui sont passées sont
        comprises dans l'interval [0,1]. Elle fournit de plus 4 membres permettant
        l'accés en lecture seul à ces composants : red(), green(), blue() et 
        alpha()

        Ecrivez de petits test pour vérifier leur comportements.

  Q2.2  Pour chacun des types de pixels, implémentez une fonction  
        color_mix qui prend deux pixels a et b du meme type et un
        réel simple précision ratio.

        color_mix calcul l'interpolation des deux pixels en entrée en calculant
        pour chaque composant des pixels a et b, la somme pondéré par ratio:
            a.composant * ratio + b.composant * (1-ratio)

  Q2.3  Proposez une stratégie d'adaptation et un concept pixel permettant d'unifier 
        toutes les implémentation de color_mix en une seule

  Q2.4  Implémentez les fonctions nécessaires à cette unification.

  Q2.5  Implémentez une fonction color_mix2 qui utilise le concept pixel pour
        factorisez le code de color_mix.

  Q2.6  En utilisant le concept pixel, implémenter une fonction to_grey qui convertit
        n'importe quel pixel en grey_pixel. La conversion RGB vers niveau de gris
        consiste à effectue la moyenne des composant R,G,B pondéré par le niveau 
        de Alpha. Quelle remarque avez vous sur les performances de cette fonction ?
**/

int main()
{


}