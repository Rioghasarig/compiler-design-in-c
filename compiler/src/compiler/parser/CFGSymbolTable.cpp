#include "CFGSymbolTable.h"
#include <sstream>
#include <algorithm> 

CFGSymbolTable cfg_symbol_table;
void CFGSymbolTable::make_terminal(std::string name) {
    
    if( terminal_table.find(name) != terminal_table.end() )
    {   
        std::ostringstream error_msg; 
        error_msg << "Terminal symbol <" << name 
            << "> already exists in table" << std::endl; 

        throw std::invalid_argument(error_msg.str());
    }

    if ( cur_term >= max_term) {
        std::ostringstream error_msg;
        error_msg << "Too many terminal symbols (max: " << max_term << " )"; 
        throw std::length_error(error_msg.str()); 
    }
 
    terminal_table.insert(
        {name,cur_term++});

}

void CFGSymbolTable::new_lhs(std::string name) {

    if (nonterminal_table.find(name) != nonterminal_table.end() ) {
        std::ostringstream error_msg; 
        error_msg << "Nonterminal symbol <" << name 
            << "> already exists in table" << std::endl; 
    
        throw std::invalid_argument(error_msg.str()); 
    }

    if ( cur_nonterm >= max_nonterm ) {
        std::ostringstream error_msg; 
        error_msg << "Too many nonterminal symbols (max: " << max_nonterm << ")";
        throw std::length_error(error_msg.str()); 
    }

    
    cur_lhs = std::make_unique<NonterminalSymbol>();    
}

void CFGSymbolTable::new_production() {
    cur_production = std::make_unique<Production>(); 
    cur_production->num = num_productions++; 
    if(cur_production->num >= max_prod) {
        std::ostringstream error_msg; 
        error_msg << "Too many productions. Max: " << max_prod << std::endl;
        throw std::length_error(error_msg.str());  
    }
    cur_production->lhs = cur_lhs->get_name(); 
}

void CFGSymbolTable::add_nonaction(std::string name_str) {
    /*Terminal Symbols should have already been declared by
      the %term directive so if it's not found in the terminal
      table, assume it's a nonterminal*/
    if(terminal_table.find(name_str) != terminal_table.end()) {
        cur_production->rhs.emplace_back(
            name_str, 
            CFGSymbol::term
        );   
    }
    else {
        cur_production->rhs.emplace_back(
            name_str, 
            CFGSymbol::nonterm
        );
    }
}
void CFGSymbolTable::add_action(std::string action_str)  {
    std::ostringstream action_name;
    action_name << "{" << cur_act - min_act << "}"; 
    cur_production->rhs.emplace_back(
        action_name.str(),
        CFGSymbol::action
    );
}

void CFGSymbolTable::add_production() {
    cur_lhs->productions.push_back(std::move(cur_production));
}



bool first_closure(NonterminalSymbolPtr& lhs) {
    int init_size = lhs->first.size(); 
    for(ProductionPtr& prod : lhs->productions) {
        bool empty_production = true; 
        for(CFGSymbol y : prod->rhs) {
            if (y.get_type() == CFGSymbol::term) {
                empty_production  = false; 
                int val = cfg_symbol_table.terminal_table.at(y.get_name()); 
                lhs->first.insert(val);
                printf("Adding terminal %s to lhs %s\n", y.get_name(), lhs->name);
            }
/*          if (y.get_type() == CFGSymbol::nonterm) {
                empty_production = false; 
                printf("Adding nonterminal %s to lhs %s\n", cfg_symbol_table.nonterminal_table.at(y.get_name())->name, lhs->name);
                std::set_union(
                    lhs->first.begin(),lhs->first.end(),
                    cfg_symbol_table.nonterminal_table.at(y.get_name()),
                    cfg_symbol_table.nonterminal_table.at(y.get_name()), 
                    std::inserter(lhs->first,lhs->first.begin())); 
            }*/
        }
    }
    int final_size = lhs->first.size(); 

    if(final_size > init_size) {
        return true;
    }
    else {
        return false; 
    }
}

void CFGSymbolTable::compute_firsts() {
    for(auto& i : nonterminal_table) {
        first_closure(i.second);
    }
}

void LlamaParserGenerator::first() {
    bool did_something = true; 
    while(did_something)  {
         did_something = false; 
        for(auto& [k,v] : symbol_table.nonterminal_table) {
            did_something = did_something | first_closure(v);
        }
    } 
}

