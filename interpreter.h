#ifndef _INTERPRETER
#define _INTERPRETER



// Takes in a frame and creates a new frame with the frame that was passed in as the parent.
Frame *initializeFrame(Frame *frame);


// Loops through the tree and evaluates the functions contained within the tree
void interpret(Value *tree);


// Takes in a subtree and a frame and evaluates the statement in the given frame.
Value *eval(Value *tree, Frame *frame);


// Add primitive functions to top-level bindings list
void bind(char *name, Value *(*function)(struct Value *), Frame *frame);


// Primitive function for less than
Value *primitiveLess(Value *args);


// Primitive function for greater than
Value *primitiveGreater(Value *args);


// Primitive function for =
Value *primitiveEqual(Value *args);


// Primitive function for multiply
Value *primitiveMultiply(Value *args);


// Primitive function for divide
Value *primitiveDivide(Value *args);


// Primitive function for modulo
Value *primitiveMod (Value *args);


// Primitive function for minus
Value *primitiveMinus(Value *args);


// evaluates cons which has two arguments and returns a cons cell with the first in the car and the second in the cdr
Value *primitiveCons(Value *args);


// evaluates null? and returns true if the argument is null and false if it isn't
Value *primitiveNull(Value *args);


// Returns the value in the car of the cons cell that is passed in
Value *primitiveCar(Value *args);


// Returns the value in the cdr of the cons cell that is passed in
Value *primitiveCdr(Value *args);


// Takes in a variable number of parameters and adds them all together and returns the result
Value *primitiveAdd (Value* args);


// Takes in a symbol and a frame and tries to find the value of that symbol in the given frame.
// If it does not find the symbol it checks the parent frame.
Value *lookUpSymbol(Value *symbol, Frame *frame);


// Takes an operator and parameters and runs the code from the operator with the parameters.
Value *apply(Value *operator, Value *args);


// Evaluates each argument that is passed in recursively and returns them in a linked list
Value *evalEach(Value *args, Frame *frame);


// Evaluates conditionals
Value *evalCond(Value *args, Frame *frame);


// Evaluates an or statement
Value *evalOr(Value *args, Frame *frame);


// Evaluates an and statement
Value *evalAnd(Value *args, Frame *frame);


// Evaluates a begin statement
Value *evalBegin(Value *args, Frame *frame);


// Evaluates a letrec statement
Value *evalLetRec(Value *args, Frame *frame);


// Evaluates a let* statement
Value *evalLetStar(Value *args, Frame *frame);


// Evaluates a define statement and binds the first item (the symbol) to a value in a given frame
Value *evalDefine (Value *args, Frame *frame);


// Evaluates a set! statement
Value *evalSet(Value *args, Frame *frame);


// Evaluates a lambda and creates a new closure which contains parameter names, the function code and the frame.
Value *evalLambda (Value *args, Frame *frame);


// Evaluates a quote function by returning what is inside the function.
Value *evalQuote(Value* args, Frame *frame);


// Evaluates an if statement in a given frame
Value *evalIf(Value *args, Frame *frame);


// Evaluates a let statement in a given frame
Value *evalLet(Value *args, Frame *frame);


// Checks that a symbol has not been defined multiple times in a frame.
Value *inFrame(Value* symbol, Frame *frame);


#endif

