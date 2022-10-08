#include "lexer.hpp"
#include <assert.h>
#include "parser.hpp"


int main(int argc, char**argv){
 
  assert(argv[1] != NULL);
  Lexer lexer(argv[1]);
  lexer.read_in();

  Parser parser(lexer.tokens);
  Program program(parser.parse_tokens());

  
  for(const auto &i: program.statements){
    std::cout << i->print_info();
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
