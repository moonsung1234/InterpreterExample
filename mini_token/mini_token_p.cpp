
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

enum TokenKind {
    LParen = 1, // (
    RParen, // )
    Plus, // +
    Minus, // -
    Multi, // *
    Divi, // /
    Assign, // =
    Equal, // ==
    NotEqual, // !=
    Less, // <
    LessEqual, // <=
    Great, // >
    GreatEqual, // >=
    Comma, // ,
    Letter,
    DblQ,
    String,
    Digit,
    Number,
    If, // if
    Else, // else
    Print, // print
    End, // end
    _EOF,
    End_KeyWord_list,
    Other
};

TokenKind token_list[256]; // ascii size : 256
ifstream fin;

struct Token {
    TokenKind token_kind;
    string text;
    int int_value;
    Token() {
        token_kind = Other;
        text = "";
        int_value = 0;
    }
    Token(TokenKind _token, string _text, int _int_value=0) {
        token_kind = _token;
        text = _text;
        int_value = _int_value;
    }
};

struct KeyWord {
    string key_name;
    TokenKind token;
};

KeyWord KeyWord_list[] = {
    { "if", If },
    { "else", Else },
    { "print", Print },
    { "end", End },
    { "==", Equal },
    { "!=", NotEqual },
    { "<=", LessEqual },
    { ">=", GreatEqual },
    { "", End_KeyWord_list }
};

void initTokenList() {
    for(int i=0; i<256; i++) token_list[i] = Other;
    for(int i='0'; i<='9'; i++) token_list[i] = Digit; // '0' ~ '9'
    for(int i='a'; i<='z'; i++) token_list[i] = Letter; // 'a' ~ 'z'
    for(int i='A'; i<='Z'; i++) token_list[i] = Letter; // 'A' ~ 'Z'

    // else
    token_list['('] = LParen;
    token_list[')'] = RParen;
    token_list['+'] = Plus;
    token_list['-'] = Minus;
    token_list['*'] = Multi;
    token_list['/'] = Divi;
    token_list['<'] = Less;
    token_list['>'] = Great;
    token_list['='] = Assign;
    token_list[','] = Comma;
    token_list['"'] = DblQ;
}

int getNextChar() {
    int c;
    
    if((c=fin.get()) == EOF) fin.close();

    return c;
}

TokenKind getKind(string str) {
    for(int i=0; KeyWord_list[i].token != End_KeyWord_list; i++) {
        if(str == KeyWord_list[i].key_name) 
            return KeyWord_list[i].token; 
    }

    return Other;
}

Token getNextToken() {
    int c = ' ';
    int num = 0;
    string str = "";

    while(isspace(c)) c = getNextChar();
    if(c == EOF) return Token(_EOF, "");

    switch(token_list[c]) {
        case Letter :
            for(; token_list[c] == Letter; c=getNextChar())
                str += c;

            break;

        case DblQ :
            for(c=getNextChar(); c != '"'; c=getNextChar())
                str += c;

            getNextChar(); // flush

            return Token(String, str);

            break;

        case Digit :
            for(num=0; token_list[c] == Digit; c=getNextChar())
                num = num * 10 + (c - '0');

            return Token(Number, "", num);

            break;

        default:
            for(; token_list[c] != Letter 
                && token_list[c] != DblQ 
                && token_list[c] != Digit 
                && !isspace(c); c=getNextChar())
                str += c;

            break;
    }

    TokenKind kind = getKind(str);

    return Token(kind, str);
}

int main(int argc, char *argv[]) {
    // file open
    if(argc == 1) exit(1);
    fin.open(argv[1]);
    if(!fin) exit(1);

    initTokenList();

    Token token;

    for(token=getNextToken(); token.token_kind != _EOF; token=getNextToken())
        cout 
        << "kind : " << token.token_kind
        << "    "
        << "text : " << token.text
        << "    "
        << "int : " << token.int_value
        << "\n";

    return 0;
}