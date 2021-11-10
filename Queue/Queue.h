#pragma once

template <typename Type>
class Queue {
private:
	Type* queue;
	int capacity;
	//int size;
	int begin;
	int end;
	
	
public:
	Queue(const int&);
	void push(Type);
	void pop();
	Type front();
	bool isEmpty();
	void print();

};

template <typename Type>
Queue<Type>::Queue(const int& _capacity) {

	this->queue = new Type[_capacity];
	this->capacity = _capacity;
	//this->size = 0;
	this->begin = 0;
	this->end = 0;
}

template <typename Type>
void Queue<Type>::push(Type element) {

	if (capacity == end) {
		throw std::out_of_range("out of range");
	}
	else {
		queue[end] = element;
		end++;
	}

}

template <typename Type>
void Queue<Type>::pop() {
	if (this->begin == this->end) {
		throw std::out_of_range("empty queue");
	}
	begin++;
}

template <typename Type>
Type front() {

	if (this->begin == this->end) {
		throw std::out_of_range("empty queue");
	}
	else {
		return this->queue[this->begin];
	}

}


template <typename Type>
bool Queue<Type>::isEmpty() {

	return (this->begin == this->end);
}

template <typename Type>
void Queue<Type>::print() {

	if (this->begin == this->end) {
		throw std::out_of_range("empty queue");
	}
	else {
		for (int i = this->begin; i < this->end; ++i) {
			std::cout << this->queue[i];
		}
	}
}
