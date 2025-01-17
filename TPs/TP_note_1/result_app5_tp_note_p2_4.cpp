#include <iostream>
#include <utility>
#include <cassert>   
#include "units_derived.hpp"


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
    //=== 1) Test basique : Addition de deux masses ===
    {
        mass<double> m1(5.0);
        mass<double> m2(2.3);
        auto m3 = m1 + m2;   // Doit être une mass<double>
        
        // Vérifions la dimension : m3 est un unit<double, massSpec>
        // => On peut comparer dimensionnellement, ou plus simplement
        //    vérifier la valeur attendue (7.3).
        assert(m3.value == 7.3);
    }

    //=== 2) Test basique : Soustraction de deux masses ===
    {
        mass<double> m1(10.0);
        mass<double> m2(1.5);
        auto m3 = m1 - m2;   // 8.5 kg
        assert(m3.value == 8.5);
    }

    //=== 3) Test incohérent : Addition de masse et longueur (DOIT échouer à la compilation) ===
    /*
        Si vous décommentez la ligne ci-dessous, 
        la compilation doit échouer (pas d’opérateur+ valide).
        
        length<double> L(5.0);
        mass<double> m(2.0);
        auto fail = L + m; // Devrait ne pas compiler
    */

    //=== 4) Multiplication force × longueur => Joule ===
    //    force (N) = kg.m.s^-2
    //    longueur (m) => multiplions => kg.m^2.s^-2 => joule
    {
        newton<double> F(10.0);
        length<double> L(3.0);
        auto FL = F * L; 
        // FL est un unit<double, [1, -2, 2, 0, 0, 0, 0]>
        // On s’attend à la valeur 10.0 × 3.0 = 30.0
        assert(FL.value == 30.0);
    }

    //=== 5) Division force / masse => accélération (m.s^-2) ===
    {
        newton<double> F(20.0);
        mass<double> m(5.0);
        auto accel = F / m; 
        // => dimension : [1,-2,1] / [1,0,0] = [0,-2,1]
        // c.-à-d. m.s^-2
        assert(accel.value == 4.0); // 20 / 5
    }

    //=== 6) Test d’une autre grandeur (conductance, siemens) ===
    {
        siemens<double> cond(0.5);
        assert(cond.value == 0.5);

        // On peut le multiplier ou diviser par un scalaire (dimensionless),
        // mais attention, un simple double n’est pas un 'unit' dimensionless.
        // On n’a pas forcément défini d’unité 'dimensionless' => c’est un choix d’implémentation.
    }

    //=== 7) Test de multiplication ou division avec d’autres combinaisons ===
    //    Par exemple, masse × longueur => dimension [1,0,1].
    //    On n’a pas d’alias « kg.m », mais on peut l’utiliser pour une démo.
    {
        mass<double> m(2.0);
        length<double> L(5.0);
        auto x = m * L; // dimensionnellement : [1,0,1]
        assert(x.value == 10.0);
    }

    //=== 8) Vérifier que la compilation échoue pour des additions incohérentes ===
    /*
        speed<double> v(3.0);
        auto fail2 = m + v; // Doit ne PAS compiler : masses + vitesse
    */

    // Si on arrive ici, tous les asserts sont passés
     // Exemples d'utilisation
    mass<double> m1(5.0);       // 5 kg
    mass<double> m2(2.3);       // 2.3 kg
    length<double> L(10.0);     // 10 m
    speed<double> v(3.0);       // 3 m/s
    newton<double> F(20.0);     // 20 N
    siemens<double> cond(0.12); // 0.12 S

    // Addition entre deux masses
    auto m3 = m1 + m2;
    std::cout << "m1 + m2 = " << m3.value << " kg\n";

    // Multiplication Force × longueur => Joule (kg.m^2.s^-2)
    auto FL = F * L;
    std::cout << "F * L = " << FL.value << " (kg.m^2.s^-2)\n";

    // Division Force / masse => accélération (m.s^-2)
    auto accel = F / m1;
    std::cout << "F / m1 = " << accel.value << " (m.s^-2)\n";
    std::cout << "Tous les tests se sont bien déroulés !" << std::endl;
    return 0;
}
