/*Final Assignment
By Luke Wharton and Marc Eidelhoch*/

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
#include "interpreter.h"



// Takes in a frame and creates a new frame with the frame that was passed in as the parent.
Frame *initializeFrame(Frame *frame){
    Frame*newFrame = talloc(sizeof(Frame));
    newFrame->bindings = makeNull();
    newFrame->parent = frame;
    return newFrame;
}

// Traverses the parsed tree and executes the code
void interpret(Value *tree){
    Frame *initialFrame = talloc(sizeof(Frame));
    initialFrame->bindings = makeNull();
    initialFrame->parent = NULL;
    bind("+", primitiveAdd, initialFrame);
    bind("car", primitiveCar, initialFrame);
    bind("cdr", primitiveCdr, initialFrame);
    bind("null?", primitiveNull, initialFrame);
    bind("cons", primitiveCons, initialFrame);
    bind("-", primitiveMinus, initialFrame );
    bind(">", primitiveGreater, initialFrame);
    bind("<", primitiveLess, initialFrame);
    bind("=", primitiveEqual, initialFrame);
    bind("*", primitiveMultiply, initialFrame);
    bind("/", primitiveDivide, initialFrame);
    bind("modulo", primitiveMod, initialFrame);
    Value *current = tree;
    Value *newTree = makeNull();
    while (current->type != NULL_TYPE){
        newTree = cons(eval(car(current), initialFrame), newTree);
        printTreeHelper(newTree);
        newTree = makeNull();
        current = cdr(current);
    }
}


// Takes in a subtree and a frame and evaluates the statement in the given frame.
Value *eval(Value *tree, Frame *frame){
    Value *result;
    Value *first;
    Value *args;
    switch (tree->type){
    case INT_TYPE: {
        return tree;
        break;
    }case DOUBLE_TYPE:
        return tree;
        break;
    case STR_TYPE:
        return tree;
        break;
    case CONS_TYPE:
        first = car(tree);
        args = cdr(tree);
        if (!strcmp(first->s,"if")) {
            result = evalIf(args,frame);
        } else if (!strcmp(first->s, "cond")) {
            result = evalCond(args, frame);
        } else if (!strcmp(first->s,"let")){
            result = evalLet(args, frame);
        } else if (!strcmp(first->s,"let*")){
            result = evalLetStar(args, frame);
        } else if (!strcmp(first->s,"letrec")){
            result = evalLetRec(args, frame);    
        } else if (!strcmp(first->s, "quote")){
            result = evalQuote(args, frame);
        } else if (!strcmp(first->s, "lambda")){
            result = evalLambda(args, frame);
        } else if (!strcmp(first->s,"define")){
            result = evalDefine(args, frame);
        } else if (!strcmp(first->s,"set!")){
            result = evalSet(args, frame);
        } else if (!strcmp(first->s, "begin")){
            result = evalBegin(args, frame);
        } else if (!strcmp(first->s, "and")){
            result = evalAnd(args, frame);
        } else if (!strcmp(first->s, "or")){
            result = evalOr(args, frame);
        } else {
            // If not a special form, evaluate the first, evaluate the args, then
            // apply the first to the args.
            Value *evaledOperator = eval(first, frame);
            Value *evaledArgs = evalEach(args, frame); 
            return apply(evaledOperator, evaledArgs);
        }
        break;
    case NULL_TYPE:
        return tree;
        break;
    case PTR_TYPE:
        break;
    case OPEN_TYPE:
        break;
    case CLOSE_TYPE:
        break;
    case BOOL_TYPE:
        return tree;
        break;
    case SYMBOL_TYPE:
        return lookUpSymbol(tree, frame);
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
        return tree;
        break;
    case PRIMITIVE_TYPE:
        return tree;
        break;
    case UNSPECIFIED_TYPE:
        printf("Evaluation Error in unspecified\n");
        texit(1);
        break;
    }
    return result;
}

// Add primitive functions to top-level bindings list
void bind(char *name, Value *(*function)(struct Value *), Frame *frame) {
    Value *value = talloc(sizeof(Value));
    value->type = PRIMITIVE_TYPE;
    value->pf = function;
    Value *newBinding = makeNull();
    Value *symbol = talloc(sizeof(Value));
    symbol->type = SYMBOL_TYPE;
    symbol->s = name;
    newBinding = cons(value, newBinding);
    newBinding = cons(symbol, newBinding);
    frame->bindings = cons(newBinding, frame->bindings);
}

// Primitive function for less than
Value *primitiveLess(Value *args){
    Value *result = talloc(sizeof(Value));
    result->type = BOOL_TYPE;
    if (length(args) != 2){
        printf("Evaluation Error: Wrong number of arguments to <\n");
        texit(1);
    }
    if (car(args)->type == INT_TYPE && car(cdr(args))->type == INT_TYPE){
        if (car(args)->i < car(cdr(args))->i){
            result->i = 1;
            return result;
        } else{
            result->i = 0;
            return result;
        }
    } else if (car(args)->type == DOUBLE_TYPE && car(cdr(args))->type == INT_TYPE){
        if (car(args)->d < car(cdr(args))->i){
            result->i = 1;
            return result;
        } else{
            result->i = 0;
            return result;
        }
    } else if (car(args)->type == INT_TYPE && car(cdr(args))->type == DOUBLE_TYPE){
        if (car(args)->i < car(cdr(args))->d){
            result->i = 1;
            return result;
        } else{
            result->i = 0;
            return result;
        }
    } else if (car(args)->type == DOUBLE_TYPE && car(cdr(args))->type == DOUBLE_TYPE){
        if (car(args)->d < car(cdr(args))->d){
            result->i = 1;
            return result;
        } else{
            result->i = 0;
            return result;
        }
    } else {
        printf("Evaluation Error: Wrong type to apply in <\n");
        texit(1);
        return result;
    }
}




// Primitive function for greater than
Value *primitiveGreater(Value *args){
    Value *result = talloc(sizeof(Value));
    result->type = BOOL_TYPE;
    if (length(args) != 2){
        printf("Evaluation Error: Wrong number of arguments in >\n");
        texit(1);
    }
    if (car(args)->type == INT_TYPE && car(cdr(args))->type == INT_TYPE){
        if (car(args)->i > car(cdr(args))->i){
            result->i = 1;
            return result;
        } else{
            result->i = 0;
            return result;
        }
    } else if (car(args)->type == DOUBLE_TYPE && car(cdr(args))->type == INT_TYPE){
        if (car(args)->d > car(cdr(args))->i){
            result->i = 1;
            return result;
        } else{
            result->i = 0;
            return result;
        }
    } else if (car(args)->type == INT_TYPE && car(cdr(args))->type == DOUBLE_TYPE){
        if (car(args)->i > car(cdr(args))->d){
            result->i = 1;
            return result;
        } else{
            result->i = 0;
            return result;
        }
    } else if (car(args)->type == DOUBLE_TYPE && car(cdr(args))->type == DOUBLE_TYPE){
        if (car(args)->d > car(cdr(args))->d){
            result->i = 1;
            return result;
        } else{
            result->i = 0;
            return result;
        }
    } else {
        printf("Evaluation Error: Wrong type to apply in >\n");
        texit(1);
        return result;
    }
}

// Primitive function for =
Value *primitiveEqual(Value *args){
    Value *result = talloc(sizeof(Value));
    result->type = BOOL_TYPE;
    if (length(args) != 2){
        printf("Evaluation Error: Wrong number of arguments in =\n");
        texit(1);
    }
    if (car(args)->type == INT_TYPE && car(cdr(args))->type == INT_TYPE){
        if (car(args)->i == car(cdr(args))->i){
            result->i = 1;
            return result;
        } else{
            result->i = 0;
            return result;
        }
    } else if (car(args)->type == DOUBLE_TYPE && car(cdr(args))->type == INT_TYPE){
        if (car(args)->d == car(cdr(args))->i){
            result->i = 1;
            return result;
        } else{
            result->i = 0;
            return result;
        }
    } else if (car(args)->type == INT_TYPE && car(cdr(args))->type == DOUBLE_TYPE){
        if (car(args)->i == car(cdr(args))->d){
            result->i = 1;
            return result;
        } else{
            result->i = 0;
            return result;
        }
    } else if (car(args)->type == DOUBLE_TYPE && car(cdr(args))->type == DOUBLE_TYPE){
        if (car(args)->d == car(cdr(args))->d){
            result->i = 1;
            return result;
        } else{
            result->i = 0;
            return result;
        }
    } else {
        printf("Evaluation Error: Wrong type to apply in =\n");
        texit(1);
        return result;
    }
}

// Primitive function for multiply
Value *primitiveMultiply(Value *args){
    Value *toMultiply;
    Value *result = talloc(sizeof(Value));
    Value *temp;
    if (car(args)->type == INT_TYPE){
            result->type = INT_TYPE;
            result->i = car(args)->i;
    } else if (car(args)->type == DOUBLE_TYPE){
        result->type = DOUBLE_TYPE;
        result->d = car(args)->d;
    } else {
        printf("Evaluation Error: Wrong type to apply in add\n");
        texit(1);
        return args;
    }
    toMultiply = cdr(args);    
    while  (toMultiply->type != NULL_TYPE){
        temp = car(toMultiply);
        if (result->type == DOUBLE_TYPE){
            if (temp->type == INT_TYPE){
                result->d *= temp->i;
            } else if (temp->type == DOUBLE_TYPE){
                result->d *= temp->d;
            } else {
                printf("Evaluation Error: Wrong type to apply in add\n");
                texit(1);
                return args;
            }
        } else {
            if (temp->type == INT_TYPE){
                result->i *= temp->i;
            } else if (toMultiply->type == DOUBLE_TYPE){
                double typeChangeTemp = result->i;
                typeChangeTemp *= temp->d;
                result->type = DOUBLE_TYPE;
                result->d = typeChangeTemp;
            } else {
                printf("Evaluation Error: Wrong type to apply in add\n");
                texit(1);
                return args;
            }            
        }
        toMultiply = cdr(toMultiply);
    }
    return result;
}

// Primitive function for divide
Value *primitiveDivide(Value *args){
    Value *firstArgument = car(args);
    Value *secondArgument = car(cdr(args));
    Value *result = talloc(sizeof(Value));
    if (length(args) != 2) {
        printf("Evaluation Error: Wrong number of arguments in divide\n");
        texit(1);
        return args;
    } else if (firstArgument->type == INT_TYPE && secondArgument->type == INT_TYPE) {
        if (secondArgument->i == 0) {
        printf("You can't divide by zero silly\n");
        texit(1);
        return args;
        }
        if (firstArgument->i % secondArgument->i == 0) {
            result->type = INT_TYPE;
            result->i = (firstArgument->i / secondArgument->i);
            return result;
        } else {
            result->type = DOUBLE_TYPE;
            result->d = ((float)firstArgument->i / secondArgument->i);
            return result;
        }
    } else if (firstArgument->type == DOUBLE_TYPE && secondArgument->type == INT_TYPE) {
        if (secondArgument->i == 0) {
        printf("You can't divide by zero silly");
        texit(1);
        return args;
        }
        result->type = DOUBLE_TYPE;
        result->d = (firstArgument->d / secondArgument->i);
        return result;
    } else if (firstArgument->type == INT_TYPE && secondArgument->type == DOUBLE_TYPE) {
        if (secondArgument->d == 0) {
        printf("You can't divide by zero silly");
        texit(1);
        return args;
        }
        result->type = DOUBLE_TYPE;
        result->d = (firstArgument->i / secondArgument->d);
        return result;
    } else if (firstArgument->type == DOUBLE_TYPE && secondArgument->type == DOUBLE_TYPE) {
        if (secondArgument->d == 0) {
        printf("You can't divide by zero silly");
        texit(1);
        return args;
        }
        result->type = DOUBLE_TYPE;
        result->d = (firstArgument->d / secondArgument->d);
        return result;
    } else {
        printf("Evaluation Error: Wrong type to apply in divide\n");
        texit(1);
        return args;
    }
}

// Primitive function for modulo
Value *primitiveMod (Value *args) {
    Value *firstArgument = car(args);
    Value *secondArgument = car(cdr(args));
    Value *result = talloc(sizeof(Value));
    if (length(args) != 2) {
        printf("Evaluation Error: Wrong number of arguments in mod\n");
        texit(1);
        return args;
    } else if (firstArgument->type == INT_TYPE && secondArgument->type == INT_TYPE) {
        result->type = INT_TYPE;
        result->i = (firstArgument->i % secondArgument->i);
        return result;
    } else {
        printf("Evaluation Error: Wrong type to apply in mod\n");
        texit(1);
        return args;
    }
}

// Primitive function for minus
Value *primitiveMinus(Value *args) {
    Value *toSubtract;
    Value *result = talloc(sizeof(Value));
    Value *temp;
    if (car(args)->type == INT_TYPE){
            result->type = INT_TYPE;
            result->i = car(args)->i;
    } else if (car(args)->type == DOUBLE_TYPE){
        result->type = DOUBLE_TYPE;
        result->d = car(args)->d;
    } else {
        printf("Evaluation Error: Wrong type to apply in minus\n");
        texit(1);
        return args;
    }
    toSubtract = cdr(args);    
    while  (toSubtract->type != NULL_TYPE){
        temp = car(toSubtract);
        if (result->type == DOUBLE_TYPE){
            if (temp->type == INT_TYPE){
                result->d -= temp->i;
            } else if (temp->type == DOUBLE_TYPE){
                result->d -= temp->d;
            } else {
                printf("Evaluation Error: Wrong type to apply in minus\n");
                texit(1);
                return args;
            }
        } else {
            if (temp->type == INT_TYPE){
                result->i -= temp->i;
            } else if (toSubtract->type == DOUBLE_TYPE){
                double typeChangeTemp = result->i;
                typeChangeTemp -= temp->d;
                result->type = DOUBLE_TYPE;
                result->d = typeChangeTemp;
            } else {
                printf("Evaluation Error: Wrong type to apply in minus\n");
                texit(1);
                return args;
            }            
        }
        toSubtract = cdr(toSubtract);
    }
    return result;
}


// evaluates cons which has two arguments and returns a cons cell with the first in the car and the second in the cdr
Value *primitiveCons(Value *args){
    if (length(args) != 2){
        printf("Evaluation error: Wrong number of arguments in cons\n");
        texit(1);
        return args;
    }
    Value *consOne = car(args);
    Value *consTwo = car(cdr(args));
    return cons(consOne, consTwo);
}

// evaluates null? and returns true if the argument is null and false if it isn't
Value *primitiveNull(Value *args){
    if (length(args) != 1){
        printf("Evaluation error: Wrong number of arguments in null?\n");
        texit(1);
        return args;
    }
    Value *result = talloc(sizeof(Value));
    result->type = BOOL_TYPE;
    if (args->type == NULL_TYPE){
        result->i = 1;
    } else if (args->type == CONS_TYPE){
        if (car(args)->type == NULL_TYPE){
            result->i = 1;
        } else{
            result->i = 0; 
        }
     } else{
        result->i = 0; 
    }
    return result;
}

// Returns the value in the car of the cons cell that is passed in
Value *primitiveCar(Value *args){
    if (length(args) != 1){
        printf("Evaluation error: Wrong number of arguments in car\n");
        texit(1);
        return args;
    } if (args->type != CONS_TYPE){
        printf("Evaluation error: Wrong type to apply in car\n");
        texit(1);
        return args;
    }
    if (car(args)->type != CONS_TYPE){
        printf("Evaluation error: Wrong type to apply in car\n");
        texit(1);
        return args;
    }
    return car(car(args));
}

// Returns the value in the cdr of the cons cell that is passed in
Value *primitiveCdr(Value *args){
    if (length(args) != 1){
        printf("Evaluation error: Wrong number of arguments in cdr 1\n");
        texit(1);
        return args;
    } if (args->type != CONS_TYPE){
        printf("Evaluation error: Wrong type ot apply in cdr 2\n");
        texit(1);
        return args;
    }
    if (car(args)->type != CONS_TYPE){
        printf("Evaluation error: Wrong type to apply in cdr 3\n");
        texit(1);
        return args;
    }
    return cdr(car(args));
}

// Takes in a variable number of parameters and adds them all together and returns the result
Value *primitiveAdd (Value* args) {
    Value *toAdd;
    Value *result = talloc(sizeof(Value));
    result->type = INT_TYPE;
    result->i = 0;
    while  (args->type != NULL_TYPE){
        toAdd = car(args);
        if (result->type == DOUBLE_TYPE){
            if (toAdd->type == INT_TYPE){
                result->d += toAdd->i;
            } else if (toAdd->type == DOUBLE_TYPE){
                result->d += toAdd->d;
            } else {
                printf("Evaluation Error: Wrong type to apply in add\n");
                texit(1);
                return args;
            }
        } else {
            if (toAdd->type == INT_TYPE){
                result->i += toAdd->i;
            } else if (toAdd->type == DOUBLE_TYPE){
                double temp = result->i;
                temp += toAdd->d;
                result->type = DOUBLE_TYPE;
                result->d = temp;
            } else {
                printf("Evaluation Error: Wrong type to apply in add\n");
                texit(1);
                return args;
            }            
        }
        args = cdr(args);
    }
    return result;
    }


// Takes in a symbol and a frame and tries to find the value of that symbol in the given frame.
// If it does not find the symbol it checks the parent frame.
Value *lookUpSymbol(Value* symbol, Frame *frame){
    Value *temp = frame->bindings;
    while(temp->type != NULL_TYPE){
        //printf("||%s||", symbol->s);
        if (!strcmp(car(car(temp))->s, symbol->s)){
            Value *toReturn = car(cdr(car(temp)));
            if (toReturn->type == CONS_TYPE) {
                return toReturn;
            } else{
                return eval(toReturn, frame);
            }
        }
        temp = cdr(temp);
    }
    if (frame->parent != NULL){
        return lookUpSymbol(symbol, frame->parent);
    } else{
        printf("Evaluation error for lookUpSymbol\n");
        texit(1);
        return symbol;
    }
}


// Takes an operator and parameters and runs the code from the operator with the parameters.
Value *apply(Value *operator, Value *args){
    if (operator->type == CLOSURE_TYPE) {
        Frame *newFrame = initializeFrame(operator->cl.frame);
        Value *tempParams = operator->cl.paramNames;
        Value *tempArgs = args;
        while (tempParams->type != NULL_TYPE){
            Value *newBinding = makeNull();
            newBinding = cons(car(tempArgs), newBinding);
            newBinding = cons(car(tempParams), newBinding);
            newFrame->bindings = cons(newBinding, newFrame->bindings);
            tempParams = cdr(tempParams);
            tempArgs = cdr(tempArgs);
        }
        return eval(operator->cl.functionCode, newFrame);
    } else if (operator->type == PRIMITIVE_TYPE){
        return operator->pf(args);
    }
    else{
        printf("Evaluation error for apply\n");
        texit(1);
        return args;
    }
}

// Evaluates each argument that is passed in recursively and returns them in a linked list
Value *evalEach(Value *args, Frame *frame){
    if (args->type == NULL_TYPE){
        return makeNull();
    } else{
        return cons(eval(car(args), frame), evalEach(cdr(args), frame));
    }
}

// Evaluates conditionals
Value *evalCond(Value *args, Frame *frame){
    Value *evaledArgs;
    while (args->type != NULL_TYPE){  
        if (car(car(args))->type == SYMBOL_TYPE){
            if (!strcmp(car(car(args))->s, "else")){
                return eval(car(cdr(car(args))), frame);
            } else {
                printf("evaluation error for cond");
                texit(1);
                return args;
            }
        } else {
            evaledArgs = eval(car(car(args)), frame);   
            if (evaledArgs->type == BOOL_TYPE){
                if (evaledArgs->i == 1) {
                    return eval(car(cdr(car(args))), frame);
                }
            } else {
                printf("evaluation error for cond 2\n");
                texit(1);
                return args;
            }
        }
        args = cdr(args);
    }
    return makeNull();
}


// Evaluates an or statement
Value *evalOr(Value *args, Frame *frame){
    if (args->type == NULL_TYPE){
        Value *result = talloc(sizeof(Value));
        result->type = BOOL_TYPE;
        result->i = 0;
        return result;
    } else {
        Value *evaledArg = eval(car(args), frame);
        if (evaledArg->type != BOOL_TYPE){
            printf("Evaluation error: Wrong type to apply for or\n");
            texit(1);
            return args;
        } else if (evaledArg->i == 1){
            Value *result = talloc(sizeof(Value));
            result->type = BOOL_TYPE;
            result->i = 1;
            return result;
        } else{
            return evalOr(cdr(args), frame);
        }
    }
}

// Evaluates an and statement
Value *evalAnd(Value *args, Frame *frame){
    if (args->type == NULL_TYPE){
        Value *result = talloc(sizeof(Value));
        result->type = BOOL_TYPE;
        result->i = 1;
        return result;
    } else {
        Value *evaledArg = eval(car(args), frame);
        if (evaledArg->type != BOOL_TYPE){
            printf("Evaluation error: Wrong type to apply for and\n");
            texit(1);
            return args;
        } else if (evaledArg->i == 0){
            Value *result = talloc(sizeof(Value));
            result->type = BOOL_TYPE;
            result->i = 0;
            return result;
        } else{
            return evalAnd(cdr(args), frame);
        }
    }
}

// Evaluates a begin statement
Value *evalBegin(Value *args, Frame *frame){
    Value *evaledArgs;
    while (args->type != NULL_TYPE){
        evaledArgs = eval(car(args), frame);
        args = cdr(args);
    }
    return evaledArgs;
}

// Evaluates a letrec statement
Value *evalLetRec(Value *args, Frame *frame){
    Frame *letFrame = initializeFrame(frame);
    if (length(args) > 1){
        if (car(args)->type != CONS_TYPE && car(args)->type != NULL_TYPE){
            printf("Evaluation error: Wrong number of arguments for letRec\n");
            texit(1);           
        }        
        int numItems = length(car(args));
        Value *temp = car(args);
        while (temp->type != NULL_TYPE){
            if (car(temp)->type == CONS_TYPE && length(car(temp)) == 2){
                if (car(car(temp))->type != SYMBOL_TYPE){
                    printf("Evaluation error: Wrong type to apply for letRec\n");
                    texit(1);
                    return args;                    
                }
                Value *unspecified = talloc(sizeof(Value));
                unspecified->type = UNSPECIFIED_TYPE;
                Value *newBinding = makeNull();
                newBinding = cons(unspecified, newBinding);
                newBinding = cons(car(car(temp)), newBinding);
                letFrame->bindings = cons(newBinding, letFrame->bindings);
            } else{
                printf("Evaluation error: Wrong type to apply for letRec\n");
                texit(1);
                return args;
            }
            temp = cdr(temp);
        }
        temp = car(args);
        Value *evaledArgs = makeNull();
        while (temp->type != NULL_TYPE){
            evaledArgs = cons(eval(car(cdr(car(temp))), letFrame), evaledArgs);
            temp = cdr(temp);
        }
        Value *frameTraverse = letFrame->bindings;
        Value *replacementBinding;
        Value *replacedBinding;
        while (evaledArgs->type != NULL_TYPE){
            replacedBinding = car(cdr(car(frameTraverse)));
            replacementBinding = car(evaledArgs);
            replacedBinding->type = replacementBinding->type;
            switch (replacementBinding->type) {
                case INT_TYPE:
                    replacedBinding->i = replacementBinding->i;
                    break;
                case DOUBLE_TYPE:
                    replacedBinding->d = replacementBinding->d;
                    break;
                case STR_TYPE:
                    replacedBinding->s = replacementBinding->s;
                    break;
                case CONS_TYPE:
                    replacedBinding->c = replacementBinding->c;
                    break;
                case NULL_TYPE:         
                    break;
                case BOOL_TYPE:
                    replacedBinding->i = replacementBinding->i;
                    break;
                case SYMBOL_TYPE:
                    replacedBinding->s = replacementBinding->s;
                    break;
                case CLOSURE_TYPE:
                    replacedBinding->cl = replacementBinding->cl;
                case PTR_TYPE:
                case OPEN_TYPE:
                case CLOSE_TYPE:
                case OPENBRACKET_TYPE:
                case CLOSEBRACKET_TYPE:
                case DOT_TYPE:
                case SINGLEQUOTE_TYPE:
                case VOID_TYPE:
                case PRIMITIVE_TYPE:
                case UNSPECIFIED_TYPE:
                    break; 
                }

            evaledArgs = cdr(evaledArgs);
            frameTraverse = cdr(frameTraverse);
        }
        Value *argsTwo;
        temp = cdr(args);
        while (temp->type != NULL_TYPE){ 
        argsTwo = eval(car(temp), letFrame);
        temp = cdr(temp);
        }
        return argsTwo;
    } else {
        printf("Evaluation error: Wrong type to apply for letRec\n");
        texit(1);
        return args;
    }
}

// Evaluates a let* statement
Value *evalLetStar(Value *args, Frame *frame){
    if (length(args) > 1){
        if (car(args)->type != CONS_TYPE && car(args)->type != NULL_TYPE){
            printf("Evaluation error: Wrong number of arguments for let*\n");
            texit(1);
        }        
        int numItems = length(car(args));
        Value *temp = car(args);
        Frame *current = frame;
        for (int i = 0; i < numItems; i++){
            Frame *letFrame = initializeFrame(current); 
            if (car(temp)->type == CONS_TYPE && length(car(temp)) == 2){
                if (car(car(temp))->type != SYMBOL_TYPE){
                    printf("Evaluation error: Wrong type to apply for let*\n");
                    texit(1);                        
                }               
                Value *newBinding = makeNull();
                newBinding = cons(eval(car(cdr(car(temp))), current), newBinding);
                newBinding = cons(car(car(temp)), newBinding);

                letFrame->bindings = cons(newBinding, letFrame->bindings);
            } else{
                printf("Evaluation error: Wrong type to apply for let*\n");
                texit(1);
                return args;
            }
            temp = cdr(temp);
            current = letFrame;
        }
        Value *argsTwo;
        temp = cdr(args);
        while (temp->type != NULL_TYPE){ 
            argsTwo = eval(car(temp), current);
            temp = cdr(temp);
        }
        return argsTwo;
    } else {
        printf("Evaluation error: Wrong type to apply for let*\n");
        texit(1);
        return args;
    }
}


// Evaluates a define statement and binds the first item (the symbol) to a value in a given frame
Value *evalDefine (Value *args, Frame *frame){
    if (length(args) == 2){
        Value *argOne = car(args);
        Value *argtwo = eval(car(cdr(args)), frame);
        Value *newBinding = makeNull();
        if (argOne->type != SYMBOL_TYPE){
            printf("Evaluation error: Wrong type to apply for define\n");
            texit(1);
        }
        newBinding = cons(argtwo, newBinding);
        newBinding = cons(argOne, newBinding);
        frame->bindings = cons(newBinding, frame->bindings);
        Value *result = talloc(sizeof(Value));

        result->type = VOID_TYPE;
        return result;
    } else{
        printf("Evaluation error: Wrong number of arguments for define\n");
        texit(1);
        return args;
    }
}

// Evaluates a set! statement
Value *evalSet(Value *args, Frame *frame){
    Value *symbol = car(args);
    Value* newValue = eval(car(cdr(args)), frame);
    Value *temp;
    while (frame != NULL){
        temp = frame->bindings;
        while (temp->type != NULL_TYPE){
            if (!strcmp(car(car(temp))->s, symbol->s)){
                switch (newValue->type) {
                case INT_TYPE:
                    car(cdr(car(temp)))->type = INT_TYPE;
                    car(cdr(car(temp)))->i = newValue->i;
                    break;
                case DOUBLE_TYPE:
                    car(cdr(car(temp)))->type = DOUBLE_TYPE;
                    car(cdr(car(temp)))->d = newValue->d;
                    break;
                case STR_TYPE:
                    car(cdr(car(temp)))->type = STR_TYPE;
                    car(cdr(car(temp)))->s = newValue->s;
                    break;
                case CONS_TYPE:
                    car(cdr(car(temp)))->type = CONS_TYPE;
                    car(cdr(car(temp)))->c = newValue->c;
                    break;
                case NULL_TYPE:
                    car(cdr(car(temp)))->type = NULL_TYPE;           
                    break;
                case BOOL_TYPE:
                    car(cdr(car(temp)))->type = BOOL_TYPE;
                    car(cdr(car(temp)))->i = newValue->i;
                    break;
                case SYMBOL_TYPE:
                    car(cdr(car(temp)))->type = SYMBOL_TYPE;
                    car(cdr(car(temp)))->s = newValue->s;
                    break;
                case CLOSURE_TYPE:
                    car(cdr(car(temp)))->type = CLOSURE_TYPE;
                    car(cdr(car(temp)))->cl = newValue->cl;
                case PTR_TYPE:
                case OPEN_TYPE:
                case CLOSE_TYPE:
                case OPENBRACKET_TYPE:
                case CLOSEBRACKET_TYPE:
                case DOT_TYPE:
                case SINGLEQUOTE_TYPE:
                case VOID_TYPE:
                case PRIMITIVE_TYPE:
                case UNSPECIFIED_TYPE:
                    break; 
                }
            }
            temp = cdr(temp);
        }
        frame = frame->parent;
    }
    Value *result = talloc(sizeof(Value));
    result->type = VOID_TYPE;
    return result;
}

// Evaluates a lambda and creates a new closure which contains parameter names, the function code and the frame.
Value *evalLambda(Value *args, Frame *frame){
    if (args->type == NULL_TYPE){
        printf("Evaluation error: Wrong type to apply for lambda\n");
        texit(1);
    }
    Value *temp = car(args);
    Value *tempTwo;
    while (temp->type != NULL_TYPE){
        if (car(temp)->type != SYMBOL_TYPE){
            printf("Evaluation error: Wrong type to apply for lambda\n");
            texit(1);
        }
        tempTwo = cdr(temp);
        while (tempTwo->type != NULL_TYPE){
            if (!strcmp(car(temp)->s, car(tempTwo)->s)) {
                printf("Evaluation error: Wrong type to apply for lambda\n");
                texit(1);
            }
            tempTwo = cdr(tempTwo);
        }
        temp = cdr(temp);
    }
    if (length(args) != 2){
        printf("Evaluation error: Wrong number of arguments for lambda\n");
        texit(1);
    }
    Value *newLambda = talloc(sizeof(Value));
    newLambda->type = CLOSURE_TYPE;
    newLambda->cl.paramNames = car(args);
    newLambda->cl.functionCode = car(cdr(args));
    newLambda->cl.frame = frame;
    return newLambda;
}

// Evaluates a quote function by returning what is inside the function.
Value *evalQuote(Value *args, Frame *frame){
    if (length(args) == 1){
        return car(args);
    } else{
        printf("Evaluation error: Wrong number of arguments for quote\n");
        texit(1);
        return args;
    }
}

// Evaluates an if statement in a given frame
Value *evalIf(Value *args, Frame *frame){
    if (length(args) == 3){
        Value *argOne = eval(car(args), frame);
        Value *argTwo = car(cdr(args));
        Value *argThree = car(cdr(cdr(args)));
        if (argOne->i == 1){   
            return eval(argTwo, frame);
        } else{
            return eval(argThree, frame);
        }
    } else{
        printf("Evaluation error: Wrong number of arguments for if\n");
        texit(1);
        return args;
    }
}

// Evaluates a let statement in a given frame
Value *evalLet(Value *args, Frame *frame){
    Frame *letFrame = initializeFrame(frame);
    if (length(args) > 1){
        if (car(args)->type != CONS_TYPE && car(args)->type != NULL_TYPE){
            printf("Evaluation error: Wrong type to apply for let\n");
            texit(1);           
        }        
        int numItems = length(car(args));
        Value *temp = car(args);
        for (int i = 0; i < numItems; i++){
            if (car(temp)->type == CONS_TYPE && length(car(temp)) == 2){
                if (car(car(temp))->type != SYMBOL_TYPE){
                    printf("Evaluation error: Wrong type to apply for let\n");
                    texit(1);                        
                }
                inFrame(car(car(temp)), letFrame);
                Value *newBinding = makeNull();
                newBinding = cons(eval(car(cdr(car(temp))), frame), newBinding);
                newBinding = cons(car(car(temp)), newBinding);


                letFrame->bindings = cons(newBinding, letFrame->bindings);
            } else{
                printf("Evaluation error: Wrong number of arguments for let\n");
                texit(1);
                return args;
            }
            temp = cdr(temp);
        }
        Value *argsTwo;
        temp = cdr(args);
        while (temp->type != NULL_TYPE){ 
        argsTwo = eval(car(temp), letFrame);
        temp = cdr(temp);
        }
        return argsTwo;
    } else {
        printf("Evaluation error: Wrong number of arguments for let\n");
        texit(1);
        return args;
    }
}

// Checks that a symbol has not been defined multiple times in a frame.
Value *inFrame(Value* symbol, Frame *frame){
    Value *temp = frame->bindings;
    while(temp->type != NULL_TYPE){
        if (!strcmp(car(car(temp))->s, symbol->s)){
            printf("Evaluation error for inFrame");
            texit(1);
        }
        temp = cdr(temp);
    }
    return symbol;
}
