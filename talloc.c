/*Talloc Assignment
Luke Wharton and Marc Eidelhoch*/

#include <stdlib.h>
#include "value.h"
#include "talloc.h"

Value *tallocHead;


// Create a new CONS_TYPE value node.
Value *consTalloc(Value *newCar, Value *newCdr) {
    Value *consNode = malloc(sizeof(Value));
    consNode->type = CONS_TYPE;
    consNode->c.car = newCar;
    consNode->c.cdr = newCdr;
    return consNode;
}

// Replacement for malloc that stores the pointers allocated. It should store
// the pointers in some kind of list; a linked list would do fine, but insert
// here whatever code you'll need to do so; don't call functions in the
// pre-existing linkedlist.h. Otherwise you'll end up with circular
// dependencies, since you're going to modify the linked list to use talloc.
void *talloc(size_t size){
    Value *newNode = malloc(size);
    tallocHead = consTalloc(newNode, tallocHead);
    return newNode;
}


// Free all pointers allocated by talloc, as well as whatever memory you
// allocated in lists to hold those pointers.
void tfree(){
    Value *current = tallocHead;
    Value *follow;
    while (current->c.cdr != NULL) {
        follow = current->c.cdr;
        free(current->c.car);
        free(current);
        current = follow;
    }
    free(current->c.car);
    free(current);
    tallocHead = NULL;
}


// Replacement for the C function "exit", that consists of two lines: it calls
// tfree before calling exit. It's useful to have later on; if an error happens,
// you can exit your program, and all memory is automatically cleaned up.
void texit(int status){
    tfree();
    exit(status);
}