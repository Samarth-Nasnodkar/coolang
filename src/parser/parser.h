#include "../includes.h"
#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include "../variables/cmp.h"
#include "../variables/logic_operators.h"
#include "node.h"

/*
atom : INT | FLOAT | IDENTIFIER
     : LPAREN PREC_5 RPAREN

PREC_0 : atom POW atom
       : atom

PREC_1 : PREC_0
       : PLUS PREC_1
       : MINUS PREC_1
       : NOT PREC_1

PREC_2 : PREC_1 MUL PREC_1
       : PREC_1 DIV PREC_1
       : PREC_1 MOD PREC_1
       : PREC_1

PREC_3 : PREC_2 ADD PREC_2
       : PREC_2 SUB PREC_2
       : PREC_2

PREC_4 : PREC_3 LE/LT/GE/GT PREC_3
       : PREC_3

PREC_5 : PREC_4 EE/NE PREC_4
       : PREC_4

PREC_6 : PREC_5 AND PREC_5
       : PREC_5

PREC_7 : PREC_6 OR PREC_6
       : PREC_6

PREC_TOP : let (mut) IDENTIFIER EQ PREC_5
         : PREC_5

PREC_IF : if PREC_TOP CODE_BLOCK else (if PREC_TOP) CODE_BLOCK
        : PREC_TOP

code_block : { PREC_IF ... }
           : PREC_IF EOL | EOF
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

  void reverse(int amount = 1) {
    currentTokenIndex -= amount;
    if (currentTokenIndex >= 0) {
      currentToken = tokens[currentTokenIndex];
    } else {
      currentToken = Token(KEY_EOF, CursorPosition(), CursorPosition());
    }
  }

  bool isUnary() {
    return currentToken.get_name() == KEY_ADD || currentToken.get_name() == KEY_SUB || currentToken.get_name() == KEY_NOT;
  }

  bool isCmp() {
    for (auto cmp : cmps) {
      if (cmp.name == currentToken.get_name()) {
        return true;
      }
    }
    return false;
  }

  std::pair<std::vector<node *>, Error> parse() {
    std::vector<node *> res;
    while (currentToken.get_name() != KEY_EOF) {
      auto _res = parseCodeBlock();
      if (_res.second.has_error()) return std::make_pair(std::vector<node *>(), _res.second);
      res.push_back(_res.first);
      advance();
    }
    return std::make_pair(res, Error());
  }

  std::pair<node *, Error> parseAtom() {
    // std::cout << currentToken.to_string() << std::endl;
    if (currentToken.get_value()._type == types::_null && currentToken.has_data()) {
      node *_node = new node{currentToken, node_type::_nullnode, nullptr, nullptr};
      advance();
      return std::make_pair(_node, Error());
    }
    if (currentToken.get_value()._type == types::_int || currentToken.get_value()._type == types::_float) {
      node *_node = new node{currentToken, node_type::_number, nullptr, nullptr};
      advance();
      return std::make_pair(_node, Error());
    } 

    if (currentToken.get_value()._type == types::_bool) {
      node *_node = new node{currentToken, node_type::_boolean, nullptr, nullptr};
      advance();
      return std::make_pair(_node, Error());
    }

    if (currentToken.get_name() == KEY_LPAREN) {
      advance();
      auto expr = parsePrec5();
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
    if (isUnary()) {
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

  std::pair<node *, Error> parsePrec4() {
    auto left = parsePrec3();
    if (left.second.has_error()) return std::make_pair(new node(), left.second);
    if (
        currentToken.get_name() == KEY_LE 
        || currentToken.get_name() == KEY_LT 
        || currentToken.get_name() == KEY_GE 
        || currentToken.get_name() == KEY_GT
      ) {
      auto op = currentToken;
      advance();
      auto right = parsePrec3();
      if (right.second.has_error()) return std::make_pair(new node(), right.second);
      return std::make_pair(new node{op, node_type::_binary_operator, left.first, right.first}, Error());
    }
    return left;
  }

  std::pair<node *, Error> parsePrec5() {
    auto left = parsePrec4();
    if (left.second.has_error()) return std::make_pair(new node(), left.second);
    if (currentToken.get_name() == KEY_EE || currentToken.get_name() == KEY_NE ) {
      auto op = currentToken;
      advance();
      auto right = parsePrec4();
      if (right.second.has_error()) return std::make_pair(new node(), right.second);
      return std::make_pair(new node{op, node_type::_binary_operator, left.first, right.first}, Error());
    }
    return left;
  }

  std::pair<node *, Error> parsePrec6() {
    auto expr = parsePrec5();
    if (expr.second.has_error()) return std::make_pair(new node(), expr.second);
    if (currentToken.get_name() == KEY_AND) {
      auto op = currentToken;
      advance();
      auto expr2 = parsePrec5();
      if (expr2.second.has_error()) return std::make_pair(new node(), expr2.second);
      return std::make_pair(new node{op, node_type::_binary_operator, expr.first, expr2.first}, Error());
    }
    return expr;
  }

  std::pair<node *, Error> parsePrec7() {
    auto prec4 = parsePrec6();
    if (prec4.second.has_error()) return std::make_pair(new node(), prec4.second);
    if (currentToken.get_name() == KEY_OR) {
      auto op = currentToken;
      advance();
      auto cond2 = parsePrec6();
      if (cond2.second.has_error()) return std::make_pair(new node(), cond2.second);
      return std::make_pair(new node{op, node_type::_binary_operator, prec4.first, cond2.first}, Error());
    }
    return prec4;
  }

  std::pair<node *, Error> parsePrecTop() {
    if (currentToken.get_value()._type == types::_keyword && currentToken.get_name() == KEY_ASSIGN) {
      bool mut = false;
      advance();
      if (currentToken.get_value()._type != types::_id) {
        if (currentToken.get_value()._type == types::_keyword && currentToken.get_name() == KEY_MUT) {
          mut = true;
          advance();
        } else {
          return std::make_pair(new node(), Error("Invalid Syntax Error", "Expected Identifier"));
        }
      }
      auto identifier = currentToken;
      advance();
      if (currentToken.get_name() != KEY_EQ) {
        return std::make_pair(new node(), Error("Invalid Syntax Error", "Expected '='"));
      }
      advance();
      auto expr = parsePrec7();
      if (expr.second.has_error()) return std::make_pair(new node(), expr.second);

      // if (currentToken.get_name() != KEY_NEWLINE && currentToken.get_name() != KEY_EOF) {
      //   return std::make_pair(new node(), Error("Invalid Syntax Error", "Expected new line"));
      // }

      return std::make_pair(new node{identifier, mut ? node_type::_variable_assign_mut : node_type::_variable_assign, expr.first, nullptr}, Error());
    }

    if (currentToken.get_value()._type == types::_id) {
      auto identifier = currentToken;
      advance();
      if (currentToken.get_name() != KEY_EQ) {
        reverse();
        return parsePrec7();
      }
      advance();
      auto expr = parsePrec7();
      if (expr.second.has_error()) return std::make_pair(new node(), expr.second);

      // if (currentToken.get_name() != KEY_NEWLINE && currentToken.get_name() != KEY_EOF) {
      //   return std::make_pair(new node(), Error("Invalid Syntax Error", "Expected new line"));
      // }

      return std::make_pair(new node{identifier, node_type::_variable_reassign, expr.first, nullptr}, Error());
    }
    return parsePrec7();  
  }

  std::pair<node *, Error> parsePrecIf() {
    if (currentToken.get_value()._type == types::_keyword && currentToken.get_name() == KEY_IF) {
      auto op_tok = currentToken;
      advance();
      std::vector<node *> elifs;
      auto cond = parsePrecTop();
      if (cond.second.has_error()) return {new node(), cond.second};
      auto code_block = parseCodeBlock();
      if (code_block.second.has_error()) return {new node(), code_block.second};
      
      while (currentToken.get_name() == KEY_NEWLINE) advance();
      
      while (currentToken.get_value()._type == types::_keyword && currentToken.get_name() == KEY_ELIF) {
        auto _elif = currentToken;
        advance();
        auto _cond = parsePrecTop();
        if (_cond.second.has_error()) return {new node(), _cond.second};
        auto _code_block = parseCodeBlock();
        if (_code_block.second.has_error()) return {new node(), _code_block.second};
        elifs.emplace_back(new node{_elif, node_type::_if_else, _cond.first, _code_block.first});
      }

      while (currentToken.get_name() == KEY_NEWLINE) advance();

      if (currentToken.get_value()._type == types::_keyword && currentToken.get_name() == KEY_ELSE) {
        auto _else = currentToken;
        advance();
        auto _code_block = parseCodeBlock();
        if (_code_block.second.has_error()) return {new node(), _code_block.second};
        elifs.emplace_back(new node{_else, node_type::_if_else, nullptr, _code_block.first});
      }

      return {new node{op_tok, node_type::_if_else, cond.first, code_block.first, elifs}, Error()};
    }
    return parsePrecTop();
  }

  std::pair<node *, Error> parseCodeBlock() {
    if (currentToken.get_name() == KEY_LBRACE) {
      advance();
      std::vector<node *> nodes;
      while (currentToken.get_name() != KEY_RBRACE) {
        if (currentToken.get_name() == KEY_NEWLINE) {
          advance();
          continue;
        }
        if (currentToken.get_name() == KEY_LBRACE) {
          auto code_block = parseCodeBlock();
          if (code_block.second.has_error()) return std::make_pair(new node(), code_block.second);
          nodes.push_back(code_block.first);
          continue;
        }
        auto expr = parsePrecIf();
        if (expr.second.has_error()) return std::make_pair(new node(), expr.second);
        nodes.push_back(expr.first);
      }
      advance();
      return std::make_pair(new node{Token(KEY_CODE_BLOCK, CursorPosition(), CursorPosition()), node_type::_code_block, nullptr, nullptr, nodes}, Error());
    }

    return parsePrecIf();
  }
};

#endif