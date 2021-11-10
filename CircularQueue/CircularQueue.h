#pragma once

template<typename Type>
class CircularQueue {
private:
	int begin;
	int end;
	int size;
	Type* queue;
public:
	CircularQueue(int);
	Type head();
	void enQueue(Type);
	Type deQueue();
	void printQueue();
	bool isEmpty();
	
};

template <typename Type>
CircularQueue<Type>::CircularQueue(int _size) {

	this->begin = -1;
	this->end = -1;
	this->size = _size;
	this->queue = new Type[size];
}

template <typename Type>
Type CircularQueue<Type>::head() {
	if (isEmpty()) {
		throw std::out_of_range("queue is empty");
	}
	else {
		return queue[begin];
	}
}

template <typename Type>
void CircularQueue<Type>::enQueue(Type element) {

	if ((this->begin == 0 && this->end == size - 1) || (this->end == (this->begin - 1) % (this->size - 1))) {
		throw std::out_of_range("queue is full");
	}
	else if (this->begin == -1){
		this->begin = this->end = 0;
		queue[end] = element;
	}
	else {
		end++;
		queue[end] = element;
	}
}

template <typename Type>
Type CircularQueue<Type>::deQueue() {

	if (isEmpty()) {
		throw std::out_of_range("queue is empty");
	}
	
	Type data = queue[this->begin];
	queue[this->begin] = -1;
	if(this->begin == this->end){

		this->begin = -1;
		this->end = -1;
	}
	else if (this->begin == this->size - 1) {
		this->begin = 0;
	}
	else {
		this->begin++;
	}
	return data;
}

template <typename Type>
void CircularQueue<Type>::printQueue() {

	if (isEmpty()) {
		throw std::out_of_range("queue is empty");
	}

	for (int i = this->begin; i <= this->end; ++i) {
		std::cout << queue[i];
	}

}

template <typename Type>
bool CircularQueue<Type>::isEmpty() {

	return (this->begin == -1) && (this->end == -1);
}
