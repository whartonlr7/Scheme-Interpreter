/*Tokenizer Assignment
Luke Wharton and Marc Eidelhoch*/

#include <stdlib.h>
#include "value.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "linkedlist.h"
#include "tokenizer.h"
#include "talloc.h"

// Read all of the input from stdin, and return a linked list consisting of the
// tokens.
Value *tokenize(){
    char charRead;
    Value *temp;
    Value *list = makeNull();
    charRead = (char)fgetc(stdin);
    int i = 0;

    while (charRead != EOF) {

        // Reads the next character if its looking at a newline
        if (charRead == '\n'){
            charRead = (char)fgetc(stdin);
        } 

        // Reads the next character if its looking at a space
        else if (charRead == ' '){
            charRead = (char)fgetc(stdin);
        } 

        // Reads all the way through to the next line if its looking at a comment
        else if (charRead == ';'){
            while (charRead != '\n'){
                charRead = (char)fgetc(stdin);
            }
            charRead = (char)fgetc(stdin);
        } 
        
        // Reads through a string and stores it in a value struct
        else if (charRead == '"'){
            char *newString = talloc(sizeof(char) * 300);
            int index = 1;
            newString[0] = charRead;
            charRead = (char)fgetc(stdin);
            while (getUnicode(charRead) != 34){
                newString[index] = charRead;
                index ++;
                charRead = (char)fgetc(stdin);
            }
            newString[index] = charRead;
            newString[index + 1] = '\0';
            temp = talloc(sizeof(Value));
            temp->type = STR_TYPE;
            temp->s = newString;
            list = cons(temp, list);
            charRead = (char)fgetc(stdin);
        } 
        
        // Stores an open paren in a value struct
        else if (charRead == '(') {
            temp = talloc(sizeof(Value));
            temp->type = OPEN_TYPE;
            list = cons(temp, list);
            charRead = (char)fgetc(stdin);
        } 
        
        // Stores a closed paren in a value struct
        else if (charRead == ')') {
            temp = talloc(sizeof(Value));
            temp->type = CLOSE_TYPE;
            list = cons(temp, list);
            charRead = (char)fgetc(stdin);
        } 

        // Identifies a symbol and stores it in a value struct
        else if (isInitial(charRead)) {
            char *newSymbol = talloc(sizeof(char) * 300);
            int index = 1;
            newSymbol[0] = charRead;
            charRead = (char)fgetc(stdin);
            while (isSubsequent(charRead)){
                newSymbol[index] = charRead;
                index++;
                charRead = (char)fgetc(stdin);
            }
            newSymbol[index] = '\0';
            if (getUnicode(charRead) == 32 || charRead == '\n' || charRead == ')') {
                temp = talloc(sizeof(Value));
                temp->type = SYMBOL_TYPE;
                temp->s = newSymbol;
                list = cons(temp, list);
            } else {
                printf("syntax error 1");
                texit(1);
            }
        }

        // Identifies a + or - and treats it as either a symbol or part of a number accordingly
        else if (charRead == '+' || charRead == '-'){
            char tempChar = charRead;
            charRead = (char)fgetc(stdin);
            if (isDigit(charRead)) {
                bool containsDecimal = false;
                char *digitString = talloc(sizeof(char) * 300);
                int index = 2;
                int integer;
                double dbl;
                digitString[0] = tempChar;
                digitString[1] = charRead;
                charRead = (char)fgetc(stdin);
                while (isDigit(charRead) || charRead == '.'){
                    if (charRead == '.'){
                        if (containsDecimal == true) {
                            printf("Syntax Error 4");
                            texit(1);
                        }
                        else {
                            containsDecimal = true;
                        }
                    }
                    digitString[index] = charRead;
                    index++;
                    charRead = (char)fgetc(stdin);
                }
                if (charRead == ' ' || charRead == '\n' || charRead == ')'){
                    digitString[index] = '\0';
                    if (containsDecimal == false){
                        integer = atoi(digitString);
                        temp = talloc(sizeof(Value));
                        temp->type = INT_TYPE;
                        temp->i = integer;
                        list = cons(temp, list);
                    } else {
                        char *n;
                        double dbl = strtod(digitString, &n);
                        temp = talloc(sizeof(Value));
                        temp->type = DOUBLE_TYPE;
                        temp->d = dbl;
                        list = cons(temp, list);
                    }
                    } else {
                        printf("Syntax Error 3");
                        texit(1);
                    }
            } else if (getUnicode(charRead) == 32 || charRead == '\n' || charRead == ')') {
                temp = talloc(sizeof(Value));
                temp->type = SYMBOL_TYPE;
                char *symbolString = talloc(sizeof(char)*2);
                symbolString[0] = tempChar;
                symbolString[1] = '\0';
                temp->s = symbolString;
                list = cons(temp, list);
            } else {
                printf("syntax error 6");
                texit(1);
            }
        }

        // Identifies a digit and stores it in a value struct
        else if (isDigit(charRead)){
            bool containsDecimal = false;
            char *digitString = talloc(sizeof(char) * 300);
            int index = 1;
            int integer;
            double dbl;
            digitString[0] = charRead;
            charRead = (char)fgetc(stdin);
            while (isDigit(charRead) || charRead == '.'){
                if (charRead == '.'){
                    if (containsDecimal == true) {
                        printf("Syntax Error 7");
                        texit(1);
                    }
                    else {
                        containsDecimal = true;
                    }
                }
                digitString[index] = charRead;
                index++;
                charRead = (char)fgetc(stdin);
            }
            if (charRead == ' ' || charRead == '\n' || charRead == ')'){
                digitString[index] = '\0';
                if (containsDecimal == false){
                    integer = atoi(digitString);
                    temp = talloc(sizeof(Value));
                    temp->type = INT_TYPE;
                    temp->i = integer;
                    list = cons(temp, list);
                } else {
                    char *n;
                    double dbl = strtod(digitString, &n);
                    temp = talloc(sizeof(Value));
                    temp->type = DOUBLE_TYPE;
                    temp->d = dbl;
                    list = cons(temp, list);
                }
            }
        }

        // Identifies a boolean and stores it in a value struct
        else if (charRead == '#') {
            charRead = (char)fgetc(stdin);
            if (charRead == 't') {
                temp = talloc(sizeof(Value));
                temp->type = BOOL_TYPE;
                temp->i = 1;
                list = cons(temp, list);
                charRead = (char)fgetc(stdin);
            } else if (charRead == 'f') {
                temp = talloc(sizeof(Value));
                temp->type = BOOL_TYPE;
                temp->i = 0;
                list = cons(temp, list);
                charRead = (char)fgetc(stdin);
            } else {
                printf("Syntax error (readBoolean): boolean was not #t or #f\n");
                texit(1);
            }

        // Prints a syntax error if a symbol starts with an illegal character
        } else {
            printf("Syntax error (readSymbol): symbol %c does not start with an allowed first character.\n", charRead);
            texit(1);
        }
    }
    Value *revList = reverse(list);
    return revList;
}


// Checks if a character is an identifier (helper function for identifying symbols)
bool isIdentifier (char c){
    if (c == '+'){
        return true;
    } else if (c == '-'){
        return true;
    } else if (isInitial(c)){
        return true;
    } else {
        return false;
    }
}
    
// Checks if a character is an initial (helper function for identifying symbols)
bool isInitial(char c) {
    if (isLetter(c)){
        return true;
    }
    else if (isInitialSymbol(c)) {
        return true;
    } else {
        return false;
    }
}

// Checks if a character is a subsequent (helper function for identifying symbols)
bool isSubsequent(char c){
    if (isInitial(c)){
        return true;
    } else if (isDigit(c)) {
        return true;
    } else if (c == '.' || c == '+' || c == '-'){
        return true;
    } else{
        return false;
    }
    
}

// Checks if a character is a letter (helper function for identifying symbols)
bool isLetter(char c) {
    int unicode = getUnicode(c);
    if (65 <= unicode && unicode <= 90){
        return true;
    } else if (97 <= unicode && unicode <= 122){
        return true;
    } else {
        return false;
    }
}

// Checks if a character is an initial symbol (helper function for identifying symbols)
bool isInitialSymbol(char c) {
    if (c =='!' || c == '$' || c == '%' || c == '*' || c == '/' || c == ':' || 
    c == '<' || c == '>' || c == '=' || c == '?' || c == '~' || c == '_' || c == '^') {
        return true;
    } else{
        return false;
    }
}

// Checks if a character is a digit (helper function for identifying numbers and symbols)
bool isDigit(char c) {
    if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9') {
        return true;
    } else {
        return false;
    }
}

// Returns the unicode value of a character
int getUnicode(char c) {
    int i = c;
    return i;
}


// Displays the contents of the linked list as tokens, with type information
void displayTokens(Value *list) {
    Value *temp = list;
    while (temp->type != NULL_TYPE){
            switch (car(temp)->type) {
            case INT_TYPE:
                printf("%i:integer \n", car(temp)->i);
                break;
            case DOUBLE_TYPE:
                printf("%f:double \n", car(temp)->d);
                break;
            case STR_TYPE:
                printf("%s:string\n", car(temp)->s);
                break;
            case CONS_TYPE:
                break;
            case NULL_TYPE:
                break;
            case PTR_TYPE:
                break;
            case OPEN_TYPE:
                printf("(:open\n");
                break;
            case CLOSE_TYPE:
                printf("):close\n");
                break;
            case BOOL_TYPE:
                if (temp->c.car->i == 1){
                    printf("#t:boolean\n");
                } else {
                    printf("#f:boolean\n");
                }
                break;
            case SYMBOL_TYPE:
                printf("%s:symbol\n", car(temp)->s);
                break;
            case OPENBRACKET_TYPE:
                break;
            case CLOSEBRACKET_TYPE:
                break;
            case DOT_TYPE:
                break;
            case SINGLEQUOTE_TYPE:
                break;
            case VOID_TYPE:
                break;
            case CLOSURE_TYPE:
                break;
            case PRIMITIVE_TYPE:
            case UNSPECIFIED_TYPE:
                break;
            }
            temp = temp->c.cdr;

    }
}