#include "../includes.h"

#ifndef KEYWORDS_H
#define KEYWORDS_H

const std::string KEY_FLOAT = "FLOAT";
const std::string KEY_INT = "INT";


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

const std::string KEY_LE = "LE";
const std::string KEY_LT = "LT";
const std::string KEY_GE = "GE";
const std::string KEY_GT = "GT";
const std::string KEY_EE = "EE";
const std::string KEY_NE = "NE";
const std::string KEY_NOT = "NOT";

const std::string KEY_ASSIGN = "let";
const std::string KEY_TRUE = "true";
const std::string KEY_FALSE = "false";
const std::string KEY_NULL = "null";

const std::string keywords[] = {
  KEY_ASSIGN,
  KEY_TRUE,
  KEY_FALSE,
  KEY_NULL
};

#endif // KEYWORDS_H
