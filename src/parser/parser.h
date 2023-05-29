#include "../includes.h"
#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include "../variables/cmp.h"
#include "../variables/logic_operators.h"
#include "node.h"

/*
atom : INT | FLOAT | STR | LIST | LIST_ELEM | IDENTIFIER
     : LPAREN PREC_5 RPAREN

FUNCTION_CALL : FUNC_CALL
              : atom

PREC_DOT : FUNCTION_CALL DOT FUNCTION_CALL
         : FUNCTION_CALL

PREC_0 : FUNCTION_CALL POW FUNCTION_CALL
       : FUNCTION_CALL

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

PREC_8   : let IDENTIFIER EQ PREC_7
         : PREC_7

PREC_9 : return PREC_8
       : PREC_8

PREC_IF : if PREC_TOP CODE_BLOCK else (if PREC_TOP) CODE_BLOCK
        : PREC_TOP

PREC_WHILE : while condition CODE_BLOCK
           : PREC_IF

PREC_FUNCTION : FUNCTION IDENTIFIER LPAREN IDENTIFIER RPAREN FUNCTION_BLOCK
              : FUNCTION IDENTIFIER LPAREN RPAREN FUNCTION_BLOCK
              : PREC_WHILE

FIELD_DECL : field IDENTIFIER EQ PREC_7

PREC_METHOD : FUNCTION IDENTIFIER LPAREN (IDENTIFIER) RPAREN FUNCTION_BLOCK
            : IDENTIFIER LPAREN (IDENTIFIER) RPAREN FUNCTION_BLOCK

OBJECT_BODY : { FIELD_DECL | PREC_METHOD ... }

PREC_OBJECT : object IDENTIFIER OBJECT_BODY

code_block : { PREC_FUNCTION ... }
           : PREC_FUNCTION EOL | EOF
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
    // std::cout << currentToken.to_string() << std::endl;
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

  int isShortHand() {
    for(int i = 0; i < sizeof(short_hand_ops) / sizeof(short_hand_ops[0]); ++i) {
      if (short_hand_ops[i].name == currentToken.get_name())
        return i;
    }

    return -1;
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
    if (currentToken.get_name() == KEY_LBRACKET) {
      std::vector<node *> elements;
      advance();
      while (currentToken.get_name() != KEY_RBRACKET) {
        auto _res = parsePrec8();
        if (_res.second.has_error()) return std::make_pair(new node(), _res.second);
        elements.emplace_back(_res.first);

        if (currentToken.get_name() == SEP_COMMA)
          advance();
      }
      advance();
      return {new node{currentToken, node_type::_list, .children = elements}, Error()};
    }

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

    if (currentToken.get_value()._type == types::_string) {
      node *_node = new node{currentToken, node_type::_str, nullptr, nullptr};
      advance();
      return std::make_pair(_node, Error());
    }

    if (currentToken.get_name() == KEY_LPAREN) {
      advance();
      auto expr = parsePrec8();
      if (expr.second.has_error()) return std::make_pair(new node(), expr.second);
      if (currentToken.get_name() != KEY_RPAREN) {
        return std::make_pair(new node(), Error("Invalid Syntax Error", "Expected ')'"));
      }
      advance();
      return expr;
    }

    if (currentToken.get_value()._type == types::_id) {
      auto id = currentToken;
      advance();
      if (currentToken.get_name() == KEY_LBRACKET) {
        advance();
        auto _index = parsePrec8();
        if (_index.second.has_error()) return std::make_pair(new node(), _index.second);
        if (currentToken.get_name() != KEY_RBRACKET) {
          return std::make_pair(new node(), Error("Invalid Syntax Error", "Expected ']'"));
        }
        advance();
        node *_node = new node{id, node_type::_index, _index.first, nullptr};
        return std::make_pair(_node, Error());
      }
      node *_node = new node{id, node_type::_variable_access, nullptr, nullptr};
      return std::make_pair(_node, Error());
    }

    return std::make_pair(new node(), Error("Invalid Syntax Error", "Expected Number"));
  }
  
  std::pair<node *, Error> parseFunctionCall() {
    if (currentToken.get_value()._type == types::_id) {
      auto identifier = currentToken;
      advance();
      if (currentToken.get_name() != KEY_LPAREN) {
        reverse();
        return parseAtom();
      }
      advance();
      std::vector<node *> args;
      while (currentToken.get_name() != KEY_RPAREN) {
        auto arg = parsePrec7();
        if (arg.second.has_error()) return std::make_pair(new node(), arg.second);
        args.push_back(arg.first);
        if (currentToken.get_name() == SEP_COMMA) {
          advance();
        }
      }
      advance();
      return std::make_pair(new node{identifier, node_type::_function_call, .children = args}, Error());
    }
    return parseAtom();
  }

  std::pair<node *, Error> parseDot() {
    auto expr = parseFunctionCall();
    if (expr.second.has_error()) return std::make_pair(new node(), expr.second);
    if (currentToken.get_name() == KEY_DOT) {
      auto id = currentToken;
      advance();
      auto expr2 = parseFunctionCall();
      if (expr2.second.has_error()) return std::make_pair(new node(), expr2.second);
      node *_node = new node{id, node_type::_dot, expr.first, expr2.first};
      return std::make_pair(_node, Error());
    }
    return expr;
  }

  std::pair<node *, Error> parsePrec0() {
    auto left = parseDot();
    if (left.second.has_error()) return std::make_pair(new node(), left.second);
    if (currentToken.get_name() == KEY_POW) {
      auto op = currentToken;
      advance();
      auto right = parseDot();
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

  std::pair<node *, Error> parsePrec8() {
    if (currentToken.get_value()._type == types::_keyword && (currentToken.get_name() == KEY_ASSIGN || currentToken.get_name() == KEY_ASSIGN_CONST)) {
      bool isConst = currentToken.get_name() == KEY_ASSIGN_CONST;
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
      auto expr = parsePrec7();
      if (expr.second.has_error()) return std::make_pair(new node(), expr.second);

      // if (currentToken.get_name() != KEY_NEWLINE && currentToken.get_name() != KEY_EOF) {
      //   return std::make_pair(new node(), Error("Invalid Syntax Error", "Expected new line"));
      // }

      return std::make_pair(new node{identifier, node_type::_variable_assign, expr.first, .isConst = isConst}, Error());
    }

    if (currentToken.get_value()._type == types::_id) {
      auto identifier = currentToken;
      advance();
      int shortHand = isShortHand();
      if (shortHand != -1) {
        advance();
        auto expr = parsePrec7();
        if (expr.second.has_error()) return {new node(), expr.second};
        return {
          new node{
            identifier,
            node_type::_variable_reassign,
            new node{
              Token(short_hand_ops[shortHand].binary_op_name, CursorPosition(), CursorPosition()),
              node_type::_binary_operator,
              new node{
                identifier,
                node_type::_variable_access,
              },
              expr.first
            },
            nullptr
          },
          Error()
        };
      }
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

  std::pair<node *, Error> parsePrec9() {
    if (currentToken.get_name() == KEY_RETURN) {
      auto op_tok = currentToken;
      advance();
      auto expr = parsePrec8();
      if (expr.second.has_error()) return {new node(), expr.second};
      return {new node{op_tok, node_type::_return, expr.first}, Error()};
    }

    return parsePrec8();
  }

  std::pair<node *, Error> parsePrecIf() {
    if (currentToken.get_value()._type == types::_keyword && currentToken.get_name() == KEY_IF) {
      auto op_tok = currentToken;
      advance();
      std::vector<node *> elifs;
      auto cond = parsePrec9();
      if (cond.second.has_error()) return {new node(), cond.second};
      auto code_block = parseCodeBlock();
      if (code_block.second.has_error()) return {new node(), code_block.second};
      
      // while (currentToken.get_name() == KEY_NEWLINE) advance();
      if (currentToken.get_name() == KEY_EOF) 
        return {new node{op_tok, node_type::_if_else, cond.first, code_block.first}, Error()};
      
      while (currentToken.get_value()._type == types::_keyword && currentToken.get_name() == KEY_ELIF) {
        auto _elif = currentToken;
        advance();
        auto _cond = parsePrec9();
        if (_cond.second.has_error()) return {new node(), _cond.second};
        auto _code_block = parseCodeBlock();
        if (_code_block.second.has_error()) return {new node(), _code_block.second};
        elifs.emplace_back(new node{_elif, node_type::_if_else, _cond.first, _code_block.first});
      }

      // while (currentToken.get_name() == KEY_NEWLINE) advance();
      if (currentToken.get_name() == KEY_EOF) 
        return {new node{op_tok, node_type::_if_else, cond.first, code_block.first, elifs}, Error()};

      if (currentToken.get_value()._type == types::_keyword && currentToken.get_name() == KEY_ELSE) {
        auto _else = currentToken;
        advance();
        auto _code_block = parseCodeBlock();
        if (_code_block.second.has_error()) return {new node(), _code_block.second};
        elifs.emplace_back(new node{_else, node_type::_if_else, nullptr, _code_block.first});
      }
      return {new node{op_tok, node_type::_if_else, cond.first, code_block.first, elifs}, Error()};
    }
    return parsePrec9();
  }

  std::pair<node *, Error> parsePrecWhile() {
    if (currentToken.get_value()._type == types::_keyword && currentToken.get_name() == KEY_WHILE) {
      auto op_tok = currentToken;
      advance();
      auto cond = parsePrec9();
      if (cond.second.has_error()) return {new node(), cond.second};
      auto code_block = parseCodeBlock();
      if (code_block.second.has_error()) return {new node(), code_block.second};
      return {new node{op_tok, node_type::_while_loop, cond.first, code_block.first}, Error()};
    }

    return parsePrecIf();
  }

  std::pair<node *, Error> parsePrecFunction() {
    if (currentToken.get_value()._type == types::_keyword && currentToken.get_name() == KEY_FUNCTION) {
      advance();
      if (currentToken.get_value()._type != types::_id) {
        return std::make_pair(new node(), Error("Invalid Syntax Error", "Expected identifier"));
      }

      auto identifier = currentToken;
      advance();
      if (currentToken.get_name() != KEY_LPAREN) {
        return std::make_pair(new node(), Error("Invalid Syntax Error", "Expected '('"));
      }
      
      advance();
      std::vector<Token> args;
      while (currentToken.get_name() != KEY_RPAREN) {
        if (currentToken.get_value()._type != types::_id) {
          return std::make_pair(new node(), Error("Invalid Syntax Error", "Expected identifier"));
        }
        args.push_back(currentToken);
        advance();
        if (currentToken.get_name() == SEP_COMMA) {
          advance();
          continue;
        }
      }
      advance();
      auto code_block = parseFunctionBlock();
      if (code_block.second.has_error()) return std::make_pair(new node(), code_block.second);
      return std::make_pair(new node{identifier, node_type::_function_definition, code_block.first, .args = args}, Error());
    }

    return parsePrecWhile();
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
        auto expr = parsePrecFunction();
        if (expr.second.has_error()) return std::make_pair(new node(), expr.second);
        nodes.push_back(expr.first);
      }
      advance();
      return std::make_pair(new node{Token(KEY_CODE_BLOCK, CursorPosition(), CursorPosition()), node_type::_code_block, nullptr, nullptr, nodes}, Error());
    }
    if (currentToken.get_name() == KEY_OBJECT) {
      advance();
      if (currentToken.get_value()._type != types::_id)
        return {new node(), Error("Invalid Syntax Error", "Expected identifier after keyword 'object'")};
      auto identifier = currentToken;
      advance();
      auto body = parseObjectBody();
      if (body.second.has_error()) return {new node(), body.second};
      return {new node{identifier, node_type::_object_definition, body.first}, Error()};
    }

    return parsePrecFunction();
  }

  std::pair<node *, Error> parseFunctionBlock() {
    auto code_block = parseCodeBlock();
    if (code_block.second.has_error()) return {new node(), code_block.second};
    if (code_block.first->value == node_type::_code_block) {
      code_block.first->value = node_type::_function_block;
    }
    
    return code_block;
  }

  std::pair<node *, Error> precField() {
    if (currentToken.get_value()._type == types::_keyword && currentToken.get_name() == KEY_FIELD) {
      advance();
      if (currentToken.get_value()._type != types::_id) 
        return {new node(), Error("Invalid Syntax Error", "Expected identifier after 'field'")};
      
      auto identifier = currentToken;
      advance();

      if (currentToken.get_name() == KEY_EQ) {
        advance();
        auto expr = parsePrec7();
        if (expr.second.has_error()) return {new node(), expr.second};

        return {new node{identifier, node_type::_field_assign, expr.first}, Error()};
      }

      return {new node{identifier, node_type::_field_assign, nullptr}, Error()};
    }

    return {new node(), Error("Invalid Syntax Error", "Expected keyword 'field' before declaration")};
  }

  std::pair<node *, Error> precMethod() {
    if (currentToken.get_name() == KEY_CONSTRUCTOR || currentToken.get_name() == KEY_FUNCTION) {
      if (currentToken.get_name() == KEY_FUNCTION) {
        advance();
        if (currentToken.get_value()._type != types::_id) {
          return std::make_pair(new node(), Error("Invalid Syntax Error", "Expected identifier"));
        }
      }
      auto identifier = currentToken;
      advance();
      if (currentToken.get_name() != KEY_LPAREN) {
        return std::make_pair(new node(), Error("Invalid Syntax Error", "Expected '('"));
      }
      
      advance();
      std::vector<Token> args;
      while (currentToken.get_name() != KEY_RPAREN) {
        if (currentToken.get_value()._type != types::_id) {
          return std::make_pair(new node(), Error("Invalid Syntax Error", "Expected identifier"));
        }
        args.push_back(currentToken);
        advance();
        if (currentToken.get_name() == SEP_COMMA) {
          advance();
          continue;
        }
      }
      advance();
      auto code_block = parseFunctionBlock();
      if (code_block.second.has_error()) return std::make_pair(new node(), code_block.second);
      return std::make_pair(new node{identifier, node_type::_method_call, code_block.first, .args = args}, Error());
    }
    return {new node(), Error("Invalid Syntax Error", "Unidentifier default method " + currentToken.get_name())};
  }

  std::pair<node *, Error> parseObjectBody() {
    if (currentToken.get_name() != KEY_LBRACE) {
      return {new node(), Error("Invalid Syntax Error", "Expected '{' after object declaration")};
    }
    advance();
    std::vector<node *> children;
    while (currentToken.get_name() != KEY_RBRACE) {
      if (currentToken.get_name() == KEY_FIELD) {
        auto _f = precField();
        if (_f.second.has_error()) return {new node(), _f.second};
        children.push_back(_f.first);
      }
      else if (currentToken.get_name() == KEY_FUNCTION || currentToken.get_name() == KEY_CONSTRUCTOR) {
        auto _f = precMethod();
        if (_f.second.has_error()) return {new node(), _f.second};
        children.push_back(_f.first);
      }
      else if (currentToken.get_name() == KEY_NEWLINE) {
        advance();
      }
      else 
        return {new node(), Error("Invalid Syntax Error", "Unexpected Token " + currentToken.get_name() + " found")};
    }
    advance();
    return {new node{Token(KEY_OBJECT_BODY, CursorPosition(), CursorPosition()), node_type::_object_body, .children = children}, Error()};
  }
};

#endif