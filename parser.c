/*Parser Assignment
By Luke Wharton*/

#include <stdlib.h>
#include "value.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "linkedlist.h"
#include "tokenizer.h"
#include "talloc.h"
#include "parser.h"
#include <assert.h>

// Takes a list of tokens from a Scheme program, and returns a pointer to a
// parse tree representing that program.
Value *parse(Value *tokens){
    Value *tree = makeNull();
    int depth = 0;

    Value *current = tokens;
    assert(current != NULL && "Error (parse): null pointer");
    while (current->type != NULL_TYPE) {
        Value *token = car(current);
        tree = addToParseTree(tree, &depth, token);
        current = cdr(current);
    }
    if (depth != 0) {
        printf("Syntax Error: not enough close parentheses.\n");
        texit(1);
    }
    tree = reverse(tree);
    return tree;
}

// Takes a given token and adds it to the parse tree, creating subtrees as needed
// based off of the type of token being passed in.
Value *addToParseTree (Value *tree, int *depth, Value *token){
    Value *newNode = talloc(sizeof(Value));
    Value *subTree = makeNull();
    switch (token->type) {
    case INT_TYPE:
        newNode->type = INT_TYPE;
        newNode->i = token->i;
        tree = cons(newNode, tree);
        break;
    case DOUBLE_TYPE:
        newNode->type = DOUBLE_TYPE;
        newNode->d = token->d;
        tree = cons(newNode, tree);
        break;
    case STR_TYPE:
        newNode->type = STR_TYPE;
        newNode->s = token->s;
        tree = cons(newNode, tree);
        break;
    case CONS_TYPE:
        break;
    case NULL_TYPE:
        break;
    case PTR_TYPE:
        break;
    case OPEN_TYPE:
        newNode->type = OPEN_TYPE;
        tree = cons(newNode, tree);
        *depth = *depth + 1;
        break;
    case CLOSE_TYPE:
        *depth = *depth - 1;
        if (*depth < 0){
            printf("Syntax error: too many close parentheses.\n");
            texit(1);
        }
        while (car(tree)->type != OPEN_TYPE && tree->type != NULL_TYPE){
            subTree = cons(car(tree), subTree);
            tree = cdr(tree);
        }
        if (car(tree)->type == NULL_TYPE){
            printf("Parsing Error\n");
            texit(1);
        }
        tree = cdr(tree);
        tree = cons(subTree, tree);
        break;
    case BOOL_TYPE:
        newNode->type = BOOL_TYPE;
        newNode->i = token->i;
        tree = cons(newNode, tree);
        break;
    case SYMBOL_TYPE:
        newNode->type = SYMBOL_TYPE;
        newNode->s = token->s;
        tree = cons(newNode, tree);
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
    return tree;
}

// Helper function for printTree
void printTreeHelper(Value *tree){
    printTree(tree);
    printf("\n");
}


// Prints the tree to the screen in a readable fashion. It should look just like
// Scheme code; use parentheses to indicate subtrees.
void printTree(Value *tree){
    assert(tree != NULL);
    if (tree->type == CONS_TYPE && isNull(cdr(tree)) == false) {
        switch (car(tree)->type) {
        case INT_TYPE:
            printf("%i ", car(tree)->i);
            printTree(cdr(tree));
            break;
        case DOUBLE_TYPE:
            printf("%f ", car(tree)->d);
            printTree(cdr(tree));
            break;
        case STR_TYPE:
            printf("%s ", car(tree)->s);
            printTree(cdr(tree));     
            break;
        case CONS_TYPE:
            printf("(");
            printTree(car(tree));
            printf(") ");
            printTree(cdr(tree));
            break;
        case NULL_TYPE:
            printf("()");
            printTree(cdr(tree));
            break;
        case PTR_TYPE:
            break;
        case OPEN_TYPE:
            break;
        case CLOSE_TYPE:
            break;
        case BOOL_TYPE:
            if (car(tree)->i == 1){
                printf("#t ");
            } else {
                printf("#f ");
            }
            printTree(cdr(tree));
            break;
        case SYMBOL_TYPE:
            printf("%s ", car(tree)->s);
            printTree(cdr(tree));
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
            printf("#<procedure>");
            printTree(cdr(tree));
            break; 
        case PRIMITIVE_TYPE:
        case UNSPECIFIED_TYPE:
            break;        
        }
    }
    //The else statement helps with the removal of extra spacing on the last
    //item in a given list
    else if (tree->type != CONS_TYPE){
        switch (tree->type) {
        case INT_TYPE:
            printf(". %i", tree->i);
            break;
        case DOUBLE_TYPE:
            printf(". %f", tree->d);
            break;
        case STR_TYPE:
            printf(". %s", tree->s);
            break;
        case CONS_TYPE:
            break;
        case NULL_TYPE:
            printf("()");
            break;
        case PTR_TYPE:
            break;
        case OPEN_TYPE:
            break;
        case CLOSE_TYPE:
            break;
        case BOOL_TYPE:
            if (car(tree)->i == 1){
                printf(". #t");
            } else {
                printf(". #f");
            }
            break;
        case SYMBOL_TYPE:
            printf(". %s", tree->s);
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
            printf(". #<procedure>");
            break; 
        case PRIMITIVE_TYPE:
        case UNSPECIFIED_TYPE:
            break;  
        }
        }
    else {
        switch (car(tree)->type) {
        case INT_TYPE:
            printf("%i", car(tree)->i);
            break;
        case DOUBLE_TYPE:
            printf("%f", car(tree)->d);
            break;
        case STR_TYPE:
            printf("%s", car(tree)->s);
            break;
        case CONS_TYPE:
            printf("(");
            printTree(car(tree));
            printf(")");
            break;
        case NULL_TYPE:
            printf("()");
            break;
        case PTR_TYPE:
            break;
        case OPEN_TYPE:
            break;
        case CLOSE_TYPE:
            break;
        case BOOL_TYPE:
            if (car(tree)->i == 1){
                printf("#t");
            } else {
                printf("#f");
            }
            break;
        case SYMBOL_TYPE:
            printf("%s", car(tree)->s);
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
            printf("#<procedure>");
            break; 
        case PRIMITIVE_TYPE:
        case UNSPECIFIED_TYPE:
            break;  
        }
    }
}
