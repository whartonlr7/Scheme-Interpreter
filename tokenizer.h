#include "value.h"
#include <stdbool.h>

#ifndef _TOKENIZER
#define _TOKENIZER

// Read all of the input from stdin, and return a linked list consisting of the
// tokens.
Value *tokenize();

// Checks if a character is an identifier (helper function for identifying symbols)
bool isIdentifier (char c);

// Checks if a character is an initial (helper function for identifying symbols)
bool isInitial(char c);

// Checks if a character is a subsequent (helper function for identifying symbols)
bool isSubsequent(char c);

// Checks if a character is a letter (helper function for identifying symbols)
bool isLetter(char c);

// Checks if a character is an initial symbol (helper function for identifying symbols)
bool isInitialSymbol(char c);

// Checks if a character is a digit (helper function for identifying numbers and symbols)
bool isDigit(char c);

// Returns the unicode value of a character
int getUnicode(char c);

// Displays the contents of the linked list as tokens, with type information
void displayTokens(Value *list);

#endif
