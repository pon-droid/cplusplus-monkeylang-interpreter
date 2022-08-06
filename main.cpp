#include "lexer.hpp"

int main(int argc, char**argv){

  Lexer lexer("test2.ml");

  lexer.read_in();
 
  for(const auto &j: lexer.tokens){
      std::cout << j.data << " -- " << j.type << "!\n";
  }
 
  return 0;
}
