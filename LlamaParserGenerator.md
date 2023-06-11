CFGSymbolTable
=== Generating The Symbol Table ====
 - Superclass for terminals, non-terminals and actions
 - Hash table for Symbols, mapping symbol name to symbol
 - Synchroniztion Set 
 - Goal symbol 
 - Current nonterminal 
 - make_terminal, new_nonterm, new_rhs, add_to_rhs
 - set of used / set Symbols 
    -find problems to check if a Symbol is used but not set.

=== Generting the Parse Table ===
LlamaParserGenerator
   - DTran: 2D array indexed by nonterimanals on rows lookahead symbols 
     in the column representing parse table 

   - make_dtran, make_yy_pushtab, make_yy_acts

 ? Terms array holding pointers to terminal / non-terminal symbols 


 --------------------------------------------------

 Important Structures:
 YYd - DFA transition table. Indexed by lookahead symbol and top of stack symbol. Requires select set for each symbol 

 Yypushtab - Indexed by production number. Evaluates to list of
 symbol values to push on the stack when executing given productiong

 action_name -> action_str
 nontermninal_name -> list of productions of that nonterminal
 production = list of names of terminals/nonterminls/actions