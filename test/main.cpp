
#include "../include/lexer.h"
using namespace Avengers;
using namespace std;

int main() {
    string testCode = R"(
        int main() {
            int a = 5;
            float b = 3.14;
            char c = 'x';
            printf("Hello, World!\n");
            return 0;
        }
    )"; 

    LexicalStream lexer(testCode);  
    vector<Token> tokens = lexer.TokenizeAll(); 

    for (const Token& token : tokens) {
        cout << "Token: " << token.lexeme << " Type: ";
        switch (token.kind) {
            case LexicalUnit::identifier: cout << "Identifier"; break;
            case LexicalUnit::keyword: cout << "Keyword"; break;
            case LexicalUnit::integer_literal: cout << "Integer Literal"; break;
            case LexicalUnit::float_literal: cout << "Float Literal"; break;
            case LexicalUnit::Operator: cout << "Operator"; break;
            case LexicalUnit::delimiter: cout << "Delimiter"; break;
            case LexicalUnit::string_literal: cout << "String Literal"; break;
            case LexicalUnit::char_literal: cout << "Char Literal"; break;
            case LexicalUnit::end_of_file: cout << "EOF"; break;
            default: cout << "Unknown"; break;
        }
        cout << " at Line: " << token.line << " Column: " << token.column << endl;
    }

    return 0;
}