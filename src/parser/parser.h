#include "../includes.h"
#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include "node.h"

/*
atom : INT | FLOAT
     : LPAREN expr RPAREN

PREC_0 : atom POW atom
       : atom

PREC_1 : PREC_0
       : PLUS PREC_1
       : MINUS PREC_1

PREC_2 : PREC_1 MUL PREC_1
       : PREC_1 DIV PREC_1
       : PREC_1 MOD PREC_1
       : PREC_1

PREC_3 : PREC_2 ADD PREC_2
        : PREC_2 SUB PREC_2
        : PREC_2

expr : PREC_3
     : let IDENTIFIER = expr
*/

#ifndef PARSER_H
#define PARSER_H

class Parser {
std::vector<Token> tokens;
int currentTokenIndex;
Token currentToken;
public:
  Parser(std::vector<Token> _tokens) {
    tokens = _tokens;
    currentTokenIndex = -1;
    advance();
  }

  void advance() {
    currentTokenIndex++;
    if (currentTokenIndex < tokens.size()) {
      currentToken = tokens[currentTokenIndex];
    } else {
      currentToken = Token(KEY_EOF, CursorPosition(), CursorPosition());
    }
  }

  std::pair<node *, Error> parse() {
    auto res = parseExpr();
    if (res.second.has_error() && currentToken.get_name() != KEY_EOF) 
      return std::make_pair(new node(), res.second);
    return std::make_pair(res.first, Error());
  }

  std::pair<node *, Error> parseAtom() {

    if (currentToken.get_value()._type == types::_int || currentToken.get_value()._type == types::_float) {
      node *_node = new node{currentToken, node_type::_number, nullptr, nullptr};
      advance();
      return std::make_pair(_node, Error());
    } 

    if (currentToken.get_name() == KEY_LPAREN) {
      advance();
      auto expr = parseExpr();
      if (expr.second.has_error()) return std::make_pair(new node(), expr.second);
      if (currentToken.get_name() != KEY_RPAREN) {
        return std::make_pair(new node(), Error("Invalid Syntax Error", "Expected ')'"));
      }
      advance();
      return expr;
    }

    if (currentToken.get_value()._type == types::_id) {
      node *_node = new node{currentToken, node_type::_variable_access, nullptr, nullptr};
      advance();
      return std::make_pair(_node, Error());
    }

    return std::make_pair(new node(), Error("Invalid Syntax Error", "Expected Number"));
  }

  std::pair<node *, Error> parsePrec0() {
    auto left = parseAtom();
    if (left.second.has_error()) return std::make_pair(new node(), left.second);
    if (currentToken.get_name() == KEY_POW) {
      auto op = currentToken;
      advance();
      auto right = parseAtom();
      if (right.second.has_error()) return std::make_pair(new node(), right.second);
      return std::make_pair(new node{op, node_type::_binary_operator, left.first, right.first}, Error());
    }
    return left;
  }

  std::pair<node *, Error> parsePrec1() {
    if (currentToken.get_name() == KEY_ADD || currentToken.get_name() == KEY_SUB) {
      auto op = currentToken;
      advance();
      auto right = parsePrec1();
      if (right.second.has_error()) return std::make_pair(new node(), right.second);
      return std::make_pair(new node{op, node_type::_unary_operator, nullptr, right.first}, Error());
    }
    return parsePrec0();
  }

  std::pair<node *, Error> parsePrec2() {
    auto left = parsePrec1();
    if (left.second.has_error()) return std::make_pair(new node(), left.second);
    if (currentToken.get_name() == KEY_MUL || currentToken.get_name() == KEY_DIV || currentToken.get_name() == KEY_MOD) {
      auto op = currentToken;
      advance();
      auto right = parsePrec1();
      if (right.second.has_error()) return std::make_pair(new node(), right.second);
      return std::make_pair(new node{op, node_type::_binary_operator, left.first, right.first}, Error());
    }
    return left;
  }

  std::pair<node *, Error> parsePrec3() {
    auto left = parsePrec2();
    if (left.second.has_error()) return std::make_pair(new node(), left.second);
    if (currentToken.get_name() == KEY_ADD || currentToken.get_name() == KEY_SUB) {
      auto op = currentToken;
      advance();
      auto right = parsePrec2();
      if (right.second.has_error()) return std::make_pair(new node(), right.second);
      return std::make_pair(new node{op, node_type::_binary_operator, left.first, right.first}, Error());
    }
    return left;
  }

  std::pair<node *, Error> parseExpr() {
    if (currentToken.get_value()._type == types::_keyword && currentToken.get_name() == KEY_ASSIGN) {
      advance();
      if (currentToken.get_value()._type != types::_id) {
        return std::make_pair(new node(), Error("Invalid Syntax Error", "Expected Identifier"));
      }
      auto identifier = currentToken;
      advance();
      if (currentToken.get_name() != KEY_EQ) {
        return std::make_pair(new node(), Error("Invalid Syntax Error", "Expected '='"));
      }
      advance();
      auto expr = parseExpr();
      if (expr.second.has_error()) return std::make_pair(new node(), expr.second);
      return std::make_pair(new node{identifier, node_type::_variable_assign, expr.first, nullptr}, Error());
    }
    return parsePrec3();
  }
};

#endif