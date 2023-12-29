#include "value.h"

#ifndef _PARSER
#define _PARSER

// Takes a list of tokens from a Scheme program, and returns a pointer to a
// parse tree representing that program.
Value *parse(Value *tokens);

// Takes a given token and adds it to the parse tree, creating subtrees as needed
// based off of the type of token being passed in.
Value *addToParseTree (Value *tree, int *depth, Value *token);

// Helper function for printTree
void printTreeHelper(Value *tree);

// Prints the tree to the screen in a readable fashion. It should look just like
// Scheme code; use parentheses to indicate subtrees.
void printTree(Value *tree);


#endif
