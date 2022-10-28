#include <string>

enum ETokenType {
	WORD,
	SEMICOLON,
	OPEN_CURL,
	CLOSE_CURL
};

class Token {
	public:
		Token(ETokenType type);
		Token(std::string word);

		ETokenType	getType() const;
		std::string getWord() const;

	private:
		std::string	_word;
		ETokenType	_type;
};
