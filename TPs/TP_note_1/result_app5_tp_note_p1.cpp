#include <iostream>
#include <cmath>
#include <tuple>
#include <algorithm>
#include <sstream>
#include <utility> // std::index_sequence, std::make_index_sequence
#include <array>
#include <cassert>


//--------------------------------------------------------------------------------------
/*
  Le but de ce TP est de mettre en place un petit système d'EXPRESSION TEMPLATES
  Les EXPRESSION TEMPLATES sont une technique d'optimisation de calcul numérique qui
  utilisent la méta-programmation pour construire une représentation légére d'une formule
  arbitraire sous la forme d'un ARBRE DE SYNTAXE ABSTRAITE.

  Une fois construit à la compilation, cet arbre devient exploitable à la compilation 
  ou à l'exécution pour effectuer des calculs de divers types. 

  Répondez au questions dans l'ordre en complétant le code.
*/
//---------------------------------------------------------------------------------------
namespace et
{
  // Q1 - Définissez un concept expr qui est valide si un type T fournit un membre T::is_expr()
  template<typename T>
  concept expr = requires( /* ???? */ )
  {
    { T::is_expr() } -> std::convertible_to<bool>;
  };

  //---------------------------------------------------------------------------------------
  /*
    Le premier élément fondamental d'un systeme d'EXPRESSION TEMPLATE est la classe de 
    terminal. Un TERMINAL représente une feuille de l'ARBRE DE SYNTAXE. Dans notre cas,
    nos terminaux sont numéroté statiquement pour représent différentes variables.

    Q2. Complétez l'implémentation de la structure template terminal ci dessous en suivant les demandes
  */
  //---------------------------------------------------------------------------------------
  template<int ID>
  struct terminal 
  {
    // Faite en sorte que terminal vérifie le concept expr
    static constexpr bool is_expr() { return true; }

    std::ostream& print(std::ostream& os) const
    {
      // Pour terminal<I>, affiche "arg<I>" et renvoit os.
      return os << "arg<" << ID << ">";
    }

    template<typename... Args>
    constexpr auto operator()(Args&&... args) const
    {
      // Construit un tuple de tout les args et renvoit le ID-eme via std::get
      // Veillez à bien repsecter le fait que args est une reference universelle
      auto tuple = std::forward_as_tuple(std::forward<Args>(args)...);
      return std::get<ID>(tuple);
    }
  };

  // Generateur de variable numérotée
  template<int ID>
  inline constexpr auto arg =  terminal<ID>{};

  // Variables _0, _& et _2 sont prédéfinies
  inline constexpr auto _0  = arg<0>;
  inline constexpr auto _1  = arg<1>;
  inline constexpr auto _2  = arg<2>;

  //---------------------------------------------------------------------------------------
  /*
    Le deuxieme élément  d'un systeme d'EXPRESSION TEMPLATE est la classe de noeud. 
    Un NODE représente un opératuer ou une fonction dans l'ARBRE DE SYNTAXE. 

    Il est définit par le type de l'OPERATION effectuée au passage du noeud et d'une
    liste variadique de ses sous-nodes.

    Q3 Complétez l'implémentation de la structure template node ci dessous en suivant les demandes
  */
  //---------------------------------------------------------------------------------------
  template<typename Op, typename... Children>
  struct node
  {
    // Faite en sorte que node vérifie le concept expr
    static constexpr bool is_expr() { return true; }

    // Construisez un node à partir d'une instande de Op et d'une liste variadique de Children
    // Ce constructeur sera constexpr
    constexpr node(Op o, Children... c)
      : op(o), children(c...) 
    {}

    // L'operateur() de node permet d'avaluer le sous arbre courant de manière 
    // récursive. Les paramètres args... représentent dans l'ordre les valeurs des
    // variables contenus dans le sous arbre.
    // Par exemple, le node {op_add, terminal<1>, termnal<0>} appelant operator()(4, 9)
    // doit renvoyer op_add(9, 4);
    // Renseignez vous sur std::apply pour vous simplifier la vie
    // Pensez qu'un node contient potentiellement d'autre node.
    template<typename... Args>
    constexpr auto operator()(Args&&... args) const
    {
      auto subresults = std::apply(
        [&](auto const&... child){
          return std::tuple{ child(std::forward<Args>(args)...)... };
        },
        children
      );

      return std::apply(op, subresults);
    }

    // Affiche un node en demandant à Op d'afficher les sous arbres
    std::ostream& print(std::ostream& os) const
    {
      auto arr = std::apply(
        [&](auto const&... child){
          return std::array<std::string, sizeof...(Children)>{
            child_to_string(child)...
          };
        },
        children
      );

      // On décompose le tableau pour que op.print recoive chaque chaîne séparément
      return std::apply(
        [&](auto const&... s) -> std::ostream& {
          return op.print(os, s...);
        },
        arr
      );
    }

    // Helper qui convertit un enfant (terminal ou node) en string
    static std::string child_to_string(auto const& sub)
    {
      std::ostringstream oss;
      sub.print(oss);
      return oss.str();
    }
    
    // Op est stockée par valeur
    // les Children... sont stockées dans un tuple
    Op op;
    std::tuple<Children...> children;
  };


  //----------------------------------------------
  /*
    add_ est un exemple de type d'operation passable à un node
    Il fournit un operator() qui effectue le calcul et une fonction
    print qui affiche le node.
  */
  //----------------------------------------------
  struct add_ 
  {
    constexpr auto operator()(auto a, auto b) const
    {
      return a + b;
    }

    std::ostream& print(std::ostream& os, auto a, auto b) const
    {
      return os << a << " + " << b ;
    }
  };
  
  // On lui associe un operator+ qui consomme des expr et renvoit le node
  template<expr L, expr R>
  constexpr auto operator+(L l, R r)
  {
    return node{add_{}, l, r};
  }

  /*
    Q4. Sur le modèle de add_, implémentez 
      - mul_ et un operator* pour la multiplication
      - abs_ et une fonction abs pour le calcul de la valeur absolue
      - fma_ et une fonction fma(a,b,c) qui calcul a*b+c
  */
  struct mul_ 
  {
    constexpr auto operator()(auto a, auto b) const
    {
      return a * b;
    }

    std::ostream& print(std::ostream& os, auto a, auto b) const
    {
      return os << a << " * " << b ;
    }
  };
  
  template<expr L, expr R>
  constexpr auto operator*(L l, R r)
  {
    return node{mul_{}, l, r};
  }

  struct abs_ 
  {
    constexpr auto operator()(auto a) const
    {
      return (a < 0) ? -a : a;
    }

    std::ostream& print(std::ostream& os, auto a) const
    {
      return os << "abs(" << a << ")" ;
    }
/*     std::ostream& print(std::ostream& os, std::array<std::string, 1> const& st) const
    {
      return os << "abs(" << st[0] << ")";
    } */
  };
  
  template<expr E>
  constexpr auto abs(E e)
  {
    return node{abs_{}, e};
  }

  struct fma_ 
  {
    constexpr auto operator()(auto a, auto b, auto c) const
    {
      return (a * b) + c;
    }

    std::ostream& print(std::ostream& os, auto a, auto b, auto c) const
    {
      return os << "(" << a << " * " << b << ")" << " + " << c;
    }
/*     std::ostream& print(std::ostream& os, std::array<std::string, 3> const& st) const
    {
      return os << "(" << st[0] << " * " << st[1] << ") + " << st[2];
    } */
  };
  
  template<expr A, expr B, expr C>
  constexpr auto fma(A a, B b, C c)
  {
    return node{fma_{}, a, b, c};
  }  
}

int main()
{
  // Q5. Le mini exemple ci dessous doit fonctionner. Complétez le avec une série de tests
  // exhaustif de tout les cas qui vous paraissent nécessaire.

  std::cout << "========== fma : " << "\n";
  constexpr auto f_1 = et::fma(et::_1, abs(et::_2),et::_0);
  f_1.print(std::cout) << "\n";

  std::cout << "f_1(1, 2, 3) = " << f_1(1, 2, 3) << "\n";
  assert(f_1(1, 2, 3) == 7);
  std::cout << "f_1(1, 2, -3) = " << f_1(1, 2, -3) << "\n" << "\n";
  assert(f_1(1, 2, -3) == 7);

  constexpr auto f_2 = et::fma(et::_1, et::_2,et::_1);
  f_2.print(std::cout) << "\n";
  std::cout << "f_2(1, 2, 3) = " << f_2(1, 2, 3) << "\n";
  assert(f_2(1, 2, 3) == 8);
  std::cout << "f_2(1, 2, -3) = " << f_2(1, 2, -3) << "\n";
  assert(f_2(1, 2, -3) == -4);


  std::cout << "\n" << "========== add : " << "\n";
  constexpr auto g = et::_0 + et::_1;
  g.print(std::cout) << "\n";

  std::cout << "g(3, 6) = " << g(3, 6) << "\n";
  assert(g(3, 6) == 9);
  std::cout << "g(-4, 3) = " << g(-4, 3) << "\n";
  assert( g(-4, 3) == -1);
  std::cout << "g(-4.2, 5.7) = " << g(-4.2, 5.7) << "\n";
  assert(std::abs(g(-4.2, 5.7) - 1.5) < 1e-12);
  std::cout << "g(-4.2, 3) = " << g(-4.2, 3) << "\n";
  assert(std::abs(g(-4.2, 3) + 1.2) < 1e-12);


  std::cout << "\n" << "========== mul : " << "\n";
  constexpr auto h = et::_0 * et::_1;
  h.print(std::cout) << "\n";

  std::cout << "h(3, 6) = " << h(3, 6) << "\n";
  assert(h(3, 6) == 18);
  std::cout << "h(-4, 7) = " << h(-4, 7) << "\n";
  assert(h(-4, 7) == -28);
  std::cout << "h(3.4, 6.5) = " << h(3.4, 6.5) << "\n";
  assert(std::abs(h(3.4, 6.5) - 22.1) < 1e-12);
  std::cout << "h(-4, 7.2) = " << h(-4, 7.2) << "\n";
  assert(std::abs(h(-4, 7.2) + 28.8) < 1e-12);

  std::cout << "\n" << "========== combinaisons : " << "\n";
  constexpr auto f_3 = et::fma(et::_0, et::_1,et::_2);
  f_3.print(std::cout) << "\n";
  h.print(std::cout) << "\n";

  std::cout << "f_3(1, 2, h(3, 5)) = " << f_3(1, 2, h(3, 5)) << "\n";
  assert(f_3(1, 2, h(3, 5)) == 17);

  return 0;
}