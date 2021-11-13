
#pragma once

template <typename T>
struct box
{
	T data;
	box* next;

	box() {};
	box(T _data, box* _next) : data(_data), next(_next) {
		this->data = _data;
		this->next = _next;
	}
	box(const box<T>& other) : data(other.data), next(other.next) {}

};


template <typename T>
class LList {

private:

	box<T>* first;

public:
	LList();
	LList(const LList<T>&);
	LList& operator=(const LList<T>&);
	~LList();

	void push(const T&);
	void push_back(const T&);
	void insertAfter(const T&, size_t);
	void deleteAt(size_t);
	size_t size();
	void print();
	void clear();
	bool isEmpty() const;
	bool is�xists(const T&);
	bool hasLoop();

	class Iterator {
	private:
		box<T>* current;

	public:
		Iterator() {
			current = NULL;
		}
		Iterator(box<T>*);
		bool operator!=(const Iterator&);
		Iterator& operator++();
		Iterator& operator--();
		T operator*();
	};

	Iterator begin();
	Iterator end();
};

template <typename T>
LList<T>::LList() {

	first = nullptr;
}

template <typename T>
LList<T>::LList(const LList<T>& other) {

	if (other.isEmpty()) {
		this->first = nullptr;
		return;
	}

	box<T>* otherCurrent = other.first;
	box<T>* thisCurrent = new box<T>(otherCurrent->data, nullptr);
	this->first = thisCurrent;

	while (otherCurrent->next != nullptr) {

		otherCurrent = otherCurrent->next;
		thisCurrent->next = new box<T>(otherCurrent->data, nullptr);
		thisCurrent = thisCurrent->next;
	}

}


template <typename T>
LList<T>& LList<T>::operator=(const LList<T>& other) {

	if (this != &other) {
		this->clear();

		box<T>* curr = other.first;

		while (curr != nullptr) {

			this->push_back(curr->data);
			curr = curr->next;
		}
	}
	return *this;
}

template <typename T>
LList<T>::~LList() {

	this->clear();
}

template <typename T>
void LList<T>::push(const T& _newElement) {

	this->first = new box<T>(_newElement, first);
}

template <typename T>
size_t LList<T>::size() {

	box<T>* curr = first;
	size_t counter = 0;

	while (curr != nullptr) {

		curr = curr->next;
		++counter;
	}

	return counter;
}

template <typename T>
void LList<T>::push_back(const T& element) {

	box<T>* newElement = new box<T>(element, nullptr);
	box<T>* curr = first;

	if (this->isEmpty()) {
		this->push(newElement->data);
	}
	else {

		while (curr->next != nullptr)
		{
			curr = curr->next;

		}
		curr->next = newElement;
	}
}

template <typename T>
void LList<T>::insertAfter(const T& element, size_t place) {

	box<T>* newBox = new box<T>(element, nullptr);
	box<T>* curr = first;
	box<T>* currNext = curr->next;
	size_t iterator = 1;

	if ((place > this->size()) || (place < 1)) {
		std::cout << "Invalid imput!" << std::endl;
		return;
	}

	while (iterator != place)
	{
		curr = curr->next;
		currNext = currNext->next;
		++iterator;

	}

	curr->next = newBox;
	newBox->next = currNext;
}

template <typename T>
void LList<T>::deleteAt(size_t place) {

	box<T>* curr = first;
	box<T>* currNext = curr->next;
	size_t iterator = 1;

	while (iterator < place)
	{
		curr = curr->next;
		currNext = currNext->next;
		++iterator;
	}

	curr = curr->next;
	delete[] curr->data;
}

template <typename T>
void LList<T>::print() {

	box<T>* curr = first;
	int iterator = 1;

	while (curr != nullptr) {

		std::cout << "Element " << iterator << ": " << curr->data << std::endl;
		++iterator;
		curr = curr->next;
	}
}

template <typename T>
void LList<T>::clear() {

	while (!this->isEmpty()) {
		box<T>* toDelete = first;
		first = first->next;
		delete[] toDelete;
	}
}

template <typename T>
bool LList<T>::isEmpty()const {

	return this->first == nullptr;
}

template <typename T>
bool LList<T>::is�xists(const T& element) {

	box<T>* curr = first;

	while (curr != nullptr)
	{
		if (curr->data == element) {
			return true;
		}
		curr = curr->next;

	}
	return false;

}

template <typename T>
bool LList<T>::hasLoop() {

	box<T>* slow = first;
	box<T>* fast = first;

	while (fast != nullptr && fast->next != nullptr) {
		slow = slow->next;
		fast = fast->next->next;

		if (slow == fast) {
			true;
		}
	}
	return false;
}

template <typename T>
LList<T>::Iterator::Iterator(box<T>* other) {
	this->current = other;
}

template <typename T>
bool LList<T>::Iterator::operator!=(const LList<T>::Iterator& other) {

	return this->current != other.current;
}

template <typename T>
typename LList<T>::Iterator& LList<T>::Iterator::operator++() {
	current = current->next;
	return *this;
}

template <typename T>
typename LList<T>::Iterator& LList<T>::Iterator::operator--() {

	box<T>* prev = first;

	while (prev->next != current)
	{
		prev = prev->next;
	}
	return prev;
}

template <typename T>
T LList<T>::Iterator::operator*() {
	return current->data;
}

template <typename T>
typename LList<T>::Iterator LList<T>::begin() {
	return Iterator(this->first);
}

template <typename T>
typename LList<T>::Iterator LList<T>::end() {
	return Iterator(nullptr);
}
