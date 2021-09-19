
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

ifstream fin;

int variable[100];

int stack[1000];
int stack_index = 0;

enum TokenKind {
    Multi,
    Divi,
    Plus,
    Minus,
    Lparen,
    Rparen,
    Assign,
    Number,
    Variable,
    Print,
    Other,
    _EOF
};

struct Token {
    TokenKind token_kind;
    int int_value;
    
    Token() {
        token_kind = Other;
        int_value = 0;
    }
    Token(TokenKind tk, int iv=0) {
        token_kind = tk;
        int_value = iv;
    }
};

Token next_token;

TokenKind getKind(char c);
int getNextChat();
Token getNextToken();
void operate(TokenKind kind);
void factor();
void term();
void expression();
void statement();

TokenKind getKind(char c) {
    if(c == '*') return Multi;
    if(c == '/') return Divi;
    if(c == '+') return Plus;
    if(c == '-') return Minus;
    if(c == '=') return Assign;
    if(c == '(') return Lparen;
    if(c == ')') return Rparen;
    if(c == '>') return Print;

    return Other;
}

int getNextChar() {
    int c;
    
    if((c=fin.get()) == EOF) fin.close();

    return c;
}

int peekNextChar() {
    int c;
    
    if((c=fin.peek()) == EOF) fin.close();

    return c;
}

Token getNextToken() {
    int c = getNextChar();
    int num = 0;
    string str = "";

    while(isspace(c)) 
        c = getNextChar();

    if(c == EOF) 
        return Token(_EOF);

    if(isdigit(c)) {
        num = c - '0';
        
        while(1) {
            if(isdigit(peekNextChar())) {
                c = getNextChar();
                num = num * 10 + (c - '0');
            
            } else
                break;
        }

        return Token(Number, num);
    
    } else if(islower(c)) {
        num = c - 'a'; // variable number

        return Token(Variable, num);
    
    } else {
        Token returned_token = Token(getKind(c));

        return returned_token;
    }

    return Token(Other);
}

void operate(TokenKind kind) {
    int result;
    int n2 = stack[--stack_index];
    int n1 = stack[--stack_index];

    if(kind == Multi) result = n1 * n2;
    if(kind == Divi) result = n1 / n2;
    if(kind == Plus) result = n1 + n2;
    if(kind == Minus) result = n1 - n2;

    stack[stack_index++] = result;
}

void factor() {
    if(next_token.token_kind == Variable) stack[stack_index++] = variable[next_token.int_value];
    if(next_token.token_kind == Number) stack[stack_index++] = next_token.int_value;
    if(next_token.token_kind == Lparen) {
        next_token = getNextToken();

        expression();
    }

    next_token = getNextToken();
}

void term() {
    factor();

    while(next_token.token_kind == Multi || next_token.token_kind == Divi) {
        TokenKind kind = next_token.token_kind;
        next_token = getNextToken();

        factor();
        operate(kind);
    }
}

void expression() {
    term();

    while(next_token.token_kind == Plus || next_token.token_kind == Minus) {
        TokenKind kind = next_token.token_kind;
        next_token = getNextToken();

        term();
        operate(kind);
    }
}

void statement() {
    int var;

    if(next_token.token_kind == Variable) {
        var = next_token.int_value; // variable number

        next_token = getNextToken(); // Assign
        next_token = getNextToken();

        expression();

        variable[var] = stack[--stack_index]; 
    }

    if(next_token.token_kind == Print) {
        next_token = getNextToken();
        
        expression();

        cout << stack[--stack_index] << "\n";
    }
}

int main() {
    fin.open("mini_calc_p.txt");
    if(!fin) exit(1);

    next_token = getNextToken();

    while(next_token.token_kind != _EOF) {
        statement();

        // next_token = getNextToken();
    }

    return 0;
}