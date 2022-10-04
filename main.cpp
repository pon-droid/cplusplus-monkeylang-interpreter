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

class LetStatement {
public:
  token let_token;
  std::string val;
  std::string print_token() const { return let_token.data; }
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
