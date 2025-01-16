#include <cassert>
#include <iostream>
#include "01_corr_matvec_impl.hpp"

int main(int, char const *[])
{
	//--------------------------------------------------------------------------
	// 1) Tests sur vec
	//--------------------------------------------------------------------------

	// Test du constructeur par défaut (valeurs 0)
	{
		vec<int, 5> v;
		for (std::size_t i = 0; i < v.size(); ++i) {
			assert(v[i] == 0 && "Default constructor should initialize to 0");
		}
	}

	// Test du constructeur avec une valeur
	{
		vec<double, 3> v(3.14);
		for (std::size_t i = 0; i < v.size(); ++i) {
			assert(v[i] == 3.14 && "Constructor with a value failed");
		}
	}

	// Test de l'accès en lecture/écriture
	{
		vec<int, 4> v;
		for (std::size_t i = 0; i < v.size(); ++i) {
			v[i] = static_cast<int>(i + 1);
		}
		for (std::size_t i = 0; i < v.size(); ++i) {
			assert(v[i] == static_cast<int>(i + 1) && "Element access error");
		}
	}

	//--------------------------------------------------------------------------
	// 2) Tests sur mat
	//--------------------------------------------------------------------------

	// Test du constructeur par défaut (valeurs 0)
	{
		mat<int, 2, 3> A;
		for (std::size_t i = 0; i < A.m(); ++i) {
			for (std::size_t j = 0; j < A.n(); ++j) {
				assert(A(i, j) == 0 && "Default constructor should initialize to 0");
			}
		}
	}

	// Test du constructeur avec une valeur
	{
		mat<float, 3, 2> A(2.5f);
		for (std::size_t i = 0; i < A.m(); ++i) {
			for (std::size_t j = 0; j < A.n(); ++j) {
				assert(A(i, j) == 2.5f && "Constructor with a value failed");
			}
		}
	}

	// Test de l'accès en lecture/écriture
	{
		mat<int, 2, 2> A;
		A(0, 0) = 1; A(0, 1) = 2;
		A(1, 0) = 3; A(1, 1) = 4;
		assert(A(0, 0) == 1 && "Element access error A(0,0)");
		assert(A(0, 1) == 2 && "Element access error A(0,1)");
		assert(A(1, 0) == 3 && "Element access error A(1,0)");
		assert(A(1, 1) == 4 && "Element access error A(1,1)");
	}

	//--------------------------------------------------------------------------
	// 3) Tests row() et col()
	//--------------------------------------------------------------------------

	{
		mat<int, 3, 2> A;
		// Remplissons la matrice A :
		// A = [ [1, 2],
		//       [3, 4],
		//       [5, 6] ]
		A(0, 0) = 1; A(0, 1) = 2;
		A(1, 0) = 3; A(1, 1) = 4;
		A(2, 0) = 5; A(2, 1) = 6;

		// row(1) => [3, 4]
		[[maybe_unused]] auto row1 = A.row(1);
		assert(row1[0] == 3 && "row(1)[0] should be 3");
		assert(row1[1] == 4 && "row(1)[1] should be 4");

		// col(1) => [2, 4, 6]
		[[maybe_unused]] auto col1 = A.col(1);
		assert(col1[0] == 2 && "col(1)[0] should be 2");
		assert(col1[1] == 4 && "col(1)[1] should be 4");
		assert(col1[2] == 6 && "col(1)[2] should be 6");
	}

	//--------------------------------------------------------------------------
	// 4) Tests dot(u, v) = produit scalaire
	//--------------------------------------------------------------------------

	{
		vec<int, 4> u;
		u[0] = 1; u[1] = 3; u[2] = -2; u[3] = 5;

		vec<int, 4> v;
		v[0] = 2; v[1] = 0; v[2] = 1;  v[3] = -1;

		[[maybe_unused]] int d = dot(u, v);
		// d = 1*2 + 3*0 + (-2)*1 + 5*(-1) = 2 + 0 -2 -5 = -5
		assert(d == -5 && "dot product calculation error");
	}

	//--------------------------------------------------------------------------
	// 5) Tests u * v = produit dyadique
	//--------------------------------------------------------------------------

	{
		vec<int, 2> u;
		u[0] = 10;
		u[1] = 20;

		vec<int, 3> v;
		v[0] = 1;
		v[1] = 2;
		v[2] = 3;

		// Dyadique => mat<int,2,3>
		// [ [u[0]*v[0], u[0]*v[1], u[0]*v[2]],
		//   [u[1]*v[0], u[1]*v[1], u[1]*v[2]] ]
		// = [ [10*1, 10*2, 10*3],
		//     [20*1, 20*2, 20*3] ]
		// = [ [10, 20, 30],
		//     [20, 40, 60] ]
		[[maybe_unused]] auto D = u * v;
		assert(D(0, 0) == 10);
		assert(D(0, 1) == 20);
		assert(D(0, 2) == 30);
		assert(D(1, 0) == 20);
		assert(D(1, 1) == 40);
		assert(D(1, 2) == 60);
	}

	//--------------------------------------------------------------------------
	// 6) Tests u * alpha et alpha * u
	//--------------------------------------------------------------------------

	{
		vec<double, 3> u;
		u[0] = 1.0; u[1] = 2.0; u[2] = -1.0;

		[[maybe_unused]] auto w1 = u * 2.0;  // => [2.0, 4.0, -2.0]
		assert(w1[0] == 2.0 && "Scalar multiplication error (u * alpha)");
		assert(w1[1] == 4.0 && "Scalar multiplication error (u * alpha)");
		assert(w1[2] == -2.0 && "Scalar multiplication error (u * alpha)");

		[[maybe_unused]] auto w2 = 0.5 * u;  // => [0.5, 1.0, -0.5]
		assert(w2[0] == 0.5 && "Scalar multiplication error (alpha * u)");
		assert(w2[1] == 1.0 && "Scalar multiplication error (alpha * u)");
		assert(w2[2] == -0.5 && "Scalar multiplication error (alpha * u)");
	}

	//--------------------------------------------------------------------------
	// 7) Tests A * x (mat-vec) => vec
	//--------------------------------------------------------------------------

	{
		// A 3x2
		mat<int, 3, 2> A;
		// A = [ [1,  2],
		//       [0,  1],
		//       [2, -1] ]
		A(0, 0) = 1;  A(0, 1) = 2;
		A(1, 0) = 0;  A(1, 1) = 1;
		A(2, 0) = 2;  A(2, 1) = -1;

		// x de taille 2
		vec<int, 2> x;
		x[0] = 3;
		x[1] = 4;

		// y = A * x => vec<int,3>
		// y[0] = 1*3 + 2*4 = 3 + 8 = 11
		// y[1] = 0*3 + 1*4 = 4
		// y[2] = 2*3 + (-1)*4 = 6 - 4 = 2
		[[maybe_unused]] auto y = A * x;
		assert(y.size() == 3 && "Matrix-vector product result should have size 3");
		assert(y[0] == 11 && "A*x [0] mismatch");
		assert(y[1] == 4  && "A*x [1] mismatch");
		assert(y[2] == 2  && "A*x [2] mismatch");
	}

	//--------------------------------------------------------------------------
	// 8) Tests x * A (vec-mat) => vec
	//--------------------------------------------------------------------------

	{
		// A 2x3
		mat<int, 2, 3> A;
		// A = [ [1,  2,  3],
		//       [-1, 4,  0] ]
		A(0, 0) = 1;   A(0, 1) = 2;  A(0, 2) = 3;
		A(1, 0) = -1;  A(1, 1) = 4;  A(1, 2) = 0;

		// x de taille 2
		vec<int, 2> x;
		x[0] = 2;
		x[1] = 1;

		// y = x * A => vec<int,3>
		// y[0] = x[0]*A(0,0) + x[1]*A(1,0) = 2*1 + 1*(-1) = 2 -1 = 1
		// y[1] = 2*2 + 1*4 = 4 + 4 = 8
		// y[2] = 2*3 + 1*0 = 6
		[[maybe_unused]] auto y = x * A;
		assert(y.size() == 3 && "Vector-matrix product result should have size 3");
		assert(y[0] == 1  && "x*A [0] mismatch");
		assert(y[1] == 8  && "x*A [1] mismatch");
		assert(y[2] == 6  && "x*A [2] mismatch");
	}

	//--------------------------------------------------------------------------
	// 9) Tests A * B (mat-mat) => mat
	//--------------------------------------------------------------------------

	{
		// A est 2x3
		mat<int, 2, 3> A;
		// A = [ [1,  2,  3],
		//       [4,  5,  6] ]
		A(0, 0) = 1;  A(0, 1) = 2;  A(0, 2) = 3;
		A(1, 0) = 4;  A(1, 1) = 5;  A(1, 2) = 6;

		// B est 3x2
		mat<int, 3, 2> B;
		// B = [ [7,  8],
		//       [9, 10],
		//       [11,12] ]
		B(0, 0) = 7;   B(0, 1) = 8;
		B(1, 0) = 9;   B(1, 1) = 10;
		B(2, 0) = 11;  B(2, 1) = 12;

		// C = A * B => 2x2
		// C(0,0) = A(0,0)*B(0,0) + A(0,1)*B(1,0) + A(0,2)*B(2,0)
		//        = 1*7 + 2*9 + 3*11 = 7 + 18 + 33 = 58
		// C(0,1) = 1*8 + 2*10 + 3*12 = 8 + 20 + 36 = 64
		// C(1,0) = 4*7 + 5*9 + 6*11 = 28 + 45 + 66 = 139
		// C(1,1) = 4*8 + 5*10 + 6*12 = 32 + 50 + 72 = 154
		auto C = A * B;

		static_assert(decltype(C)::m() == 2, "Result rows should be 2");
		static_assert(decltype(C)::n() == 2, "Result cols should be 2");
		assert(C(0,0) == 58 && "A*B [0,0] mismatch");
		assert(C(0,1) == 64 && "A*B [0,1] mismatch");
		assert(C(1,0) == 139 && "A*B [1,0] mismatch");
		assert(C(1,1) == 154 && "A*B [1,1] mismatch");
	}

	//--------------------------------------------------------------------------
	// S'il n'y a eu aucune erreur, on affiche un message final
	//--------------------------------------------------------------------------

	std::cout << "Tous les tests se sont déroulés avec succès !" << std::endl;
	return 0;
}
