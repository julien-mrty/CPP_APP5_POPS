#include <iostream>
#include <type_traits>

/*
  Implémentez le traits sanitize_type qui retire tous les qualificateurs const et 
  les références de toute forme d'un type donné.
*/
template <typename T> struct sanitize_type 
{
    using type = typename std::remove_const<typename std::remove_reference<T>::type>::type;
};


/*
  Implémentez le traits remove_pointers qui retire tous les pointeurs d'un type donné. 
  Attention aux éventuels const !
*/
// Primary template: Handles the base type when no pointer remains.
template <typename T> struct remove_pointers 
{
  using type = T;
};
// Specialization: Removes one pointer layer and recurses.
template <typename T> struct remove_pointers<T*> 
{
  using type = typename remove_pointers<T>::type;
};
template <typename T> struct remove_pointers<T const *> 
{
  using type = typename remove_pointers<T>::type;
};


/*
  Implémentez le traits volume qui calcule à la compilation la quantité d'éléments 
  contenu dans un type tableau de n'importe quel nombre de dimensions. On considérera
  que le volume d'un type non-tableau est égal à 1.
*/
// Base case: Non-array types, volume is 1
template <typename T> struct volume {
    static const std::size_t value = 1;
};
// Specialization for array types
template <typename T, std::size_t N> struct volume<T[N]> {
    static const std::size_t value = N * volume<T>::value;
};


int main()
{
    // ECRIVEZ DES TESTS AVEC STATIC_ASSERT

    // Test 1: const int&
    static_assert(std::is_same<sanitize_type<const int&>::type, int>::value, "Test 1 failed");
    // Test 2: volatile int&&
    static_assert(std::is_same<sanitize_type<volatile int&&>::type, volatile int>::value, "Test 2 failed");
    // Test 3: const volatile int
    static_assert(std::is_same<sanitize_type<const volatile int>::type, volatile int>::value, "Test 3 failed");
    // Test 4: int
    static_assert(std::is_same<sanitize_type<int>::type, int>::value, "Test 4 failed");
    std::cout << "sanitize_type : OK" << std::endl;

    // Test 1: int*
    static_assert(std::is_same<remove_pointers<int*>::type, int>::value, "Test 1 failed");
    // Test 2: int**
    static_assert(std::is_same<remove_pointers<int**>::type, int>::value, "Test 2 failed");
    // Test 3: const int*
    static_assert(std::is_same<remove_pointers<const int*>::type, int>::value, "Test 3 failed");
    // Test 4: int**
    static_assert(std::is_same<remove_pointers<int***>::type, int>::value, "Test 4 failed");
    std::cout << "remove_pointers : OK" << std::endl;

    // Test 1: Volume of a simple int (non-array)
    static_assert(volume<int>::value == 1, "Test 1 failed");
    // Test 2: Volume of a 1D array of size 5
    static_assert(volume<int[5]>::value == 5, "Test 2 failed");
    // Test 3: Volume of a 2D array (3x4)
    static_assert(volume<int[3][4]>::value == 12, "Test 3 failed");
    // Test 4: Volume of a 3D array (2x3x4)
    static_assert(volume<int[2][3][4]>::value == 24, "Test 4 failed");
    std::cout << "volume : OK" << std::endl;

    return 0;
}