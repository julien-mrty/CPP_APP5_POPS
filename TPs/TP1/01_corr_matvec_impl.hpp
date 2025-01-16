#pragma once

#include <array>
#include <cstddef>

// Type vec(tor)

// Implementez vec, qui est un vecteur (au sens mathematique, pas un std::vector
// de taille dynamique). Les donnees sont stockees dans un std::array, et la
// structure fournit un operator[] (const et non-const), un constructeur par
// valeur pour initialiser toutes les valeurs (ce que ne fait pas std::array),
// et un membre size().

template <typename T, std::size_t N> struct vec {
	// Storage
	std::array<T, N> data;

	//--------------------------------------------------------------------------
	// Constructors
	//--------------------------------------------------------------------------
	
	// Default constructor: zero-initialize
	constexpr vec() : data{} {}
	
	// "Constructor by value" that fills all elements with the same value
	constexpr explicit vec(const T& value) {
		data.fill(value);
	}

	//--------------------------------------------------------------------------
	// Element access
	//--------------------------------------------------------------------------
	
	// Non-const subscript operator
	constexpr T& operator[](std::size_t i) {
		return data[i];
	}
	
	// Const subscript operator
	constexpr const T& operator[](std::size_t i) const {
		return data[i];
	}

	//--------------------------------------------------------------------------
	// Size
	//--------------------------------------------------------------------------
	
	static constexpr std::size_t size() {
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
	// We store M*N elements contiguously
	std::array<T, M * N> data;

	//--------------------------------------------------------------------------
	// Constructors
	//--------------------------------------------------------------------------
	
	// Default constructor: zero-initialize
	constexpr mat() : data{} {}
	
	// Fill all entries with a single value
	constexpr explicit mat(const T& value) {
		data.fill(value);
	}

	//--------------------------------------------------------------------------
	// Dimensions
	//--------------------------------------------------------------------------
	
	static constexpr std::size_t m() { return M; }
	static constexpr std::size_t n() { return N; }

	//--------------------------------------------------------------------------
	// Element access
	//--------------------------------------------------------------------------
	
	// Non-const element access (i = row, j = column)
	constexpr T& operator()(std::size_t i, std::size_t j) {
		return data[i * N + j];
	}

	// Const element access
	constexpr const T& operator()(std::size_t i, std::size_t j) const {
		return data[i * N + j];
	}

	//--------------------------------------------------------------------------
	// Extract a full column as a vec<T,M>
	//--------------------------------------------------------------------------
	
	constexpr vec<T, M> col(std::size_t j) const {
		vec<T, M> result;
		for (std::size_t i = 0; i < M; ++i) {
			result[i] = (*this)(i, j);
		}
		return result;
	}

	//--------------------------------------------------------------------------
	// Extract a full row as a vec<T,N>
	//--------------------------------------------------------------------------
	
	constexpr vec<T, N> row(std::size_t i) const {
		vec<T, N> result;
		for (std::size_t j = 0; j < N; ++j) {
			result[j] = (*this)(i, j);
		}
		return result;
	}
};

// Implementez dot(vec, vec) qui effectue un produit scalaire
// entre deux vecteurs

template <typename T, std::size_t N> constexpr T dot(const vec<T, N>& u, const vec<T, N>& v){
	T result {};
	for (std::size_t i = 0; i < N; ++i){
		result += u[i] * v[i];
	}
	return result;
}

// Implementez un operator* entre deux objets de type vec, qui effectue un
// produit dyadique (https://fr.wikipedia.org/wiki/Produit_dyadique)
// Genere une matrice m dans laquelle chaque valeur aux coordonnees (i,j)
// vaut u[i] * v[j].

template <typename T, std::size_t M, std::size_t N> 
constexpr mat<T, M, N> operator*(const vec<T, M>& u, const vec<T, N>& v){
	mat<T, M, N> result;
	for (std::size_t i = 0; i < M; ++i){
		for (std::size_t j = 0; j < N; ++j){
			result(i, j) = u[i] * v[j];
		}
	}
	return result;
}

// Implementez un operator* qui multiplie les valeurs de u par n
// (en faisant une copie ou transfert du vector)

template <typename T, std::size_t N>
constexpr vec<T, N> operator*(const vec<T, N>& u, const T& n) {
	vec<T, N> result;
	for (std::size_t i = 0; i < N; ++i) {
		result[i] = u[i] * n;
	}
	return result;
}

// Produit inverse (facultatif)

template <typename T, std::size_t N>
constexpr vec<T, N> operator*(const T& alpha, const vec<T, N>& u) {
	return u * alpha;
}

// Implementez un produit matrice/vecteur via l'operator*

template <typename T, std::size_t M, std::size_t N>
constexpr vec<T, M> operator*(const mat<T, M, N>& A, const vec<T, N>& x) {
	vec<T, M> result;
	for (std::size_t i = 0; i < M; ++i) {
		T sum{};
		for (std::size_t j = 0; j < N; ++j) {
			sum += A(i, j) * x[j];
		}
		result[i] = sum;
	}
	return result;
}

// Implementez un produit vecteur/matrice via l'operator*

template <typename T, std::size_t M, std::size_t N>
constexpr vec<T, N> operator*(const vec<T, M>& x, const mat<T, M, N>& A) {
	vec<T, N> result;
	for (std::size_t j = 0; j < N; ++j) {
		T sum{};
		for (std::size_t i = 0; i < M; ++i) {
			sum += x[i] * A(i, j);
		}
		result[j] = sum;
	}
	return result;
}

// Implementez un produit matrice/matrice via l'operator*

template <typename T, std::size_t M, std::size_t N, std::size_t P>
constexpr mat<T, M, P> operator*(const mat<T, M, N>& A, const mat<T, N, P>& B) {
	mat<T, M, P> result;
	for (std::size_t i = 0; i < M; ++i) {
		for (std::size_t j = 0; j < P; ++j) {
			T sum{};
			for (std::size_t k = 0; k < N; ++k) {
				sum += A(i, k) * B(k, j);
			}
			result(i, j) = sum;
		}
	}
	return result;
}