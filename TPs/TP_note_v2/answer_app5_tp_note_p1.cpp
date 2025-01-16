#include <iostream>
#include <tuple>
#include <array>
#include <sstream>
#include <cmath>
#include <utility> // std::index_sequence, std::make_index_sequence

namespace et
{
  //------------------------------------------------------------------------------
  // Q1) Le concept expr : on vérifie que T::is_expr() existe et est convertible en bool
  //------------------------------------------------------------------------------
  template<typename T>
  concept expr = requires {
    { T::is_expr() } -> std::convertible_to<bool>;
  };

  //------------------------------------------------------------------------------
  // Q2) La structure terminal<ID>
  //------------------------------------------------------------------------------
  template<int ID>
  struct terminal 
  {
    // Pour satisfaire le concept expr :
    static constexpr bool is_expr() { return true; }

    // Affichage
    std::ostream& print(std::ostream& os) const
    {
      // Exemple : "arg<0>" si ID=0
      return os << "arg<" << ID << ">";
    }

    // Opérateur de calcul : on prend un pack d'arguments (parfait forwarding),
    // on met le tout dans un tuple, puis on renvoie la valeur ID-ième.
    template<typename... Args>
    constexpr auto operator()(Args&&... args) const
    {
      auto t = std::forward_as_tuple(std::forward<Args>(args)...);
      return std::get<ID>(t);
    }
  };

  // Générateur de variables (arg<0>, arg<1>, etc.)
  template<int ID>
  inline constexpr terminal<ID> arg{};

  // Quelques variables prédéfinies (terminaux usuels)
  inline constexpr auto _0 = arg<0>;
  inline constexpr auto _1 = arg<1>;
  inline constexpr auto _2 = arg<2>;

  //------------------------------------------------------------------------------
  // Q3) La structure node<Op, Children...>
  //
  // stocke : 
  //    - un Op (par valeur)
  //    - un tuple<Children...> pour représenter récursivement l’arbre
  //------------------------------------------------------------------------------
  template<typename Op, typename... Children>
  struct node
  {
    // Pour satisfaire le concept expr :
    static constexpr bool is_expr() { return true; }

    // Constructeur constexpr qui prend un Op et des Children...
    constexpr node(Op o, Children... c)
      : op(o), children(c...) 
    {}

    // Évaluation : on appelle récursivement operator()(args...) sur chaque enfant
    // puis on appelle Op::operator() sur le tableau/tuple de résultats.
    template<typename... Args>
    constexpr auto operator()(Args&&... as) const
    {
      // Récupère un array de tous les calculs des sous-enfants :
      // children = tuple<Child1, Child2, ...>
      // On mappe => child1(as...), child2(as...)...
      auto subresults = std::apply(
        [&](auto const&... ch){
          return std::array{ ch(std::forward<Args>(as)...)... };
        },
        children
      );
      // Ensuite on applique op(...) sur ce tableau de résultats
      // => on utilise std::apply(op, subresults)
      return std::apply(op, subresults);
    }

    // Affichage : on va générer la représentation de chaque sous-enfant,
    // puis laisser l'Op les imprimer comme il veut.
    std::ostream& print(std::ostream& os) const
    {
      // Pour chaque sous-enfant, on capture sa représentation texte :
      auto arr = std::apply(
        [&](auto const&... ch){
          return std::array<std::string, sizeof...(Children)>{
            child_to_string(ch)...
          };
        },
        children
      );
      // Puis on appelle op.print en lui passant ce tableau :
      return op.print(os, arr);
    }

    // Petit helper qui convertit un enfant (terminal ou node) en string
    static std::string child_to_string(auto const& sub)
    {
      std::ostringstream oss;
      sub.print(oss);
      return oss.str();
    }

    // Stockage
    Op op;
    std::tuple<Children...> children;
  };

  //------------------------------------------------------------------------------
  // Un helper pour imprimer un node ou terminal : usage cout << expr
  //------------------------------------------------------------------------------
  template<expr E>
  std::ostream& operator<<(std::ostream& os, E const& e)
  {
    return e.print(os);
  }

  //------------------------------------------------------------------------------
  // Un opérateur de base : add_
  //------------------------------------------------------------------------------
  struct add_ 
  {
    // operator()(a, b) => a + b
    constexpr auto operator()(auto a, auto b) const
    {
      return a + b;
    }
    // Affiche un + entre deux sous-expressions
    // On suppose 2 children => array de size 2
    std::ostream& print(std::ostream& os, std::array<std::string, 2> const& st) const
    {
      return os << "(" << st[0] << " + " << st[1] << ")";
    }
  };
  
  // Opérateur binaire + : construit un node<add_, L, R>
  template<expr L, expr R>
  constexpr auto operator+(L l, R r)
  {
    return node{add_{}, l, r};
  }

  //------------------------------------------------------------------------------
  // Q4) Sur le modèle de add_ : mul_, abs_, fma_
  //------------------------------------------------------------------------------

  //--- (1) Multiplication

  struct mul_
  {
    constexpr auto operator()(auto a, auto b) const
    {
      return a * b;
    }
    std::ostream& print(std::ostream& os, std::array<std::string, 2> const& st) const
    {
      return os << "(" << st[0] << " * " << st[1] << ")";
    }
  };

  // operator*
  template<expr L, expr R>
  constexpr auto operator*(L l, R r)
  {
    return node{mul_{}, l, r};
  }

  //--- (2) Valeur absolue (unaire)

  struct abs_
  {
    constexpr auto operator()(auto x) const
    {
      using std::abs; // pour gérer cas int, float, etc.
      return abs(x);
    }
    // array de 1 sous-expression (car unaire)
    std::ostream& print(std::ostream& os, std::array<std::string, 1> const& st) const
    {
      return os << "abs(" << st[0] << ")";
    }
  };

  // fonction abs(e)
  template<expr E>
  constexpr auto abs(E e)
  {
    return node{abs_{}, e};
  }

  //--- (3) FMA (a*b + c) => fonction ternaire

  struct fma_
  {
    constexpr auto operator()(auto a, auto b, auto c) const
    {
      return a * b + c;
    }
    // On suppose 3 sous-expressions => on imprime (a*b)+c
    std::ostream& print(std::ostream& os, std::array<std::string, 3> const& st) const
    {
      return os << "(" << st[0] << " * " << st[1] << ") + " << st[2];
    }
  };

  // fonction fma(e1, e2, e3)
  template<expr A, expr B, expr C>
  constexpr auto fma(A a, B b, C c)
  {
    return node{fma_{}, a, b, c};
  }

} // end namespace et


//--------------------------------------------------------------------------------------
// Exemple d'utilisation minimal + tests
//--------------------------------------------------------------------------------------
int main()
{
  // Q5) Exemple : f = fma(_1, abs(_2), _0);
  // => calcul : _1 * abs(_2) + _0
  constexpr auto f = et::fma(et::_1, abs(et::_2),et::_0);

  // Affichage de l'expression symbolique
  std::cout << "Expression = ";
  f.print(std::cout) << "\n";

  // Test sur quelques valeurs
  // f( x0, x1, x2 ) = x1 * abs(x2) + x0
  // => arguments dans l'ordre : (x0, x1, x2)
  // Ici, f(1, 2, -3) => 2 * abs(-3) + 1 = 2*3 + 1 = 7
  std::cout << "f(1,2,-3) = " << f(1,2,-3) << "\n";
  std::cout << "f(2,5,10) = " << f(2,5,10) << "\n";

  // Autres petits exemples :
  // 1) _0 + _1 => x0 + x1
  constexpr auto g = et::_0 + et::_1;
  std::cout << "g(10, 3) = " << g(10, 3) << "\n"; // => 13

  // 2) _2 * 4 => on peut faire: _2 * terminal immuable, mais
  //    ici on n'a pas fait un terminal "constant" explicit, on pourrait
  //    le remplacer par un code direct (ou on ferait un node constant).
  //    Pour la démo, on reste sur _2...
  //    Par exemple, h = fma(_2, _2, _0) => x2*x2 + x0
  constexpr auto h = et::fma(et::_2, et::_2, et::_0);
  std::cout << "h(5, 99, 3) = " << h(5, 99, 3) << "\n"; // => 3 + 99*99 ?

  return 0;
}
