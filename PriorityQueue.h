#pragma once
#include <assert.h>  
#include <iostream>

template <typename T>
class PriorityQueue {

public:

	PriorityQueue() :begin(nullptr), end(nullptr) {}
	PriorityQueue(const PriorityQueue<T>& other);
	PriorityQueue<T>& operator=(const PriorityQueue<T>& other);
	~PriorityQueue();

	const T& first()const;

	void enqueue(const T&);
	void dequeue();
	bool isEmpty()const;

private:
	template <typename T>
	struct NodeP
	{
		T data;
		NodeP* next;

		NodeP(const T& data = T(), NodeP* next = nullptr) :data(data), next(next) {}
	};

	NodeP<T>* begin;
	NodeP<T>* end;

private:
	void clear();
	void copy(const NodeP<T>* start);
	void sort(NodeP<T>* head);
	void mySwap(NodeP<T>* left, NodeP<T>* right);
};

template <typename T>
inline void PriorityQueue<T>::clear() {

	NodeP<T>* current = begin;

	while (current != nullptr) {
		NodeP<T>* toRemove = current;
		current = current->next;
		delete toRemove;
	}
}

template <typename T>
inline void PriorityQueue<T>::copy(const NodeP<T>* start) {

	for (NodeP<T>* p = start; p != nullptr; p = p->next) {
		enqueue(p->data);
	}
}

template <typename T>
inline PriorityQueue<T>::PriorityQueue(const PriorityQueue<T>& other) {
	
	copy(other.begin);
}

template <typename T>
inline PriorityQueue<T>& PriorityQueue<T>::operator=(const PriorityQueue<T>& other) {

	if (this != &other) {
		clear();
		copy(other.begin);
	}
	return *this;
}

template <typename T>
inline PriorityQueue<T>::~PriorityQueue() {

	clear();
}

template <typename T>
inline const T& PriorityQueue<T>::first()const {

	if (isEmpty()) {
		throw std::underflow_error("Empty queue");
		return T();
	}
	return begin->data;
}

template <typename T>
inline void PriorityQueue<T>::enqueue(const T& element) {

	NodeP<T>* n = new NodeP<T>(element);

	if (!isEmpty()) {
		end->next = n;
	}
	else {
		begin = n;
	}
	end = n;
}

template <typename T>
inline void PriorityQueue<T>::dequeue() {

	if (isEmpty()) {}

	NodeP<T>* p = begin;
	begin = begin->next;

	if (begin == nullptr) {
		end = nullptr;
	}
	T x = p->data;
	delete p;
}

template <typename T>
bool PriorityQueue<T>::isEmpty()const {
	return (begin == nullptr) && (end == nullptr);
}

template <typename T>
void PriorityQueue<T>::mySwap(NodeP<T>* left, NodeP<T>* right) {

	T temp = left->data;
	left->data = right->data;
	right->data = temp;
}

template <typename T>
void PriorityQueue<T>::sort(NodeP<T>* head) {

	int swapped;

	NodeP<T>* leftPtr;
	NodeP<T>* rightPtr = NULL;

	do {
		swapped = 0;
		leftPtr = head;
		while (leftPtr->next != rightPtr)
		{
			if (!(leftPtr->data < leftPtr->next->data)) {
				mySwap(leftPtr, leftPtr->next);

				swapped = 1;
			}
			leftPtr = leftPtr->next;
		}
		rightPtr = leftPtr;
	} while (swapped);

}


