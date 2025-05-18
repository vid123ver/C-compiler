
#include "../include/lexer.h"
#include <iostream>
#include <sstream>//functionality of string streams

namespace Avengers{
    const unordered_map<string, bool> c_keywords = {
        {"auto", true}, {"break", true}, {"case", true}, {"char", true}, {"const", true},
        {"continue", true}, {"default", true}, {"do", true}, {"double", true}, {"else", true},
        {"enum", true}, {"extern", true}, {"float", true}, {"for", true}, {"goto", true},
        {"if", true}, {"int", true}, {"long", true}, {"register", true}, {"return", true},
        {"short", true}, {"signed", true}, {"sizeof", true}, {"static", true}, {"struct", true},
        {"switch", true}, {"typedef", true}, {"union", true}, {"unsigned", true}, {"void", true},
        {"volatile", true}, {"while", true}
    };
    //now we write constructor to initilize the values 
    LexicalStream::LexicalStream(const string& codeInput)
        :buffer(codeInput) , cursor(0) , currentLine(1) , currentColumn(1){}
    
    void LexicalStream::reset() {
        cursor = 0;
        currentLine = 1;
        currentColumn = 1;
    }

    bool LexicalStream::isAtEnd() const {
        if(cursor >= buffer.size()){
            return true;
        }
        return false;
    }
    //function of getChar:
    // 1.	looks at the current character in the input buffer
	// 2.	does not move the cursor forward
    //inshot this is only use for checking purpuse in if statement not when you need to increase cursor
    char LexicalStream::nextChar() const {
        return isAtEnd() ? '\0' : buffer[cursor];
    }
    //function of getChar (imp):
	// 1.	returns the current character in the input buffer
	// 2.	moves the cursor forward by one position (so next time we read next character)
    char LexicalStream::getChar() {
        if (isAtEnd()) return '\0';
        char c = buffer[cursor++];//increase cursor so that it points next to current character
        if (c == '\n') {
            currentLine++;
            currentColumn = 1;
        } else {
            currentColumn++;
        }
        return c;
    }

    void LexicalStream::skipWhitespaceAndComments() {
        while (!isAtEnd()) {
            char c = nextChar();
            if (isspace(c)) {
                getChar();
            } else if (c == '/' && buffer[cursor + 1] == '/') {
                while (!isAtEnd() && getChar() != '\n');
            } else if (c == '/' && buffer[cursor + 1] == '*') {
                getChar(); //consume --> /
                getChar(); //consume --> *
                while (!isAtEnd()) {
                    if (nextChar() == '*' && buffer[cursor + 1] == '/') {
                        getChar(); //consume --> /
                        getChar(); //consume --> *
                        break;//now comment is over and get out of loop
                    }
                    getChar();//if this above "if" statement not used then consume the comments portion 
                }
            } else {
                break;
            }
        }
    }

    bool LexicalStream::is_c_keyword(const string& word){
        return c_keywords.find(word) != c_keywords.end();
    }
    //Its job is to detect and return tokens that may be : identifier or keyword
    Token LexicalStream::parseIdentifierOrKeyword(){
        int start_column = currentColumn;
        string lexeme;//declarint a empty string to store the words like main, if , while , for 
        //loop until you build a full word (into lexeme)
        while(!isAtEnd() && (isalnum(nextChar()) || nextChar() == '_')){
            lexeme += getChar();
        }
        LexicalUnit type = is_c_keyword(lexeme) ? LexicalUnit::keyword : LexicalUnit::identifier;
        return Token(type, lexeme, currentLine, start_column);
    }

    Token LexicalStream::parseNumberLiteral(){
        int start_column = currentColumn;
        string lexeme;//declarint a empty string to store the words numbers
        bool is_float = false;
        // loop until you build a full number (into lexeme)
        while(!isAtEnd() && isdigit(nextChar())){
            lexeme += getChar();
        }
        if (!isAtEnd() && nextChar() == '.') {
            is_float = true;
            lexeme += getChar();
        }
        LexicalUnit type = is_float ? LexicalUnit::float_literal : LexicalUnit::integer_literal;
        return Token(type, lexeme , currentLine , start_column);
    }

    Token LexicalStream::parseOperatorOrSymbol(){
        int start_col = currentColumn;
        char c = getChar();
        string lexeme(1, c);
        //handle == , =! , <= , >=
        if((c == '=' || c == '!' || c == '<' || c == '>') && nextChar() == '='){
            lexeme += getChar();
        }
        //handle ++ , -- , || , &&
        else if((c == '+' || c == '-' || c == '&' || c == '|') && nextChar() == c){
            lexeme += getChar();
        }
        return Token(LexicalUnit::Operator ,lexeme ,currentLine , start_col);
    }

    Token LexicalStream::parseStringOrCharLiteral()         {
        int startCol = currentColumn;
        char quote = getChar();  // either ' or "
        string lexeme(1, quote);
        bool isChar = (quote == '\'');
        
        while (!isAtEnd()) {
            char c = getChar();
            lexeme += c;
        
            if (c == quote) break;
        
            if (c == '\\' && !isAtEnd()) {  // handle escape sequence
                lexeme += getChar();
            }
        }

        if (lexeme.back() != quote) {
            Error("Unterminated string/char literal", currentLine, startCol);
        }

        if (isChar && lexeme.length() > 4) {
            Error("Invalid character literal", currentLine, startCol);
        }

        LexicalUnit type = isChar ? LexicalUnit::char_literal : LexicalUnit::string_literal;
        return Token(type , lexeme , currentLine , startCol);
    }

    Token LexicalStream::nextToken(){
        skipWhitespaceAndComments();
        if(isAtEnd()){
            return Token(LexicalUnit::end_of_file, "", currentLine, currentColumn);
        }
        char ch = nextChar();
        if(isalpha(ch) || ch == '_'){
            return parseIdentifierOrKeyword();
        }
        if(isdigit(ch)){
            return parseNumberLiteral();
        }
        if (ch == '\'' || ch == '\"'){
            return parseStringOrCharLiteral();
        } 
        return parseOperatorOrSymbol();
    }

    vector<Token> LexicalStream::TokenizeAll(){
        vector<Token> tokens;
        while(true){
            Token tok = nextToken();
            tokens.push_back(tok);
            if(tok.kind == LexicalUnit::end_of_file) {
                break;
            }
        }
        return tokens;
    }
    void LexicalStream::Error(const string& msg, int line, int col) const {
        cerr << "Error at line " << line << ", column " << col << ": " << msg << endl;
    }
}

