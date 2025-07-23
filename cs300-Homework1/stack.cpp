#include "stack.h"
#include <iostream>
#include <stdexcept>
#include <exception>

using namespace std;

// Constructor
template <class Object>
Stack<Object>::Stack() : topOfStack(-1) {
    // Initialize the top of the stack
}

// Check if stack is empty
template <class Object>
bool Stack<Object>::isEmpty() const {
    return topOfStack == -1;
}

// Check if stack is full
template <class Object>
bool Stack<Object>::isFull() const {
    // return topOfStack == theArray.size() - 1;
    return topOfStack == SIZE - 1;
}

//BAKKKKKKKKKKKKKKKKKKK
// Return the top element of the stack without popping it
template <class Object>
const Object& Stack<Object>::top() const {
//    if (isEmpty()) {
////        static Object defaultObject = Object();
////        return defaultObject;
//        //std::cerr << "Stack is empty, cannot retrieve top element." << std::endl;
//        //exit(1);  // Terminate program
////        throw std:: underflow_error("Stack is empty, cannot retrieve top element.");
//    }
    if (topOfStack >=0) {
        return theArray[topOfStack];  // Return the top element
    }
}


/**
* Get the most recently inserted item in the stack.
* Does not alter the stack.
* Return the most recently inserted item in the stack.
* Exception Underflow if stack is already empty.
*/

// Return the top element without popping it
//const Object& top() const {
//    if (isEmpty()) {
//        std::cerr << "Error: Stack is empty. Returning default object." << std::endl;
//        static Object dummy; // Return a dummy object (this can be any default value)
//        return dummy;
//    }
//    return theArray[topOfStack];
//}


// Make the stack logically empty.
template <class Object>
void Stack<Object>::makeEmpty() {
    topOfStack = -1;
}

//BAKKKKKKKKKKKKKKKKKKKKKKK

//Remove the most recently inserted item from the stack.
//Exception Underflow if stack is already empty.


// Pop the top element from the stack
template <class Object>
void Stack<Object>::pop() {
//    if (isEmpty()) {
//        std::cerr << "Stack underflow, cannot pop." << std::endl;
//        //throw Underflow();
//        //throw std::underflow_error("Stack is empty");
//    }
    if(topOfStack >=0) {
        topOfStack--;
    }
}

//// Pop the top element from the stack
//template <class Object>
//void Stack<Object>::pop() {
//    if (!isEmpty()) {
//        --topOfStack;
//    }
//}



// Push an element onto the stack
template <class Object>
void Stack<Object>::push(const Object & x) {
    theArray[++topOfStack] = x;
}

//template <class Object>
//void Stack<Object>::push(const Object &x) {
//    if (!isFull()) {
//        theArray[++topOfStack] = x;
//    }
//}

// Retrieve the top element and pop it from the stack
template <class Object>
Object Stack<Object>::topAndPop() {
//    if (isEmpty()) {
//        throw Underflow();
//    }
    return theArray[topOfStack--];
}