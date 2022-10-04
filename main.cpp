#include "lexer.hpp"
#include <assert.h>

template <class T>
class Node {
  T const & t;
public:
  Node(T const & obj) : t(obj) { }
  std::string print_token() const { return t.print_token(); } 
};

struct Ident { token ident_tok; std::string val; };


/*
class LetStatement {
public:
  token let_token;
  std::string val;
  std::string print_token() const { return let_token.data; }
};
*/

class LetStatement {
public:
  token let;
  token ident;
  std::vector<token> expression;
  explicit LetStatement(const token& let, const token& ident): let(let), ident(ident) {}
};
  
class ReturnStatement {
public:
  token ret_token;
  std::string return_val;
};

class Expression {
public:
  token first_tok;
  std::string exp;
};


int main(int argc, char**argv){
 
  assert(argv[1] != NULL);
  Lexer lexer(argv[1]);
  lexer.read_in();

  std::vector<LetStatement> statements;
  /*
  for(size_t i = 0; i < lexer.tokens.size(); i++){
    switch(lexer.tokens[i].type){
    case LET:
      if(lexer.tokens[i+1].type == IDENT){
	LetStatement let;
	let.val = lexer.tokens[i+1].data;
	let.let_token = lexer.tokens[i+1];
	statements.push_back(let);
	if(!(lexer.tokens[i+2].type == ASSIGN)){
	  std::cout << "Expected ASSIGN but got " << lexer.tokens[i+2].data <<
	  " ! " << lexer.tokens[i+2].type << "\n";
	}
      } else {
	std::cout << "Expected LET but got " << lexer.tokens[i+1].data << " ! " << lexer.tokens[i+1].type <<
	"\n";
      }
    case RETURN:
    
    default:
      continue;
    }

  }

  for(const auto &i: statements){
    std::cout << i.val << "\n";
  }
  */

#define TOKTYPE(index) (lexer.tokens[index].type)
#define TOK(index)  (lexer.tokens[index])

  for(size_t i = 0; i < lexer.tokens.size(); i++){
    switch(TOKTYPE(i)){
    case LET:{
      
      if(TOKTYPE(i+1) != IDENT){
	std::cout << "parser fail: expected next token to be identifier, got " <<
	TOK(i+1).data << " of type " << TOKTYPE(i+1) << "\n";
        continue;
      }
      else if (TOKTYPE(i+2) != ASSIGN){	
	std::cout << "parser fail: expected next token to be assignment = , got " <<
	TOK(i+2).data << " of type " << TOKTYPE(i+2) << "\n";
	continue;
      }
      
      assert(TOKTYPE(i+1) == IDENT && TOKTYPE(i+2) == ASSIGN);
      
      LetStatement let(TOK(i), TOK(i+1));
      i+=3;
      
      for(;TOKTYPE(i) != SEMICOLON; i++){
	let.expression.push_back(TOK(i));
      }
      
      statements.push_back(let);
    }
      
    default: continue;
      
    }
  }


  for(const auto &i: statements){
    std::cout << i.ident.data << " | ";
    for(const auto &j: i.expression){
      std::cout << j.data << " ";
    };
    std::cout << "\n";
  }
      
      


  
  /*
  std::cout << "Monkeylang interpreter - in C++!\n";
  
  while(true){
    Lexer lexer;
    std::string line;
    std::cout << ">> ";
    std::getline(std::cin, line);
    lexer.gen_tokens(line);

    for(const auto &j: lexer.tokens){
	std::cout << j.data << " -- " << j.type << "!\n";
    }
  }
  */
 
  return 0;
}
