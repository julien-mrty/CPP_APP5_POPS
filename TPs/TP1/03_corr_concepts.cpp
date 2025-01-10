#include <iostream>
#include <array>
#include <vector>
#include <cstdint>
#include <cassert>
#include <concepts>


/**
  Exercice 2 - Manipulation de pixel

  On souhaite ecrire une petite bibliothèque pour traiter des opérations 
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

// Q2.1
struct grey_pixel
{
  std::uint8_t level;
};

struct rgb_pixel
{
  std::uint8_t r,g,b;
};

class rgba_pixel
{
  public:
  rgba_pixel(float rr, float gg, float bb, float aa)
            : r(rr), g(gg), b(bb), a(aa)
  {
    assert(r >= 0.f && r<=1.f);
    assert(g >= 0.f && g<=1.f);
    assert(b >= 0.f && b<=1.f);
    assert(a >= 0.f && b<=1.f);
  }

  float red()   const { return r; }
  float green() const { return g; }
  float blue()  const { return b; }
  float alpha() const { return a; }

  private:
  float r,g,b,a;
};

// Q2.2
grey_pixel color_mix(grey_pixel a, grey_pixel b, float ratio)
{
  std::uint8_t v = a.level*ratio + b.level*(1.f-ratio);
  return grey_pixel{ v };
}

rgb_pixel color_mix(rgb_pixel a, rgb_pixel b, float ratio)
{
  std::uint8_t vr = a.r*ratio + b.r*(1.f-ratio);
  std::uint8_t vg = a.g*ratio + b.g*(1.f-ratio);
  std::uint8_t vb = a.b*ratio + b.b*(1.f-ratio);
  return rgb_pixel{ vr, vg, vb };
}

rgba_pixel color_mix(rgba_pixel a, rgba_pixel b, float ratio)
{
  float vr = a.red()*ratio + b.red()*(1.f-ratio);
  float vg = a.green()*ratio + b.green()*(1.f-ratio);
  float vb = a.blue()*ratio + b.blue()*(1.f-ratio);
  float va = a.alpha()*ratio + b.alpha()*(1.f-ratio);
  return rgba_pixel{ vr, vg, vb, va };
}

// Q2.3
template<typename T>
concept pixel = requires(T const& p, int i)
{
  { red(p)   };
  { green(p) };
  { blue(p)  };
  { alpha(p) };
};

// Q2.4
auto red(grey_pixel const& a)     { return a.level; }
auto green(grey_pixel const& a)   { return a.level; }
auto blue(grey_pixel const& a)    { return a.level; }
auto alpha(grey_pixel const& a)   { return 1.f;     }

auto red(rgb_pixel const& a)     { return a.r; }
auto green(rgb_pixel const& a)   { return a.g; }
auto blue(rgb_pixel const& a)    { return a.b; }
auto alpha(rgb_pixel const& a)   { return 1.f; }

auto red(rgba_pixel const& a)     { return a.red();   }
auto green(rgba_pixel const& a)   { return a.green(); }
auto blue(rgba_pixel const& a)    { return a.blue();  }
auto alpha(rgba_pixel const& a)   { return a.alpha(); }

// Q2.5
template<pixel P1, pixel P2>
rgba_pixel color_mix(P1 a, P2 b, float ratio)
{
  float vr = red(a)*ratio   + red(b)*(1.f-ratio);
  float vg = green(a)*ratio + green(b)*(1.f-ratio);
  float vb = blue(a)*ratio  + blue(b)*(1.f-ratio);
  float va = alpha(a)*ratio + alpha(b)*(1.f-ratio);
  return rgba_pixel{ vr, vg, vb, va };
}

// Q2.6
// Reponse: Ca semble non-optimal pour P == grey_pixel
template<pixel P> grey_pixel to_grey(P a)
{
  std::uint8_t v = (red(a) + green(a) + blue(a))*alpha(a)/3;
  return grey_pixel{ v };
}

int main()
{

// Il manque les tests unitaires, je mettrai peut-être à jour ce fichier plus tard, désolé !

}