#ifndef SPEC_HPP
#define SPEC_HPP

#include <array>
#include <iostream>

/*
  Structure spec (partie 4) :
  - Contient un std::array<int,7> représentant les exposants
    dans l'ordre : M, S, L, K, I, MO, CD
  - Permet de faire des additions / soustractions d'exposants
    pour gérer la multiplication / division d'unités.
*/

struct spec
{
    // Ordre des indices : [M, S, L, K, I, MO, CD]
    std::array<int, 7> dims;

    // Constructeur constexpr prenant un std::array
    constexpr spec(std::array<int, 7> arr) : dims{arr} {}

    // Constructeur par défaut : tout à 0
    constexpr spec() : dims{0,0,0,0,0,0,0} {}

    // Opérateur d'égalité
    constexpr bool operator==(const spec& other) const {
        for(int i = 0; i < 7; ++i) {
            if(dims[i] != other.dims[i])
                return false;
        }
        return true;
    }

    constexpr bool operator!=(const spec& other) const {
        return !(*this == other);
    }
};

// Addition de deux spec (pour la multiplication d'unités)
constexpr spec spec_add(const spec& s1, const spec& s2)
{
    std::array<int, 7> result{};
    for(int i = 0; i < 7; ++i) {
        result[i] = s1.dims[i] + s2.dims[i];
    }
    return spec(result);
}

// Soustraction de deux spec (pour la division d'unités)
constexpr spec spec_sub(const spec& s1, const spec& s2)
{
    std::array<int, 7> result{};
    for(int i = 0; i < 7; ++i) {
        result[i] = s1.dims[i] - s2.dims[i];
    }
    return spec(result);
}

#endif // SPEC_HPP
