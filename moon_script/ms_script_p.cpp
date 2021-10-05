
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

ifstream fin;

enum TokenKind {
    If, // if
    Else, // else
    While, // while
    For, // for
    Function, // function
    Int, // int
    String, // string
    Plus, // +
    Minus, // -
    Multi, // *
    Divi, // /
    Equal, // ==
    NotEqual, // !=
    RightEqual, // <=
    LeftEqual, // >=
    Other,
    _EOF
};

struct Token {
    TokenKind token_kind;

    int int_value;
    string string_value;

    Token() {
        token_kind = Other;
        int_value = 0;
        string_value = "";
    }

    Token(TokenKind kind) {
        token_kind = kind;
    }

    Token(TokenKind kind, int value) {
        token_kind = kind;
        int_value = value;
    }

    Token(TokenKind kind, string value) {
        token_kind = kind;
        string_value = value;
    }
};

int getNextChar() {
    int c;
    
    if((c=fin.get()) == EOF) fin.close();

    return c;
}

TokenKind getKind(string str) {
    if(str == "if") return If;
    if(str == "else") return Else;
    if(str == "while") return While;
    if(str == "for") return For;
    if(str == "function") return Function;
}

Token getNextToken() {
    int c = ' ';
    int num = 0;
    string str = "";

    while(isspace(c)) c = getNextChar();

    if(c == EOF) {
        return Token(_EOF);

    // int
    } else if(isdigit(c)) {
        while(isdigit(c)) {
            num = num * 10 + (c - '0');
            c = getNextChar();
        }

        return Token(Int, num);

    // string
    } else if(c == '"') {
        while(c != '"') {
            str += c;
            c = getNextChar();
        }

        return Token(String, str);

    // varibale | statement
    } else if(isalpha(c)) {
        while(isalpha(c)) {
            str += c;
            c = getNextChar();
        }
    }

    return Token(getKind(str), str);
}

int main() {
    return 0;
}