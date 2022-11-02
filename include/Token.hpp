#pragma once
#include <string>

class Token {
	public:
		enum ETokenType {
			WORD,
			SEMICOLON,
			OPEN_CURL,
			CLOSE_CURL
		};

		Token(Token::ETokenType type);
		Token(std::string word);
		~Token();

		Token::ETokenType	&getType();
		std::string 		&getWord();
		
		bool	isWordEqualTo(std::string const& value) const;
		bool	isType(Token::ETokenType const& type) const;

	private:
		std::string	_word;
		ETokenType	_type;
};
