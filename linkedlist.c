/*Linked List Assignment
By Luke Wharton and Marc Eidelhoch*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "linkedlist.h"
#include "value.h"
#include <assert.h>
#include <string.h>
#include "talloc.h"


// Create a new NULL_TYPE value node.
Value *makeNull() {
    Value *nullNode = talloc(sizeof(Value));
    nullNode->type = NULL_TYPE;
    return nullNode;
}

// Create a new CONS_TYPE value node.
Value *cons(Value *newCar, Value *newCdr) {
    Value *consNode = talloc(sizeof(Value));
    consNode->type = CONS_TYPE;
    consNode->c.car = newCar;
    consNode->c.cdr = newCdr;
    return consNode;
}



// Helper function for display function
void displayHelper(Value *list) {
    if (isNull(cdr(list)) == false) {
        switch (car(list)->type) {
        case INT_TYPE:
            printf("%i ", car(list)->i);
            break;
        case DOUBLE_TYPE:
            printf("%f ", car(list)->d);
            break;
        case STR_TYPE:
            printf("%s ", car(list)->s);
            break;
        case CONS_TYPE:
            printf("(");
            displayHelper(list);
            printf(")");
            break;
        case NULL_TYPE:
            break;
        case PTR_TYPE:
            break;
        case OPEN_TYPE:
            printf("%s ", car(list)->s);
            break;
        case CLOSE_TYPE:
            printf("%s ", car(list)->s);
            break;
        case BOOL_TYPE:
            printf("%s ", car(list)->s);
            break;
        case SYMBOL_TYPE:
            printf("%s ", car(list)->s);
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
        displayHelper(cdr(list));
    } 
    else {
        switch (car(list)->type) {
        case INT_TYPE:
            printf("%i", car(list)->i);
            break;
        case DOUBLE_TYPE:
            printf("%f", car(list)->d);
            break;
        case STR_TYPE:
            printf("%s", car(list)->s);
            break;
        case CONS_TYPE:
            printf("(");
            displayHelper(list);
            printf(")");
            break;
        case NULL_TYPE:
        case PTR_TYPE:
        case OPEN_TYPE:
        case CLOSE_TYPE:
        case BOOL_TYPE:
        case SYMBOL_TYPE:
        case OPENBRACKET_TYPE:
        case CLOSEBRACKET_TYPE:
        case DOT_TYPE:
        case SINGLEQUOTE_TYPE:
        case VOID_TYPE:
        case CLOSURE_TYPE:
        case PRIMITIVE_TYPE:
        case UNSPECIFIED_TYPE:
            break; 
        }
    }
}


// Display the contents of the linked list to the screen in some kind of
// readable format
void display(Value *list) {
    assert(list != NULL);
    printf("(");
    displayHelper(list);
    printf(")\n");
}



// Return a new list that is the reverse of the one that is passed in. No stored
// data within the linked list is duplicated; rather, a new linked list
// of CONS_TYPE nodes is created, that point to items in the original
// list.
Value *reverse(Value *list){
    Value *newHead = makeNull();
    Value *temp = list;

    while (temp->type != NULL_TYPE) {
        newHead = cons(temp->c.car, newHead);
        temp = temp->c.cdr;
    }
    return newHead;
}


// Utility to make it less typing to get car value. Use assertions to make sure
// that this is a legitimate operation.
Value *car(Value *list) {
    assert(list != NULL);
    assert(list->type == CONS_TYPE);
    return list->c.car;
}

// Utility to make it less typing to get cdr value. Use assertions to make sure
// that this is a legitimate operation.
Value *cdr(Value *list) {
    assert(list != NULL);
    assert(list->type == CONS_TYPE);
    return list->c.cdr;
}


// Utility to check if pointing to a NULL_TYPE value. Use assertions to make sure
// that this is a legitimate operation.
bool isNull(Value *value) {
    assert(value != NULL);
    if (value->type == NULL_TYPE) {
        return true;
    } else {
        return false;
    }
}

// Measure length of list. Use assertions to make sure that this is a legitimate
// operation.
int length(Value *value) {
    assert(value != NULL);
    if (isNull(value)) {
        return 0;
    } else {
        assert(value->type == CONS_TYPE);
        return 1 + length(value->c.cdr);
    }
}

