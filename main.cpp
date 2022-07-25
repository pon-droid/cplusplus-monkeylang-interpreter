#include <vector>
#include <iostream>
#include <fstream>

std::string input_buffer = {
  "let five = 5;\n"
  "let ten = 10;\n"
  "\n"
  "let add = fn(x, y) {\n"
  " x + y;\n"
  "};\n"
  "\n"
  "let result = add(five, ten);"
};


enum token_type {
  ILLEGAL,
  IDENT,
  INT,
  ASSIGN,
  PLUS,
  COMMA,
  SEMICOLON,
  LPAREN,
  RPAREN,
  LBRACE,
  RBRACE,
  FUNCTION,
  LET,
  END_FILE
};

struct token {
  token_type type;
  char data;
};

struct Lexer {
  std::fstream file;
  char ch;
  std::vector<token> tokens;
  void read_char();

  Lexer(const std::string& filename);
};

Lexer::Lexer(const std::string& filename){
  file.open(filename,std::fstream::in);
  ch = -1;
};

void Lexer::read_char(){
  while(file >> ch){
    token tok;
    
    switch(ch) {
    case '=':
      tok.type = ASSIGN;
      break;
    case ';':
      tok.type = SEMICOLON;
      break;
    case '(':
      tok.type = LPAREN;
      break;
    case ')':
      tok.type = RPAREN;
      break;
    case ',':
      tok.type = COMMA;
      break;
    case '+':
      tok.type = PLUS;
      break;
    case '{':
      tok.type = LBRACE;
      break;
    case '}':
      tok.type = RBRACE;
      break;
    default:
      if (isalpha(ch)){
	tok.type = IDENT;
	//do something smart
	std::cout << tok.data << "\n";
	//read_char();
	break;
      }
    }

    tok.data = ch;

    tokens.push_back(tok);
  }
}


std::ostream& operator<<(std::ostream& out, const token_type& val){
  std::string s;
#define STR(p) case(p): s = #p; break;
  switch(val){
    STR(ILLEGAL);
    STR(IDENT);
    STR(INT);
    STR(ASSIGN);
    STR(PLUS);
    STR(COMMA);
    STR(SEMICOLON);
    STR(LPAREN);
    STR(RPAREN);
    STR(LBRACE);
    STR(RBRACE);
    STR(FUNCTION);
    STR(LET);
    STR(END_FILE);
  }
#undef STR

  return out << s;
}

int main(){
  //std::cout << input_buffer;


  
  Lexer lexer("test.ml");
  lexer.read_char();
  


  for(const auto &i: lexer.tokens){
    std::cout << i.data << " -- " << i.type << "\n";
  };





  return 1;
  
  char ch;
  std::fstream file("simp.ml",std::fstream::in);

  std::vector<token> tokens;
  
  while(file >> ch){
    token tok;

    switch(ch) {
    case '=':
      tok.type = ASSIGN;
      break;
    case ';':
      tok.type = SEMICOLON;
      break;
    case '(':
      tok.type = LPAREN;
      break;
    case ')':
      tok.type = RPAREN;
      break;
    case ',':
      tok.type = COMMA;
      break;
    case '+':
      tok.type = PLUS;
      break;
    case '{':
      tok.type = LBRACE;
      break;
    case '}':
      tok.type = RBRACE;
      break;
    default:
      if (isalpha(ch)){
	tok.type = IDENT;
	//do something smart
      }
    }

    tok.data = ch;

    tokens.push_back(tok);
  };

  for(const auto &i: tokens){
    std::cout << i.data << " -- " << i.type << "\n";
  };
  return 0;
}
