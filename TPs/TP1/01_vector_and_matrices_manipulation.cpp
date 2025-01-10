// #pragma once

#include <iostream>
#include <array>
#include <cstddef>

// Type vec(tor)

// Implementez vec, qui est un vecteur (au sens mathematique, pas un std::vector
// de taille dynamique). Les donnees sont stockees dans un std::array, et la
// structure fournit un operator[] (const et non-const), un constructeur par
// valeur pour initialiser toutes les valeurs (ce que ne fait pas std::array),
// et un membre size().

template <typename T, std::size_t N> struct vec {
    // ...Compléter ici...
    // Les données sont stockées dans un std::array
    std::array<T, N> data;

    // Constructeur par défaut
    vec() : data{} {}

    // Constructeur pour initialiser toutes les valeurs
    explicit vec(const T& value) {
        data.fill(value);
    }

    // Accès en lecture/écriture avec operator[]
    T& operator[](std::size_t index) {
        return data[index];
    }

    // Accès en lecture avec operator[] (const)
    const T& operator[](std::size_t index) const {
        return data[index];
    }

    // Méthode size()
    constexpr std::size_t size() const {
        return N;
    }
};

// Sur le meme modele, implementez mat qui est un type matrice.
// Elle doit fournir un constructeur par valeur pour initialiser le contenu,
// des membres m() et n() pour acceder aux dimensions (respectivement hauteur
// et largeur), un operator()(std::size_t, std::size_t) (const et non-const)
// pour l'acces aux elements, ainsi que deux methodes col(std::size_t) et
// row(std::size_t), qui permettent d'extraire respectivement une colonne ou une
// ligne de la matrice.

template <typename T, std::size_t M, std::size_t N> struct mat {
    // ...Compléter ici...
    // Les données sont stockées dans un std::array de vecteurs (lignes)
    std::array<std::array<T, N>, M> data;

    // Constructeur par défaut
    mat() : data{} {}

    // Constructeur pour initialiser toutes les valeurs
    explicit mat(const T& value) {
        for (std::size_t i = 0; i < M; i++) {
            data[i].fill(value);
        }
    }

    constexpr std::size_t m() const {
        return M;
    }

    constexpr std::size_t n() const {
        return N;
    }

    T& operator()(std::size_t idx_m, std::size_t idx_n) {
        return data[idx_m][idx_n]; // Ligne ; Colonne
    }

    const T& operator()(std::size_t idx_m, std::size_t idx_n) const {
        return data[idx_m][idx_n]; // Ligne ; Colonne
    }

    auto col(std::size_t c_idx) const {
        std::array<T, M> column;

        for (std::size_t i = 0; i < M; i++) {
            column[i] = data[i][c_idx];
        }

        return column;
    }

    auto row(std::size_t r_idx) const {
        std::array<T, N> row;

        for (std::size_t i = 0; i < N; i++) {
            row[i] = data[r_idx][i];
        }

        return row;
    }
};

// Implementez dot(vec, vec) qui effectue un produit scalaire
// entre deux vecteurs

// ...Compléter ici...
template <typename T, std::size_t N>
T dot(const vec<T, N>& v1, const vec<T, N>& v2) {
    T result = T(); // Initialisation à la valeur par défaut de T (0 pour les types numériques)
    for (std::size_t i = 0; i < N; ++i) {
        result += v1[i] * v2[i];
    }
    return result;
}


// Implementez un operator* entre deux objets de type vec, qui effectue un
// produit dyadique (https://fr.wikipedia.org/wiki/Produit_dyadique)
// Genere une matrice m dans laquelle chaque valeur aux coordonnees (i,j)
// vaut u[i] * v[j].
// ...Compléter ici...
template <typename T, std::size_t N, std::size_t M>
mat<T, N, M> operator*(const vec<T, N>& u, const vec<T, M>& v) {
    mat<T, N, M> result;
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < M; ++j) {
            result(i, j) = u[i] * v[j];
        }
    }
    return result;
}


// Implementez un operator* qui multiplie les valeurs de u par n
// (en faisant une copie ou transfert du vector)
// ...Compléter ici...
template <typename T, std::size_t N>
vec<T, N> operator*(const vec<T, N>& u, const T& scalar) {
    vec<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result[i] = u[i] * scalar;
    }
    return result;
}

template <typename T, std::size_t N>
vec<T, N> operator*(const T& scalar, const vec<T, N>& u) {
    return u * scalar; // Utilise la commutativité
}


// Implementez un produit matrice/vecteur via l'operator*
// ...Compléter ici...
template <typename T, std::size_t M, std::size_t N>
vec<T, M> operator*(const mat<T, M, N>& matrix, const vec<T, N>& v) {
    vec<T, M> result;
    for (std::size_t i = 0; i < M; ++i) {
        result[i] = 0;
        for (std::size_t j = 0; j < N; ++j) {
            result[i] += matrix(i, j) * v[j];
        }
    }
    return result;
}


// Implementez un produit vecteur/matrice via l'operator*
// ...Compléter ici...
template <typename T, std::size_t M, std::size_t N>
vec<T, N> operator*(const vec<T, M>& u, const mat<T, M, N>& matrix) {
    vec<T, N> result;
    for (std::size_t j = 0; j < N; ++j) {
        result[j] = 0;
        for (std::size_t i = 0; i < M; ++i) {
            result[j] += u[i] * matrix(i, j);
        }
    }
    return result;
}


// Implementez un produit matrice/matrice via l'operator*
// ...Compléter ici...
template <typename T, std::size_t M, std::size_t N, std::size_t P>
mat<T, M, P> operator*(const mat<T, M, N>& A, const mat<T, N, P>& B) {
    mat<T, M, P> result;
    for (std::size_t i = 0; i < M; ++i) {
        for (std::size_t j = 0; j < P; ++j) {
            result(i, j) = 0;
            for (std::size_t k = 0; k < N; ++k) {
                result(i, j) += A(i, k) * B(k, j);
            }
        }
    }
    return result;
}


int main() {
    // Vector
    std::cout << "Vector" << std::endl;

    vec<int, 3> v1(2); // Initialise le vecteur v1 avec les valeurs [2, 2, 2]
    vec<int, 3> v2(3); // Initialise le vecteur v2 avec les valeurs [3, 3, 3]
    for (std::size_t i = 0; i < v1.size(); ++i) {
        std::cout << v1[i] << " ";
    }
    std::cout << std::endl;

    v1[2] = 10; // Modifie un élément
    for (std::size_t i = 0; i < v1.size(); ++i) {
        std::cout << v1[i] << " ";
    }

    int result = dot(v1, v2);
    std::cout << std::endl << "Produit scalaire : " << result << std::endl;

    vec<int, 3> u(1); // [1, 1, 1]
    vec<int, 3> v(2); // [2, 2, 2]

    // Produit dyadique
    auto dyadic = u * v;
    std::cout << "Produit dyadique : " << dyadic(0, 0) << std::endl;

    // Vecteur * scalaire
    auto scaled = u * 5;
    std::cout << "Vecteur * scalaire : " << scaled[0] << std::endl;

    // Produit matrice/vecteur
    mat<int, 3, 3> matrix(2);
    auto mv_result = matrix * v;
    std::cout << "Produit matrice/vecteur : " << mv_result[0] << std::endl;

    // Produit vecteur/matrice
    auto vm_result = u * matrix;
    std::cout << "Produit vecteur/matrice : " << vm_result[0] << std::endl;

    // Produit matrice/matrice
    mat<int, 3, 3> B(3);
    auto mm_result = matrix * B;
    std::cout << "Produit matrice/matrice : " << mm_result(0, 0) << std::endl;

    std::cout << std::endl << std::endl;

    // Matrix
    std::cout << "Matrix" << std::endl;

    const std::size_t n_lines = 3;
    const std::size_t n_col = 3;
    mat<int, n_lines, n_col> matrix_2(0);
    std::cout << "M: " << matrix_2.m() << ", N: " << matrix_2.n() << "\n";

    // Extraction d'une ligne
    auto row = matrix_2.row(1);
    for (const auto& val : row) {
        std::cout << val << " ";
    }
    std::cout << "\n";

    // Extraction d'une colonne
    auto column = matrix_2.col(2);
    for (const auto& val : column) {
        std::cout << val << " ";
    }
    std::cout << "\n";

    return 0;
}