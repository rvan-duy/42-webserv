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

	private:
		std::string	_word;
		ETokenType	_type;

		ETokenType	getType() const;
		std::string getWord() const;
};
