#pragma once

#include "lexer.hpp"
#include <memory>
#include <numeric>
#define ReturnStatement Statement

class Statement {
public:
  token state_tok;
  std::vector<token> expressions;
  virtual token get_ident() const { return state_tok; };
  virtual std::string print_info() const {
    std::string sum_expr;
    for(const auto &i: expressions){
      sum_expr += i.data;
    }

    return state_tok.data + " " + sum_expr + ";\n";};
};

class LetStatement : public Statement {
public:
  token ident;
  virtual token get_ident()  const { return ident; };
  virtual std::string print_info()  const {
    std::string sum_expr;
    for(const auto &i: expressions){
      sum_expr += i.data;
    }

    return state_tok.data + " " + ident.data + " = " + sum_expr + ";\n";};
};

class Program {
public:
  std::vector<std::shared_ptr<Statement>> statements;
  void add(std::shared_ptr<Statement> object) { statements.push_back(object); }
};

class Parser {
public:
  Program parse_tokens(const std::vector<token>& tokens);
  size_t index;
};

#define TOKTYPE(index) (tokens[index].type)
#define TOK(index)  (tokens[index])

Program Parser::parse_tokens(const std::vector<token>& tokens){
  Program statements;

  auto parse_let = [&](){
    if(TOKTYPE(index+1) != IDENT){
      std::cout << "parser fail: expected next token to be identifier, got " <<
	TOK(index+1).data << " of type " << TOKTYPE(index+1) << "\n";
      return ILLEGAL;
    }
    else if (TOKTYPE(index+2) != ASSIGN){	
      std::cout << "parser fail: expected next token to be assignment = , got " <<
	TOK(index+2).data << " of type " << TOKTYPE(index+2) << "\n";
      return ILLEGAL;
    }
      
    assert(TOKTYPE(index+1) == IDENT && TOKTYPE(index+2) == ASSIGN);
      
    LetStatement let;
    let.ident = TOK(index+1);
    let.state_tok = TOK(index);
      
    index+=3;
      
    for(;TOKTYPE(index) != SEMICOLON; index++){
      let.expressions.push_back(TOK(index));
    }
      
    statements.add(std::make_shared<LetStatement>(let));

    return LET;
  };

  auto parse_ret = [&](){
    if(TOKTYPE(index+1) != IDENT && TOKTYPE(index+1) != INT){
      std::cout << "parser fail: expected token after return to be expression, got "
		  << TOK(index+1).data << " of type " << TOKTYPE(index+1) << " instead\n";
      return ILLEGAL;
    }

    ReturnStatement ret;
    ret.state_tok = TOK(index);
    index++;

    for(;TOKTYPE(index) != SEMICOLON; index++){
      ret.expressions.push_back(TOK(index));
    }
    
    statements.add(std::make_shared<Statement>(ret));
    return RETURN;
  };

  

  for(index = 0; index < tokens.size(); index++){
    switch(TOKTYPE(index)){
    case LET:{
      parse_let();
      break;
    }

    case RETURN:{
      parse_ret();
      break;
    }
      
    default: continue;
      
    }
  }
  
  index = 0;
  return statements;
};

