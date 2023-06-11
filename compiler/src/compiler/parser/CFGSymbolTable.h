#include <string>
#include <unordered_map> 
#include <memory> 
#include <set>
#include <vector>  

class CFGSymbol; 
using CFGSymbolPtr = std::unique_ptr<CFGSymbol>;

class NonterminalSymbol; 
using NonterminalSymbolPtr = std::unique_ptr<NonterminalSymbol>; 
struct  Production{
    int num; // Production Number
    std::vector<CFGSymbol> rhs; 
    std::string lhs;  
};

using ProductionPtr = std::unique_ptr<Production>; 

class CFGSymbol {
    public: 
    enum symbol_type  {term, nonterm, action}; 
    CFGSymbol(std::string name, symbol_type type)
         : name(name),  type(type)  {}

    std::string get_name() {return name;}; 
    symbol_type get_type() {return type;};
    private: 
    
    std::string name; 
    symbol_type type; 

};

class NonterminalSymbol {
    public: 
    std::string get_name() {return name;}; 

    int val; 
    std::string name; 
    std::set<int> first; 
    std::set<int> follow; 
    std::vector<ProductionPtr> productions; 

}; 

class ActionSymbol {
    int val;
    std::string action_str; 
};

using ActionSymbolPtr = std::unique_ptr<ActionSymbol>;

class CFGSymbolTable {
    
    public:

    int cur_term; 
    int max_term;  
    int max_prod; 

    int cur_nonterm;
    int max_nonterm; 
 

    int cur_act; 
    int min_act;
    int max_act; 
    
    std::unordered_map<std::string, int> terminal_table; 
    std::unordered_map<std::string, NonterminalSymbolPtr> nonterminal_table;
    std::unordered_map<std::string, ActionSymbolPtr> action_table;  

    NonterminalSymbolPtr cur_lhs; 
    ProductionPtr cur_production; 
    int num_productions;

    public: 
    CFGSymbolTable() : cur_term(0),max_term(255),
                        max_prod(100), cur_nonterm(256),
                        max_nonterm(511),cur_act(512),
                        min_act(512), max_act(1023) {

    }
    void make_terminal(std::string name);
    void new_lhs(std::string name); 
    void new_production(); 
    void add_nonaction(std::string name); 
    void add_action(std::string action_str); 
    void add_production(); 

    void compute_firsts(); 
};

class LlamaParserGenerator {

    private: 
    CFGSymbolTable symbol_table;
    bool first_closure(NonterminalSymbolPtr& lhs);
    void first();  
};