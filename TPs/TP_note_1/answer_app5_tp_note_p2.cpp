#include <iostream>
#include <utility>

/**************************************************/
/* AJOUTS À INSÉRER APRÈS LE CODE INITIAL CI-DESSUS*/
/**************************************************/

// 1) Définition de la structure unit (ETAPE 1)
// --------------------------------------------
// Cette structure encode :
//   - un type T pour stocker la valeur
//   - sept exposants entiers dans l'ordre : (kg, s, m, K, A, mol, cd)
// --------------------------------------------
template<
    typename T,
    int EXP_M,   // masse (kg)
    int EXP_S,   // temps (s)
    int EXP_L,   // longueur (m)
    int EXP_K,   // température (K)
    int EXP_I,   // intensité électrique (A)
    int EXP_MO,  // quantité de matière (mol)
    int EXP_CD   // intensité lumineuse (cd)
>
struct unit
{
    T value;
};

// 2) Définition des types dérivés pour illustrer quelques unités de base
//    et dérivées (ETAPE 2)
// ----------------------------------------------------------------------

// Masse : kg
template<typename T>
using mass = unit<T, /*kg=*/1, /*s=*/0, /*m=*/0, /*K=*/0, /*A=*/0, /*mol=*/0, /*cd=*/0>;

// Temps : s
// (non demandé explicitement, mais utile pour tests)
template<typename T>
using time_ = unit<T, /*kg=*/0, /*s=*/1, /*m=*/0, /*K=*/0, /*A=*/0, /*mol=*/0, /*cd=*/0>;

// Longueur : m
template<typename T>
using length = unit<T, /*kg=*/0, /*s=*/0, /*m=*/1, /*K=*/0, /*A=*/0, /*mol=*/0, /*cd=*/0>;

// Vitesse : m.s^-1
// => exponents : masse=0, temps=-1, longueur=1, K=0, A=0, mol=0, cd=0
template<typename T>
using speed = unit<T, 0, -1, 1, 0, 0, 0, 0>;

// Force : newton (N) => kg·m·s^-2
// => exponents : masse=1, temps=-2, longueur=1, K=0, A=0, mol=0, cd=0
template<typename T>
using newton = unit<T, 1, -2, 1, 0, 0, 0, 0>;

// Conductance électrique : siemens (S) => Ω^-1
// => Ω (ohm) = kg·m^2·s^-3·A^-2, donc siemens = kg^-1·m^-2·s^3·A^2
template<typename T>
using siemens = unit<T, -1, 3, -2, 0, 2, 0, 0>;

// (On pourrait en définir beaucoup d’autres si besoin…)

// 3) Implémentation des opérateurs (ETAPE 3)
// -------------------------------------------
// Règles :
//   - + et - : on n’autorise l’opération que si les exposants sont *identiques*
//   - * : on additionne les exposants
//   - / : on soustrait les exposants
//
// Nota : on utilise ici un static_assert pour contraindre la compatibilité
//        dimensionnelle lorsque nécessaire.

// --- AJOUT / SOUSTRACTION (même dimension) ---
template<
    typename T1, int M1, int S1, int L1, int K1, int I1, int MO1, int CD1,
    typename T2, int M2, int S2, int L2, int K2, int I2, int MO2, int CD2
>
auto operator+(
    unit<T1, M1, S1, L1, K1, I1, MO1, CD1> const& lhs,
    unit<T2, M2, S2, L2, K2, I2, MO2, CD2> const& rhs
)
{
    static_assert(M1==M2 && S1==S2 && L1==L2 && K1==K2 && I1==I2 && MO1==MO2 && CD1==CD2,
                  "Incompatible units for operator+");
    using ResultType = decltype(lhs.value + rhs.value);
    return unit<ResultType, M1, S1, L1, K1, I1, MO1, CD1>{ lhs.value + rhs.value };
}

template<
    typename T1, int M1, int S1, int L1, int K1, int I1, int MO1, int CD1,
    typename T2, int M2, int S2, int L2, int K2, int I2, int MO2, int CD2
>
auto operator-(
    unit<T1, M1, S1, L1, K1, I1, MO1, CD1> const& lhs,
    unit<T2, M2, S2, L2, K2, I2, MO2, CD2> const& rhs
)
{
    static_assert(M1==M2 && S1==S2 && L1==L2 && K1==K2 && I1==I2 && MO1==MO2 && CD1==CD2,
                  "Incompatible units for operator-");
    using ResultType = decltype(lhs.value - rhs.value);
    return unit<ResultType, M1, S1, L1, K1, I1, MO1, CD1>{ lhs.value - rhs.value };
}

// --- MULTIPLICATION : on additionne les exposants ---
template<
    typename T1, int M1, int S1, int L1, int K1, int I1, int MO1, int CD1,
    typename T2, int M2, int S2, int L2, int K2, int I2, int MO2, int CD2
>
auto operator*(
    unit<T1, M1, S1, L1, K1, I1, MO1, CD1> const& lhs,
    unit<T2, M2, S2, L2, K2, I2, MO2, CD2> const& rhs
)
{
    using ResultType = decltype(lhs.value * rhs.value);
    return unit<
        ResultType,
        (M1 + M2),
        (S1 + S2),
        (L1 + L2),
        (K1 + K2),
        (I1 + I2),
        (MO1 + MO2),
        (CD1 + CD2)
    >{ lhs.value * rhs.value };
}

// --- DIVISION : on soustrait les exposants ---
template<
    typename T1, int M1, int S1, int L1, int K1, int I1, int MO1, int CD1,
    typename T2, int M2, int S2, int L2, int K2, int I2, int MO2, int CD2
>
auto operator/(
    unit<T1, M1, S1, L1, K1, I1, MO1, CD1> const& lhs,
    unit<T2, M2, S2, L2, K2, I2, MO2, CD2> const& rhs
)
{
    using ResultType = decltype(lhs.value / rhs.value);
    return unit<
        ResultType,
        (M1 - M2),
        (S1 - S2),
        (L1 - L2),
        (K1 - K2),
        (I1 - I2),
        (MO1 - MO2),
        (CD1 - CD2)
    >{ lhs.value / rhs.value };
}

// 4) Simplification avec une structure spec (ETAPE 4)
// ----------------------------------------------------
// Idée : on définit un 'spec' qui contient les 7 exposants dans un tableau.
// En C++20, on pourrait le passer directement en paramètre d’un template.
// Pour illustrer, on laisse une esquisse. L'exemple ci-dessous n'est pas
// nécessairement relié à l'opérateur existant :
//
//   struct spec {
//       int dims[7];
//       // ... éventuels constructeurs ou opérations ...
//   };
//
//   template<typename T, spec D>
//   struct unit {
//       T value;
//   };
//
//   // => On pourrait alors implémenter tout ce qui précède en se basant sur
//   //    un "spec" plutôt que sur 7 int distincts.
//

// -------------------------------------------------------------------
// Tests dans le main() pour démontrer le fonctionnement minimal
// -------------------------------------------------------------------
int main()
{
    std::cout << "\n=== Tests du système d'unités ===\n\n";

    mass<double> m{10.0};         // 10 kg
    length<double> L{5.0};        // 5 m
    time_<double> t{2.0};         // 2 s
    speed<double> v{1.0};         // 1 m/s
    newton<double> F{20.0};       // 20 N
    siemens<double> cond{0.5};    // 0.5 S

    // Vérifions qu'on ne puisse pas additionner kg et m (devrait échouer si décommenté)
    // auto x = m + L; // --> static_assert fail

    // 1) Addition de masses
    auto sum_mass = m + mass<double>{2.0}; // kg + kg
    std::cout << "sum_mass = " << sum_mass.value << " (kg)\n"; // 12 kg

    // 2) Multiplication d'une longueur par une masse => kg·m
    //    exponents = (1,0,0,0,0,0,0)+(0,0,1,0,0,0,0) => (1,0,1,0,0,0,0)
    auto mass_length = m * L;
    std::cout << "mass_length = " << mass_length.value << " (kg·m)\n"; // 50 kg·m

    // 3) Division : newton / mass = (kg·m·s^-2)/(kg) = m·s^-2
    //    => exponents = (1,-2,1,0,0,0,0) - (1,0,0,0,0,0,0) => (0,-2,1,0,0,0,0)
    auto accel = F / m; // => 2.0 m/s^2
    std::cout << "accel = " << accel.value << " (m·s^-2)\n";

    // 4) Exemples : on peut faire m / t => (kg) / (s) = kg·s^-1
    auto flow = m / t;
    std::cout << "flow = " << flow.value << " (kg·s^-1)\n";

    // 5) Juste un affichage "check" sur la conductance (ne fait rien de spécial ici)
    std::cout << "cond = " << cond.value << " (S)\n";

    std::cout << "\n=== Fin des tests ===\n";
    return 0;
}
