
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

char out[1000];
int out_index = 0;

int stack[1000];
int stack_index = 0;

ifstream fin;

int getNextChar() {
    int c;
    
    if((c=fin.get()) == EOF) fin.close();

    return c;
}

int getOrder(int c) {
    if(c == '*' || c == '/') return 3;
    if(c == '+' || c == '-') return 2;
    if(c == '(') return 1;

    return 0;
}

void polish(char* str) {
    while(*str != NULL) {
        while(isspace(*str)) str++; // skip spaces

        if(islower(*str) || isdigit(*str)) 
            out[out_index++] = *str;
        
        else if(*str == '(') 
            stack[stack_index++] = *str;
        
        else if(*str == ')') {
            char c;
            
            while((c=stack[--stack_index]) != '(') {
                out[out_index++] = c;
            }
        
        } else {
            while(stack_index > 0 && getOrder(*str) <= getOrder(stack[stack_index - 1])) {
                out[out_index++] = stack[--stack_index];
            }

            stack[stack_index++] = *str;
        }

        str++;

        // cout << "stack : " << stack << "\n";
        // cout << "out : " << out << "\n";
    }

    while(stack_index > 0) {
        out[out_index++] = stack[--stack_index];
    }

    out[out_index] = NULL;
}

double eval(int n1, char s, int n2) {
    if(s == '*') return (double) n1 * n2;
    if(s == '/') return (double) n1 / n2;
    if(s == '+') return (double) n1 + n2;
    if(s == '-') return (double) n1 - n2;

    return 0.0;
}

double execute(char* str) {
    while(*str != NULL) {
        while(isspace(*str)) str++;

        if(islower(*str))
            stack[stack_index++] = *str;

        if(isdigit(*str))
            stack[stack_index++] = *str - '0';

        if(getOrder(*str) > 1) {
            int n2 = stack[--stack_index];
            int n1 = stack[--stack_index];

            stack[stack_index++] = eval(n1, *str, n2);
        }

        str++;
    }

    return (double) stack[--stack_index];
}

int main() {
    char command[1000];

    cout << "command : ";
    cin.getline(command, 1000);

    polish(command);

    cout << "change : " << out << "\n";
    cout << "result : " << execute(out) << "\n";

    return 0;
}
