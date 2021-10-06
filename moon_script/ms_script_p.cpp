
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

// int peekNextChar() {
//     int c;

//     if((c=fin.peek()) != EOF) fin.close();

//     return c;
// }

TokenKind getOtherKind(string str) {
    if(str == "if") // if
        return If;

    if(str == "else") // else
        return Else;

    if(str == "while") // while
        return While;

    if(str == "for") // for
        return For;

    if(str == "function") // function
        return Function;

    if(str == "+") // +
        return Plus;

    if(str == "-") // -
        return Minus;

    if(str == "*") // *
        return Multi;

    if(str == "/") // /
        return Divi;

    if(str == "==") // ==
        return Equal;

    if(str == "!=") // !=
        return NotEqual;

    if(str == "<=") // <=
        return RightEqual;

    if(str == ">=") // >=
        return LeftEqual;

    return Other;
}

string getKindName(TokenKind kind) {
    switch(kind) {
        case If : // if
            return "if";

        case Else : // else
            return "else";

        case While : // while
            return "while";

        case For : // for
            return "for";

        case Function : // function
            return "function";

        case Int : // int
            return "int";

        case String : // string
            return "string";

        case Plus : // +
            return "plus";

        case Minus : // -
            return "minus";

        case Multi : // *
            return "multi";

        case Divi : // /
            return "divi";

        case Equal : // ==
            return "equal";

        case NotEqual : // !=
            return "not equal";

        case RightEqual : // <=
            return "right equal";

        case LeftEqual : // >=
            return "left equal";

        default :
            return "other";
    }
}

Token getNextToken() {
    static int c = getNextChar();
    int num = 0;
    string str = "";

    while(isspace(c)) c = getNextChar();
    
    // cout << "char : " << (char) c << "\n";

    if(c == EOF) {
        return Token(_EOF);

    // int
    } else if(isdigit(c)) {
        num = c - '0';

        while(isdigit(c=getNextChar())) num = num * 10 + (c - '0');

        return Token(Int, num);

    // string
    } else if(c == '"') {
        while((c=getNextChar()) != '"') str += c;

        c = getNextChar();

        return Token(String, str);

    // varibale | statement
    } else {
        str += c;

        while(!isspace(c=getNextChar()) && !isdigit(c) && c != '"') str += c;
    }

    return Token(getOtherKind(str), str);
}

int main() {
    fin.open("ms_script_p.txt");

    if(!fin) exit(1);

    Token token;

    while((token=getNextToken()).token_kind != _EOF) {
        cout << getKindName(token.token_kind) << "\n";
    }

    return 0;
}