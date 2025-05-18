#ifndef LEXICAL_ENGINE_H
#define LEXICAL_ENGINE_H
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cctype>
using namespace std;

namespace Avengers {
    enum class LexicalUnit
    {
        identifier,
        keyword,
        integer_literal,
        float_literal,
        Operator,
        delimiter,
        string_literal,
        char_literal,
        end_of_file,
        unknown

    };

    struct Token {
        LexicalUnit kind;
        string lexeme; // store the actual value from the source code that was matched
        //track position of token (used in error handing)
        int line;//line number where token is appears
        int column;//col number (character position) on that line.
        Token(LexicalUnit t, const std::string &lex, int ln, int col)
        :kind(t) , lexeme(lex) , line(ln) , column(col) {}
    };
    //Heart of lexical analysis: now we read input c code and genrate tokens one by one 
    class LexicalStream{
        public:
            explicit LexicalStream(const string &codeInput);
            Token nextToken();
            void reset();//Useful for Moving cursor back to the start
            vector<Token> TokenizeAll();//return a list of token (store all tokens in vector)

        private://helper methods --> internal used functions
            string buffer;
            size_t cursor;
            int currentLine;
            int currentColumn;
            void skipWhitespaceAndComments();
            char nextChar() const;//return the current character without moving the cursor, i.e. use for lookahead decisions
            char getChar();//returns current character and advances the cursor.
            bool isAtEnd() const;//check that cursor reached at end or not 
            Token parseIdentifierOrKeyword();//It uses isKeyword() to distinguish the two.
            Token parseNumberLiteral();//Detects digits, decimals
            Token parseOperatorOrSymbol();//Uses character-matching logic.
            Token parseStringOrCharLiteral();//for char literals and string literals
            void Error(const string& msg, int line, int col) const;

            static bool is_c_keyword(const string& word);//check input token is keyword or not
    };
}
#endif
