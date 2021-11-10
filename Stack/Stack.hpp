#pragma once
#define MAX 1000
#include <iostream>

template <typename Type>
class Stack {

	int top;
public:
	Type data[MAX];

	Stack() { top = -1; }
	int getSize();
	bool push(Type);
	Type pop();
	Type peek();
	bool isEmpty();
	void print();
};

template <typename Type>
int Stack<Type>::getSize() {
	return this->top;
}


template <typename Type>
bool Stack<Type>::push(Type element) {

	if (top >= MAX - 1) {
		throw "Stack Overflow";
		return false;
	}

	data[++top] = element;
	return true;
}

template <typename Type>
Type Stack<Type>::pop() {

	if (top < 0) {
		throw "Stack Underflow";
		return 0;
	}
	else {
		Type x = data[top--];
		return x;
	}
}

template <typename Type>
Type Stack<Type>::peek() {
	if (top < 0) {
		return 0;
	}
	else {

		return data[top];
	}

}

template <typename Type>
bool Stack<Type>::isEmpty() {

	return (top < 0);
}

template <typename Type>
void Stack<Type>::print() {

	for (int i = this->getSize(); i >= 0; i--) {
		std::cout << data[i];
	}
}


