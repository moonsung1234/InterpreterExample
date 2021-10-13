
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

ifstream fin;

union Value {
    string string_value;
    int int_value;
};

vector<Code> codes;
map<string, Value> memory;
vector<int> stack;

int pc = 0;
int stack_index = 0;

Token token;

// default setting variable
const int LOCAL_SIZE = 10;

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
    Assign, // =
    Equal, // ==
    NotEqual, // !=
    RightEqual, // <=
    LeftEqual, // >=
    Other,
    Variable,
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

struct Code {
    TokenKind kind;
    string name;
    Value value;
    int arguments_length;
    int end_local_address;
    int address;

    void setValue(int int_value) {
        value.int_value = int_value;
    }

    void setValue(string string_value) {
        value.string_value = string_value;
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

    if(str == "=") // =
        return Assign;

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

        case Assign :
            return "assign";

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

int getOrder(TokenKind kind) {
    switch(kind) {
        case Multi : case Divi : return 3;
        case Plus : case Minus : return 2;
        case Equal : case LeftEqual : case RightEqual : return 1;
        default : return 0;
    }
} 

void operate(TokenKind kind) {
    int result;
    int n2 = stack.at(--stack_index);
    int n1 = stack.at(--stack_index);

    switch(kind) {
        case Multi :
            result = n1 * n2;

            break;

        case Divi :
            result = n1 / n2;

            break;

        case Plus :
            result = n1 + n2;

            break;

        case Minus :
            result = n1 - n2;

            break;
    }

    stack.at(stack_index++) = result;
}

void factor() {
    switch(token.token_kind) {
        case Int : 
            stack.at(stack_index++) = token.int_value;
            
            break;

        case String : 
            stack.at(stack_index++) = token.string_value;

            break;

        case Other :
            stack.at(stack_index++) = memory[token.string_value].int_value;

            break;
    }

    token = getNextToken();
}

void term(int n) {
    TokenKind kind;

    if(n == 4) {
        factor();

        return;
    }

    term(n + 1);

    if(n == getOrder(token.token_kind)) {
        kind = token.token_kind;
        token = getNextToken();

        term(n + 1);
        operate(kind);
    }
}

void setFunction() {
    Code code;
    token = getNextToken();

    code.kind = Function;
    code.name = token.string_name;
    code.address = pc;
    code.arguments_length = 0;
    code.end_local_address = pc + LOCAL_SIZE;

    token = getNextToken(); // (

    while((token=getNextToken()) != ')') {
        code.arguments_length++;
    }

    token = getNextToken(); // after )
    pc += LOCAL_SIZE + 1;

    codes.push_back(code);
}

void parse() {
    Code code;
    
    // variable
    if(token.token_kind == Other) {
        string var_name = token.string_value;
        token = getNextToken(); // assign

        term(1);

        code.kind = Variable;
        code.name = var_name;
        code.setValue(stack.at(--stack_index));
        code.address = pc++;
        code.arguments_length = 0;
        code.end_local_address = 0;

        codes.push_back(code);

    } else if(token.token_kind == Function) {
        setFunction();
    }
}

void execute() {
    // something code will be here...
}

int main() {
    fin.open("ms_script_p.txt");

    if(!fin) exit(1);

    while(token.token_kind != _EOF) {
        cout << getKindName(token.token_kind) << "\n";

        parse();
        execute();
    }

    return 0;
}