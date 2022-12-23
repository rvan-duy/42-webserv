#pragma once
#include <string.h>

#include <Token.hpp>
#include <vector>

class Lexer {
 public:
  static std::vector<Token> tokenizeFile(std::ifstream &file);

 private:
  // Not meant to be initialized
  Lexer(){};
  ~Lexer(){};

  // Methods
  static Token::ETokenType _getType(int c);
  static void _parseLine(std::vector<Token> *tokens, std::string &line);
  static std::string _parseWord(std::string::iterator it);
  static bool _isSpecialChar(int c);
};
