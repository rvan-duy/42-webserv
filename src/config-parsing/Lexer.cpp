#include <Lexer.hpp>
#include <Logger.hpp>

std::vector<Token> Lexer::tokenizeFile(std::ifstream &file) {
  std::vector<Token> tokens;
  Logger &logger = Logger::getInstance();
  if (!file.is_open()) {
    logger.error("Attempting to tokenize file that isn't open");
    return tokens;
  }
  logger.log("Starting tokenizing file", VERBOSE);

  /**
   * Parses file line by line
   */
  std::string line;
  while (std::getline(file, line)) {
    _parseLine(&tokens, line);
  }
  logger.log("File successfully lexed", VERBOSE);
  return tokens;
}

void Lexer::_parseLine(std::vector<Token> *tokens, std::string &line) {
  std::string::iterator it = line.begin();
  std::string word("");
  Token::ETokenType type;

  while (it != line.end()) {
    while (isspace(*it)) it++;
    if (!*it) {
      return;
    }
    type = _getType(*it);
    if (type == Token::WORD) {
      word = _parseWord(it);
      tokens->push_back(Token(word));
      it += word.length();
      word = "";
    } else {
      tokens->push_back(Token(type));
      it++;
    }
  }
}

std::string Lexer::_parseWord(std::string::iterator it) {
  std::string word("");

  while (*it && !isspace(*it) && !_isSpecialChar(*it)) {
    word += *it;
    it++;
  }
  return word;
}

Token::ETokenType Lexer::_getType(int c) {
  switch (c) {
    case ';':
      return Token::SEMICOLON;
    case '{':
      return Token::OPEN_CURL;
    case '}':
      return Token::CLOSE_CURL;
    default:
      return Token::WORD;
  }
}

bool Lexer::_isSpecialChar(int c) {
  if (c == ';' || c == '{' || c == '}') {
    return true;
  }
  return false;
}
