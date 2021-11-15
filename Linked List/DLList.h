#pragma once
#include <iostream>

template <typename T>
struct Node {

	T data;
	Node* prev;
	Node* next;

	Node() {};
	Node(T _data, Node* _prev, Node* _next) {
		this->data = _data;
		this->prev = _prev;
		this->next = _next;
	}
};

template <typename T>
class DLList {
private:
	Node<T>* first;
	Node<T>* last;

	void deleteList(Node<T>*);
	void copy(const DLList<T>&);

public:
	DLList() : first(nullptr), last(nullptr) {}
	DLList(const DLList<T>&);
	DLList<T>& operator=(const DLList<T>& );
	~DLList();

	T& getFront();
	T& getLast(); 
	void addFront(T); 
	void addAfter(T, int); 
	void addBefore(T, int);
	void addEnd(T);
	void popFront(); 
	void popBack(); 
	void removeAllElementsWithSpecifiedValue(T); 
	size_t getSize(); 
	bool isEmpty();
	friend std::ostream& operator<<(std::ostream& out, const DLList<T>& list) {

		Node<T>* curr = list.first;

		while (curr != nullptr)
		{
			out << curr->data << std::endl;
			curr = curr->next;
		}

		return out;
	}

	class Iterator {
	private:

		Node<T>* current;

	public:
		Iterator() { current = nullptr; }
		Iterator(Node<T>*);
		bool operator!=(Iterator);
		Iterator& operator++();
		Iterator operator++(int);
		bool operator==(const Iterator);
		Iterator& operator--() {
			current = current->prev;
			return *this;
		}
		T& operator*() { return current->data; }
	};

	Iterator begin() { return Iterator(this->first); }
	Iterator end() { return Iterator(nullptr); }

};

template <typename T>
void DLList<T>::deleteList(Node<T>* first) {
	Node<T>* curr = nullptr;
	while (first) {
		curr = first;
		first = first->next;
		delete curr;
	}
	first = nullptr;
}

template <typename T>
void DLList<T>::copy(const DLList<T>& other) {

	Node<T>* curr = other.first;

	while (curr != nullptr) {
		addEnd(curr->data);
		curr = curr->next;
	}
}


template <typename T>
DLList<T>::DLList(const DLList<T>& other) {
	copy(other);
}

template <typename T>
DLList<T>& DLList<T>::operator=(const DLList<T>& other) {

	if (this != &other) {
		this->deleteList(first);
		copy(other);
	}
	return *this;
}

template <typename T>
DLList<T>::~DLList() {
	this->deleteList(first);
}

template <typename T>
T& DLList<T>::getLast() {

	return this->last->data;
}

template <typename T>
T& DLList<T>::getFront() {

	return this->first->data;
}

template <typename T>
void DLList<T>::addFront(T element) {

	Node<T>* newNode = new Node<T>(element, nullptr, nullptr);

	if (first == nullptr && last == nullptr) {
		this->first = newNode;
		this->last = newNode;
	}
	else {
		newNode->next = first;
		first->prev = newNode;
		first = newNode;
	}
}

template <typename T>
void DLList<T>::addAfter(T element, int index) {

	Node<T>* newNode = new Node<T>(element, nullptr, nullptr);

	Node<T>* curr = this->first;
	Node<T>*currPrev = this->first;
	int i = 0;

	while (i < index-1)
	{
		currPrev = curr;
		curr = curr->next;
		i++;
	}
	currPrev->next = newNode;
	newNode->prev = currPrev;
	newNode->next = curr;
	curr->prev = newNode;
}

template <typename T>
void DLList<T>::addBefore(T element, int index) {
	Node<T>* newNode = new Node<T>(element, nullptr, nullptr);

	Node<T>* curr = this->first;
	Node<T>* currPrev = this->first;
	int i = 0;

	while (i < index - 1)
	{
		currPrev = curr;
		curr = curr->next;
		i++;
	}
	currPrev->next = newNode;
	newNode->prev = currPrev;
	newNode->next = curr;
	curr->prev = newNode;
}

template <typename T>
void DLList<T>::addEnd(T element) {

	Node<T>* newNode = new Node<T>(element, nullptr, nullptr);
	Node<T>* curr = last;

	if (this->isEmpty()) {
		addFront(newNode->data);
	}

	else {
		newNode->prev = curr;
		curr->next = newNode;
		last = newNode;
	}
}

template <typename T>
void DLList<T>::popFront() {

	auto toRemove = this->first;
	first = first->next;
	first->prev = nullptr;
	delete toRemove;
}

template <typename T>
void DLList<T>::popBack() {

	auto toRemove = this->last;
	this->last = last->prev;
	last->next = nullptr;
	delete toRemove;

}

template <typename T>
void DLList<T>::removeAllElementsWithSpecifiedValue(T element) {

	Node<T>* curr = first;
	while (curr->next != nullptr) {
		Node<T>* next = curr->next;
		if (curr->data == element) {
			curr->prev->next = curr->next;
			curr->next->prev = curr->prev;
			delete curr;
		}
		curr = next;
	}
}

template <typename T>
size_t DLList<T>::getSize() {

	Node<T>* curr = first;
	size_t counter = 0;

	while (curr != nullptr) {
		counter++;
		curr = curr->next;
	}
	return counter;
}

template <typename T>
bool DLList<T>::isEmpty() {
	return first == nullptr && last == nullptr;
}


template <typename T>
DLList<T>::Iterator::Iterator(Node<T>* other) {
	this->current = other;
}


template <typename T>
typename DLList<T>::Iterator& DLList<T>::Iterator::operator++() {
	current = current->next;
	return *this;
}

template <typename T>
typename DLList<T>::Iterator DLList<T>::Iterator::operator++(int) {
	
	auto cpy = *this;
	current = current->next;
	return cpy;
}

template <typename T>
bool DLList<T>::Iterator::operator==(const Iterator other) {

	return this->current == other.current;
}

template <typename T>
bool DLList<T>::Iterator::operator!=(Iterator other) {
	return !(*this == other);
}
