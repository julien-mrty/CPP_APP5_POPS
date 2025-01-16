#include <iostream>
#include <cmath>
#include <tuple>
#include <algorithm>
#include <sstream>


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
  concept expr = requires()
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
      // auto tuple = std::forward_as_tuple(args...); // Fonctionne aussi ?
      auto tuple = std::forward_as_tuple(std::forward<Args>(args)...);
      return std::get<ID>(tuple);
    }
  };

  // Generateur de variable numérotée
  template<int ID>
  inline constexpr auto arg = terminal<ID>{};
  //  inline constexpr terminal<ID> arg{};

  // Variables _0, _1 et _2 sont prédéfinies
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
    constexpr node(Op o, Children... c) : op(o), children(c...) {}

    // L'operateur() de node permet d'évaluer le sous arbre courant de manière 
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
          return std::array{ child(std::forward<Args>(args)...)... };
        },
        children
      );

      return std::apply(op, subresults);
    }

    // Affiche un node en demandant à Op d'afficher les sous arbres
    std::ostream& print(std::ostream& os) const
    {
      // Pour chaque sous-enfant, on capture sa représentation texte :
      auto arr = std::apply(
        [&](auto const&... child){
          return std::array<std::string, sizeof...(Children)>{
            child_to_string(child)...
          };
        },
        children
      );
      // Puis on appelle op.print en lui passant ce tableau :
      return op.print(os, arr);
    }

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
      return os << a << " + " << b;
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
      return os << a << " * " << b;
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
      return (a > 0) ? a : -a;
    }

    std::ostream& print(std::ostream& os, auto a) const
    {
      return os << "abs(" << a << ")";
    }
  };
  
  template<expr T>
  constexpr auto abs(T expr1)
  {
    return node{abs_{}, expr1};
  }


  struct fma_ 
  {
    constexpr auto operator()(auto a, auto b, auto c) const
    {
      return a * b + c;
    }

    std::ostream& print(std::ostream& os, auto a, auto b, auto c) const
    {
      return os << a << " * " << b << " + " << c;
    }
  };
  
  template<expr L, expr M, expr R>
  constexpr auto fma(L l, M m, R r)
  {
    return node{fma_{}, l, m, r};
  }
  
}

int main()
{
  // Q5. Le mini exemple ci dessous doit fonctionner. Complétez le avec une série de tests
  // exhaustif de tout les cas qui vous paraissent nécessaire.
  constexpr auto f = et::fma(et::_1, abs(et::_2),et::_0);

  f.print(std::cout) << "\n";

  std::cout << f(1,2,3) << "\n";

  return 0;
}