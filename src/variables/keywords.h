#include "../includes.h"

#ifndef KEYWORDS_H
#define KEYWORDS_H

const std::string KEY_FLOAT = "FLOAT";
const std::string KEY_INT = "INT";
const std::string KEY_STRING = "STRING";


const std::string KEY_ADD = "ADD";
const std::string KEY_SUB = "SUB";
const std::string KEY_MUL = "MUL";
const std::string KEY_DIV = "DIV";
const std::string KEY_MOD = "MOD";
const std::string KEY_POW = "POW";
const std::string KEY_EQ = "EQ";
const std::string KEY_LPAREN = "LPAREN";
const std::string KEY_RPAREN = "RPAREN";
const std::string KEY_LBRACE = "LBRACE";
const std::string KEY_RBRACE = "RBRACE";
const std::string KEY_CODE_BLOCK = "CODE_BLOCK";
const std::string KEY_NEWLINE = "NEWLINE";
const std::string KEY_EOF = "EOF";

const std::string KEY_PLUS_EQUALS = "PLUS_EQUALS";
const std::string KEY_MINUS_EQUALS = "MINUS_EQUALS";
const std::string KEY_MUL_EQUALS = "MUL_EQUALS";
const std::string KEY_DIV_EQUALS = "DIV_EQUALS";
const std::string KEY_MOD_EQUALS = "MOD_EQUALS";
const std::string KEY_POW_EQUALS = "POW_EQUALS";

const std::string KEY_LE = "LE";
const std::string KEY_LT = "LT";
const std::string KEY_GE = "GE";
const std::string KEY_GT = "GT";
const std::string KEY_EE = "EE";
const std::string KEY_NE = "NE";
const std::string KEY_NOT = "not";

const std::string KEY_AND = "and";
const std::string KEY_OR = "or";

const std::string KEY_ASSIGN = "let";
const std::string KEY_MUT = "mut";
const std::string KEY_TRUE = "true";
const std::string KEY_FALSE = "false";
const std::string KEY_NULL = "null";
const std::string KEY_IF = "if";
const std::string KEY_ELSE = "else";
const std::string KEY_ELIF = "elif";
const std::string KEY_WHILE = "while";
const std::string KEY_FUNCTION = "fn";
const std::string KEY_RETURN = "return";

const std::string SEP_COMMA = "COMMA";

const std::string keywords[] = {
  KEY_ASSIGN,
  KEY_MUT,
  KEY_TRUE,
  KEY_FALSE,
  KEY_NULL,
  KEY_AND,
  KEY_OR,
  KEY_NOT,
  KEY_IF,
  KEY_ELIF,
  KEY_ELSE,
  KEY_WHILE,
  KEY_FUNCTION,
  KEY_RETURN,
};

#endif // KEYWORDS_H
