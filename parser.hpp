#pragma once

#include "lexer.hpp"
#include <memory>
#include <numeric>

struct Node {
  virtual token return_token() const { token tok; return tok;};
  virtual std::string print_info() const { return "!!!!!!!"; };
};


class Statement : public Node {
  
};

class Expression : public Node {
  
};

class Identifier : public Expression {
public:  
  token ident;
  virtual token return_token() const { return ident; };
  virtual std::string print_info() const { return ident.data; };
};

class IntegerIdentifier : public Expression {
public:
  token ident;
  int val;
  virtual token return_token() const { return ident; }
  virtual std::string print_info() const { return ident.data; };
};

class LetStatement : public Statement {
public:
  token let;
  token ident;
  Expression value;
  virtual token return_token()  const { return let; };
  virtual std::string print_info() const { return let.data + " " + ident.data + " = ;\n"; };
    
};

class ReturnStatement : public Statement {
  public:
  token ret_stat;
  Expression value;
  virtual token return_token() const { return ret_stat; };
  virtual std::string print_info() const { return ret_stat.data + " ;\n"; };
};
    
class ExpressionStatement : public Statement {
public:
  token first_tok;
  std::shared_ptr<Expression> expression;
  virtual token return_token() const { return first_tok; };
  virtual std::string print_info() const { return expression->print_info(); };
};

class PrefixExpression : public Expression {
public:
  token prefix;
  std::shared_ptr<Expression> right;
  virtual token return_token() const { return prefix; };
  virtual std::string print_info() const { return "(" + prefix.data + right->print_info() + ")\n"; };
};


class Program {
public:
  std::vector<std::unique_ptr<Statement>> statements;
  void add(std::unique_ptr<Statement> object) { statements.emplace_back(std::move(object)); }
};

enum precedence {
  LOWEST,
  EQUALS, // ==
  LESSGREATER, // > and <
  SUM, // +
  PRODUCT, // *
  PREFIX, // -X or !X
  CALL // function(x)
};

class Parser {
public:
  std::unique_ptr<const std::vector<token>> tokens;
  Program parse_tokens();
  std::unique_ptr<Expression> parse_expression(const precedence& priority);
  size_t index;

  Parser(const std::vector<token> lexer_output){
    tokens = std::make_unique<const std::vector<token>>(lexer_output);
  };
};

#define TOKTYPE(index) (tokens->at(index).type)
#define TOK(index)  (tokens->at(index))

std::unique_ptr<Expression> Parser::parse_expression(const precedence& priority){
  
  switch(TOKTYPE(index)){
    case IDENT:{
      Identifier ident;
      ident.ident = TOK(index);
      return std::make_unique<Identifier>(ident);
    }
    case INT:{
      IntegerIdentifier ident;
      ident.ident = TOK(index);
      ident.val = std::stoi(TOK(index).data);
      return std::make_unique<IntegerIdentifier>(ident);
    }
    case BANG:
    case MINUS: {
      PrefixExpression pre;
      pre.prefix = TOK(index);
      index++;
      pre.right = parse_expression(PREFIX);
      return std::make_unique<PrefixExpression>(pre);
    }

      
    default:{ std::cout << "Parser FAIL\n"; return nullptr; }
  }
}

Program Parser::parse_tokens(){
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
    let.let = TOK(index);
      
    index+=3;
      
    for(;TOKTYPE(index) != SEMICOLON; index++){
      //      let.expressions.push_back(TOK(index));
    }
      
    statements.add(std::make_unique<LetStatement>(let));

    return LET;
  };

  auto parse_ret = [&](){
    if(TOKTYPE(index+1) != IDENT && TOKTYPE(index+1) != INT){
      std::cout << "parser fail: expected token after return to be expression, got "
		  << TOK(index+1).data << " of type " << TOKTYPE(index+1) << " instead\n";
      return ILLEGAL;
    }

    ReturnStatement ret;
    ret.ret_stat = TOK(index);
    index++;

    for(;TOKTYPE(index) != SEMICOLON; index++){
      //      ret.expressions.push_back(TOK(index));
    }
    
    statements.add(std::make_unique<Statement>(ret));
    return RETURN;
  };
  

  auto parse_expr_state = [&](){
    ExpressionStatement expr_stat;
    expr_stat.first_tok = TOK(index);
    
    expr_stat.expression = parse_expression(LOWEST);

    if(TOKTYPE(index+1) == SEMICOLON){ index++; }

    statements.add(std::make_unique<ExpressionStatement>(expr_stat));

    return expr_stat;
  };

  

  for(index = 0; index < tokens->size(); index++){
    switch(TOKTYPE(index)){
    case LET:{
      parse_let();
      break;
    }

    case RETURN:{
      parse_ret();
      break;
    }
      
    default: { parse_expr_state();  }    
    }
  }
  
  index = 0;
  return statements;
};

