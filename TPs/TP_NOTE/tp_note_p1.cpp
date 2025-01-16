#include <iostream>
#include <cmath>
#include <tuple>
#include <algorithm>
#include <sstream>   // Pour std::ostringstream dans l'affichage des noeuds
#include <string>    // Pour std::string

//--------------------------------------------------------------------------------------
/*
  Le but de ce TP est de mettre en place un petit système d'EXPRESSION TEMPLATES
  Les EXPRESSION TEMPLATES sont une technique d'optimisation de calcul numérique qui
  utilisent la méta-programmation pour construire une représentation légère d'une formule
  arbitraire sous la forme d'un ARBRE DE SYNTAXE ABSTRAITE (AST).

  Une fois construit à la compilation, cet arbre devient exploitable à la compilation 
  ou à l'exécution pour effectuer des calculs de divers types. 

  Répondez aux questions dans l'ordre en complétant le code.
*/
//---------------------------------------------------------------------------------------

namespace et
{
  //---------------------------------------------------------------------------------------
  // Q1 - DÉFINIR UN CONCEPT expr
  // Le concept 'expr' est valide si un type T fournit un membre T::is_expr.
  // En pratique, on vérifie simplement l'existence de 'typename T::is_expr;'
  //---------------------------------------------------------------------------------------
  template<typename T>
  concept expr = requires 
  {
    typename T::is_expr;  // On attend que le type T contienne un type interne nommé is_expr
  };

  //---------------------------------------------------------------------------------------
  /*
    Q2. TERMINAL
    Le premier élément fondamental d'un système d'EXPRESSION TEMPLATE est la classe de
    terminal. Un TERMINAL représente une "feuille" dans l'ARBRE DE SYNTAXE. Ici, chaque
    terminal correspond à une variable, identifiée par un ID entier. 

    - On veut faire en sorte que 'terminal<ID>' satisfasse le concept expr.
    - La méthode print(...) doit afficher "arg<ID>" dans le flux.
    - L'operator()(Args&&... args) doit renvoyer la (ID)-ième valeur parmi les paramètres
      passés (stockés dans un std::tuple).
  */
  //---------------------------------------------------------------------------------------
  template<int ID>
  struct terminal
  {
    // On définit un type interne is_expr pour que terminal<ID> respecte le concept expr.
    using is_expr = void;  

    // Affichage : on imprime "arg<ID>"
    std::ostream& print(std::ostream& os) const
    {
      return os << "arg<" << ID << ">";
    }

    // Operator() : récupère la (ID)-ième valeur des arguments passés (via un std::tuple)
    template<typename... Args>
    constexpr auto operator()(Args&&... args) const
    {
      // On crée un tuple (références universelles), puis on renvoie l'élément ID.
      auto t = std::forward_as_tuple(args...);
      return std::get<ID>(t);
    }
  };

  // Générateur de variables numérotées (terminal<ID>)
  template<int ID>
  inline constexpr auto arg = terminal<ID>{};

  // Variables pré-définies : _0, _1, _2
  inline constexpr auto _0 = arg<0>;
  inline constexpr auto _1 = arg<1>;
  inline constexpr auto _2 = arg<2>;

  //---------------------------------------------------------------------------------------
  /*
    Q3. NODE
    Le deuxième élément d'un système d'EXPRESSION TEMPLATE est la classe de noeud (node).
    Un NODE représente un opérateur ou une fonction, et contient :
      - Un type d'opération (ex. addition, multiplication, etc.) : Op
      - Une liste variadique d'enfants (sous-expressions) : Children...

    L'operator() du node doit :
      - évaluer récursivement les enfants
      - appeler l'opérateur défini par Op (ex. a + b, ou a*b, etc.)

    La méthode print doit faire appel à Op::print(...) en lui passant les "versions texte"
    de ses sous-enfants (eux-mêmes des expr qui ont une méthode print).
  */
  //---------------------------------------------------------------------------------------
  template<typename Op, typename... Children>
  struct node
  {
    // Pour respecter le concept expr, on déclare un type is_expr.
    using is_expr = void;

    // Le node stocke l'opérateur et ses enfants dans un tuple
    Op op;
    std::tuple<Children...> children;

    // Constructeur constexpr qui initialise les membres
    constexpr node(Op o, Children... c)
      : op{o}, children{c...}
    {
    }

    // L'opérateur () évalue le noeud :
    //  - on évalue récursivement chaque child(...) 
    //  - on applique op(...) sur les résultats
    template<typename... Args>
    constexpr auto operator()(Args&&... args) const
    {
      return apply_impl(std::index_sequence_for<Children...>{}, std::forward<Args>(args)...);
    }

    // Fonction helper qui va "dérouler" le tuple children
    // et appeler op(...) avec le résultat de l'évaluation de chaque enfant.
    template<std::size_t... I, typename... Args>
    constexpr auto apply_impl(std::index_sequence<I...>, Args&&... args) const
    {
      // Pour chaque enfant i dans children,
      // on appelle (std::get<I>(children))(args...) qui l'évalue.
      // Puis on forward tous les résultats à op(...).
      return op( (std::get<I>(children)(std::forward<Args>(args)...))... );
    }

    // Méthode print : on veut afficher une représentation "symbole" du node,
    // en demandant à op d'imprimer le résultat. Pour cela, on convertit
    // récursivement chaque sous-enfant en une chaîne de caractères, puis
    // on appelle op.print(os, ...).
    std::ostream& print(std::ostream& os) const
    {
      return print_impl(std::index_sequence_for<Children...>{}, os);
    }

    template<std::size_t... I>
    std::ostream& print_impl(std::index_sequence<I...>, std::ostream& os) const
    {
      // On convertit chaque enfant en chaîne de caractères
      std::string arr[] = { child_to_string(std::get<I>(children))... };
      // Puis on délègue l'affichage à op.print, qui reçoit autant de
      // paramètres qu'il y a d'enfants.
      return op.print(os, arr[I]...);
    }

    // Convertit un child (terminal ou node) en chaîne de caractères
    template<typename Child>
    static std::string child_to_string(const Child& c)
    {
      std::ostringstream oss;
      c.print(oss);
      return oss.str();
    }
  };

  //---------------------------------------------------------------------------------------
  /*
    Exemples de types d'opérations : add_, mul_, etc.
    Chaque "opération" fournit deux choses :
      - un operator()(...) qui effectue le calcul
      - une méthode print(...) qui décrit la forme de l'opération en texte
  */
  //---------------------------------------------------------------------------------------

  //----------------------------------------------
  // Opération add_
  //----------------------------------------------
  struct add_
  {
    // Calcul : addition de deux arguments
    constexpr auto operator()(auto a, auto b) const
    {
      return a + b;
    }

    // Affichage : "a + b"
    std::ostream& print(std::ostream& os, const std::string& a, const std::string& b) const
    {
      return os << a << " + " << b;
    }
  };

  // Opérateur + (consomme des expr, renvoie un node)
  template<expr L, expr R>
  constexpr auto operator+(L l, R r)
  {
    // On construit un node dont l'opérateur est add_, et dont les enfants sont L et R
    return node{add_{}, l, r};
  }

  //----------------------------------------------
  // Q4. Implémenter :
  //   - mul_ et operator*()
  //   - abs_ et une fonction abs()
  //   - fma_ et une fonction fma(a,b,c) = a*b + c
  //----------------------------------------------

  // Opération mul_
  struct mul_
  {
    // Calcul : multiplication de deux arguments
    constexpr auto operator()(auto a, auto b) const
    {
      return a * b;
    }

    // Affichage : "a * b"
    std::ostream& print(std::ostream& os, const std::string& a, const std::string& b) const
    {
      return os << a << " * " << b;
    }
  };

  // Opérateur * (consomme des expr, renvoie un node)
  template<expr L, expr R>
  constexpr auto operator*(L l, R r)
  {
    return node{mul_{}, l, r};
  }

  // Opération abs_
  struct abs_
  {
    // Calcul : valeur absolue (un argument)
    constexpr auto operator()(auto x) const
    {
      return (x < 0) ? -x : x;
    }

    // Affichage : "abs(a)"
    std::ostream& print(std::ostream& os, const std::string& a) const
    {
      return os << "abs(" << a << ")";
    }
  };

  // Fonction abs(...) (consomme un expr, renvoie un node)
  template<expr X>
  constexpr auto abs(X x)
  {
    return node{abs_{}, x};
  }

  // Opération fma_ (fused multiply-add)
  // fma(a, b, c) = a*b + c
  struct fma_
  {
    // Calcul : std::fma -> a*b + c
    constexpr auto operator()(auto a, auto b, auto c) const
    {
      // On peut utiliser std::fma (dans <cmath>) ou juste (a*b + c)
      return std::fma(a, b, c);
    }

    // Affichage : "fma(a, b, c)"
    std::ostream& print(std::ostream& os, const std::string& a, const std::string& b, const std::string& c) const
    {
      return os << "fma(" << a << ", " << b << ", " << c << ")";
    }
  };

  // Fonction fma(...) (consomme trois expr, renvoie un node)
  template<expr A, expr B, expr C>
  constexpr auto fma(A a, B b, C c)
  {
    return node{fma_{}, a, b, c};
  }

} // namespace et

//--------------------------------------------------------------------------------------
int main()
{
  // Q5. Exemple d'utilisation : 
  // On construit une expression fma(_1, abs(_2), _0).
  // Cela correspond mathématiquement à fma( arg<1>, abs(arg<2>), arg<0> )
  // => c'est-à-dire : (value_arg1 * abs(value_arg2)) + value_arg0
  // Les indices : _0 = arg<0>, _1 = arg<1>, _2 = arg<2>.
  //
  // L'ordre des arguments lors de l'appel : f(x0, x1, x2) => 
  //   _0 => x0
  //   _1 => x1
  //   _2 => x2
  //
  // Mais attention, quand on écrit f(1,2,3), ça veut dire :
  //   - _0 prend la valeur 1
  //   - _1 prend la valeur 2
  //   - _2 prend la valeur 3

  constexpr auto f = et::fma(et::_1, et::abs(et::_2), et::_0);

  // On affiche la structure de l'arbre d'expression
  // Résultat attendu : fma(arg<1>, abs(arg<2>), arg<0>)
  f.print(std::cout) << "\n";

  // On évalue l'expression pour (x0=1, x1=2, x2=3)
  // => f(1, 2, 3) = fma( arg<1>(1,2,3), abs(arg<2>(1,2,3)), arg<0>(1,2,3) )
  //               = fma(2, abs(3), 1)
  //               = 2 * 3 + 1
  //               = 6 + 1
  //               = 7
  std::cout << f(1, 2, 3) << "\n";  // Affiche 7

  //-------------------------------------------------------------------------
  // Tests complémentaires :
  // 1) Addition simple avec arg<0> + arg<1>
  constexpr auto add_expr = et::_0 + et::_1;
  add_expr.print(std::cout) << "\n";             // "arg<0> + arg<1>"
  std::cout << add_expr(10, 5) << "\n";          // 10 + 5 = 15

  // 2) Multiplication (arg<0> * arg<1>)
  constexpr auto mul_expr = et::_0 * et::_1;
  mul_expr.print(std::cout) << "\n";             // "arg<0> * arg<1>"
  std::cout << mul_expr(4, 5) << "\n";           // 4 * 5 = 20

  // 3) Valeur absolue d'une seule variable : abs(_0)
  constexpr auto abs_expr = et::abs(et::_0);
  abs_expr.print(std::cout) << "\n";             // "abs(arg<0>)"
  std::cout << abs_expr(-42) << "\n";            // abs(-42) = 42

  // 4) fma( _0, _1, _2 ) => x0*x1 + x2
  constexpr auto fma_expr = et::fma(et::_0, et::_1, et::_2);
  fma_expr.print(std::cout) << "\n";             // "fma(arg<0>, arg<1>, arg<2>)"
  std::cout << fma_expr(2.0, 10.0, 3.0) << "\n"; // 2.0*10.0 + 3.0 = 20 + 3 = 23

  return 0;
}
