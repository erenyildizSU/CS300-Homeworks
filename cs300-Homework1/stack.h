#ifndef CS300_HOMEWORK1_STACK_H
#define CS300_HOMEWORK1_STACK_H
#define SIZE 13

#include <vector>
#include <exception>
#include <string>
#include <stdexcept>
#include <iostream>

using namespace std;

template <class Object>
class Stack {

private:
    Object theArray [SIZE]{};
    int topOfStack;

public:

    Stack();
    bool isEmpty( ) const;
    bool isFull( ) const;
    //const Object & top( ) const;
    void makeEmpty( );
    void pop( );
    void push( const Object & x );
    Object topAndPop( );
    const Object& top() const;
};

#endif //CS300_HOMEWORK1_STACK_H