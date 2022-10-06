#pragma once

#include <assert.h>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>

enum token_type {
  ILLEGAL,
  IDENT,
  INT,
  ASSIGN,
  PLUS,
  MINUS,
  COMMA,
  SEMICOLON,
  LPAREN,
  RPAREN,
  LBRACE,
  RBRACE,
  FUNCTION,
  LET,
  END_FILE,
  SLASH,
  ASTERISK,
  BANG,
  LT,
  GT,
  TRUE,
  FALSE,
  IF,
  ELSE,
  RETURN,
  EQ,
  NOT_EQ
};

struct token {
  token_type type;
  std::string data;
};



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
    STR(MINUS);
    STR(BANG);
    STR(ASTERISK);
    STR(SLASH);
    STR(LT);
    STR(GT);
    STR(TRUE);
    STR(FALSE);
    STR(IF);
    STR(ELSE);
    STR(RETURN);
    STR(EQ);
    STR(NOT_EQ);
  }
#undef STR

  return out << s;
}


struct Lexer {
  std::vector<token> tokens;
  std::fstream file;

  std::unordered_map<std::string, token_type> keywords = {
    {"fn", FUNCTION},
    {"let", LET},
    {"true", TRUE},
    {"false", FALSE},
    {"if", IF},
    {"else", ELSE},
    {"return", RETURN}
  };
  
  void gen_tokens(const std::string& line);
  void read_in();

  Lexer() {}
  explicit Lexer(const std::string& filename) : file(filename) {}
};

void Lexer::gen_tokens(const std::string& line){
  

  auto is_ident = [&](char letter){
    return ('a' <= letter && letter <= 'z') || ('A' <= letter && letter <= 'Z');
  };

  auto is_num = [&](char letter){
    return ('0' <= letter) && (letter <= '9');
  };



    
  for(size_t i = 0; i < line.size(); i++){
    token tok;


    switch(line[i]) {
    case '=':
      if(line[i+1] == '='){
	tok.type = EQ;
	tok.data = line[i];
	tok.data += line[i+1];
	i++;
	tokens.push_back(tok);
	break;
      } else {
	tok.type = ASSIGN; goto DEFAULT;}

       
    case ';': tok.type = SEMICOLON; goto DEFAULT;
      
    case '(': tok.type = LPAREN; goto DEFAULT;
      
    case ')': tok.type = RPAREN; goto DEFAULT;
      
    case ',': tok.type = COMMA; goto DEFAULT;
      
    case '{': tok.type = LBRACE; goto DEFAULT;
      
    case '}': tok.type = RBRACE; goto DEFAULT;
      
    case '+': tok.type = PLUS; goto DEFAULT;
      
    case '-': tok.type = MINUS; goto DEFAULT;
      
    case '!':
      if(line[i+1] == '='){
	tok.type = NOT_EQ;
	tok.data = line[i];
	tok.data += line[i+1];
	tokens.push_back(tok);	
	i++;
	break;
      } else {
	tok.type = BANG; goto DEFAULT;}

      
    case '*': tok.type = ASTERISK; goto DEFAULT;
      
    case '/': tok.type = SLASH; goto DEFAULT;
      
    case '<': tok.type = LT; goto DEFAULT;
      
    case '>': tok.type = GT; goto DEFAULT;
       
    case ' ': break;
    DEFAULT:
    default:
      tok.data = line[i];
	  
      if(is_ident(line[i])){
	
	  for( ; ; ){	    
	    if(is_ident(line[i+1])){
	      tok.data += line[++i];
	    } else { break; }
	  }

	  if(keywords[tok.data]){
	    tok.type = keywords[tok.data];
	  } else {
	    tok.type = IDENT;
	  }
	  
      }

      if(is_num(line[i])){
	tok.type = INT;
	  for(;;){	    
	    if(is_num(line[i+1])){
	      tok.data += line[++i];
	    } else { break; }
	   
	  }
	  
      }
      
      tokens.push_back(tok);
    }
  }
}

void Lexer::read_in(){
  std::string line;
  while(std::getline(file,line)){
    gen_tokens(line);
  };

}


