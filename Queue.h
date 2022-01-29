#pragma once
#include <assert.h>  
#include <iostream>

template <typename T>
class Queue {

public:

	Queue() :begin(nullptr), end(nullptr) {}
	Queue(const Queue<T>& other);
	Queue<T>& operator=(const Queue<T>& other);
	~Queue();

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
};

template <typename T>
inline void Queue<T>::clear() {

	NodeP<T>* current = begin;

	while (current != nullptr) {
		NodeP<T>* toRemove = current;
		current = current->next;
		delete toRemove;
	}
}

template <typename T>
inline void Queue<T>::copy(const NodeP<T>* start) {

	for (NodeP<T>* p = start; p != nullptr; p = p->next) {
		enqueue(p->data);
	}
}

template <typename T>
inline Queue<T>::Queue(const Queue<T>& other){
	
		copy(other.begin);
}

template <typename T>
inline Queue<T>& Queue<T>::operator=(const Queue<T>& other) {

	if (this != &other) {
		clear();
		copy(other.begin);
	}
	return *this;
}

template <typename T>
inline Queue<T>::~Queue() {

	clear();
}

template <typename T>
inline const T& Queue<T>::first()const {

	if (isEmpty()) {
		throw std::underflow_error("Empty queue");
		return T();
	}
	return begin->data;
}

template <typename T>
inline void Queue<T>::enqueue(const T& element) {

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
inline void Queue<T>::dequeue() {

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
bool Queue<T>::isEmpty()const {
	return (begin == nullptr) && (end == nullptr);
}


