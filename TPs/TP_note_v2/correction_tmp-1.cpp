#include <iostream>
#include <cmath>
#include <tuple>
#include <algorithm>
#include <array>
#include <sstream>   // <-- Needed for std::ostringstream

namespace et
{
  // Q1 - Définissez un concept expr qui est valide si un type T fournit un membre T::is_expr()
  template<typename T>
  concept expr = requires()
  {
    { T::is_expr() } -> std::convertible_to<bool>;
  };

  //---------------------------------------------------------------------------------------
  // TERMINAL
  //---------------------------------------------------------------------------------------
  template<int ID>
  struct terminal 
  {
    static constexpr bool is_expr() { return true; }

    std::ostream& print(std::ostream& os) const
    {
      // Pour terminal<I>, affiche "arg<I>"
      return os << "arg<" << ID << ">";
    }

    template<typename... Args>
    constexpr auto operator()(Args&&... args) const
    {
      // Construire un tuple de tous les args, puis en extraire l'ID-ème
      auto tuple = std::forward_as_tuple(std::forward<Args>(args)...);
      return std::get<ID>(tuple);
    }
  };

  // Générateur de variable numérotée
  template<int ID>
  inline constexpr auto arg = terminal<ID>{};

  // Variables _0, _1, _2 pré-définies
  inline constexpr auto _0 = arg<0>;
  inline constexpr auto _1 = arg<1>;
  inline constexpr auto _2 = arg<2>;

  //---------------------------------------------------------------------------------------
  // NODE
  //---------------------------------------------------------------------------------------
  template<typename Op, typename... Children>
  struct node
  {
    static constexpr bool is_expr() { return true; }

    // Constructeur constexpr
    constexpr node(Op o, Children... c)
      : op(o), children(c...) {}

    // Évaluation récursive
    template<typename... Args>
    constexpr auto operator()(Args&&... args) const
    {
      // Pour chacun des enfants, on appelle child(std::forward<Args>(args)...)
      auto subresults = std::apply(
        [&](auto const&... child)
        {
          return std::array{ child(std::forward<Args>(args)...)... };
        },
        children
      );
      // On applique ensuite op(...) sur le std::array
      return std::apply(op, subresults);
    }

    // Affichage d'un node
    std::ostream& print(std::ostream& os) const
    {
      // Convertir chacun des enfants en une std::string
      auto arr = std::apply(
        [&](auto const&... ch)
        {
          return std::array<std::string, sizeof...(Children)>{
            child_to_string(ch)...
          };
        },
        children
      );
      // Maintenant, on “déplie” arr pour appeler op.print(os, arr[0], arr[1], ...)
      return std::apply(
        [&](auto const&... str) {
          return op.print(os, str...);
        },
        arr
      );
    }

    static std::string child_to_string(auto const& sub)
    {
      std::ostringstream oss;
      sub.print(oss);
      return oss.str();
    }

    Op op;
    std::tuple<Children...> children;
  };

  //----------------------------------------------
  // add_
  //----------------------------------------------
  struct add_
  {
    constexpr auto operator()(auto a, auto b) const
    {
      return a + b;
    }

    // Pour afficher, on reçoit 2 sous-chaines
    std::ostream& print(std::ostream& os, std::string const& a, std::string const& b) const
    {
      return os << "(" << a << " + " << b << ")";
    }
  };

  // operator+ qui construit un node<add_, L, R>
  template<expr L, expr R>
  constexpr auto operator+(L l, R r)
  {
    return node{add_{}, l, r};
  }

  //----------------------------------------------
  // mul_
  //----------------------------------------------
  struct mul_
  {
    constexpr auto operator()(auto a, auto b) const
    {
      return a * b;
    }

    std::ostream& print(std::ostream& os, std::string const& a, std::string const& b) const
    {
      return os << "(" << a << " * " << b << ")";
    }
  };

  template<expr L, expr R>
  constexpr auto operator*(L l, R r)
  {
    return node{mul_{}, l, r};
  }

  //----------------------------------------------
  // abs_
  //----------------------------------------------
  struct abs_
  {
    constexpr auto operator()(auto a) const
    {
      return (a >= 0) ? a : -a;
    }

    std::ostream& print(std::ostream& os, std::string const& x) const
    {
      return os << "abs(" << x << ")";
    }
  };

  // Évitez de nommer la variable comme le template-parameter
  template<expr T>
  constexpr auto abs(T e)
  {
    return node{abs_{}, e};
  }

  //----------------------------------------------
  // fma_
  //----------------------------------------------
  struct fma_
  {
    // a * b + c
    constexpr auto operator()(auto a, auto b, auto c) const
    {
      return a * b + c;
    }

    std::ostream& print(std::ostream& os,
                        std::string const& a,
                        std::string const& b,
                        std::string const& c) const
    {
      return os << "(" << a << " * " << b << " + " << c << ")";
    }
  };

  template<expr L, expr M, expr R>
  constexpr auto fma(L l, M m, R r)
  {
    return node{fma_{}, l, m, r};
  }

} // namespace et

int main()
{
  // Exemple de test FMA avec abs, etc.
  constexpr auto f = et::fma(et::_1, et::abs(et::_2), et::_0);

  // Affiche la structure symbolique
  f.print(std::cout) << "\n"; 

  // Évalue l'expression FMA( _1, abs(_2), _0 ) = (arg<1> * abs(arg<2>) + arg<0>)
  std::cout << "f(1, 2, 3) = " << f(1, 2, 3) << "\n";
  std::cout << "f(3, -4, 10) = " << f(3, -4, 10) << "\n";

  // Autres tests
  // 1) Addition simple
  constexpr auto add_expr = et::_0 + et::_1;
  add_expr.print(std::cout) << " => " << add_expr(3, 7) << "\n";

  // 2) Multiplication simple
  constexpr auto mul_expr = et::_1 * et::_0;
  mul_expr.print(std::cout) << " => " << mul_expr(10, 5) << "\n";

  // 3) Composition : ( (x + y) * abs(z) )
  constexpr auto combo_expr = (et::_0 + et::_1) * et::abs(et::_2);
  combo_expr.print(std::cout) << " => " << combo_expr(1, 2, -3) << "\n";

  return 0;
}
