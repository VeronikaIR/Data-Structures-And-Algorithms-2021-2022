#pragma once
#include <iostream>

template <typename Type>
class Vector {
private:
	Type* vector;
	size_t curSize;
	size_t capacity;

	void copy(const Vector&);
	void resize();
	void destroy();

public:

	size_t get_curSize()const;
	Vector();
	Vector(const Vector&);
	Vector& operator=(const Vector&);
	~Vector();
	void addElement(const Type& element);
	const Type operator[](size_t index)const;
	Type& operator[](size_t index);
	Vector operator+(const Vector&)const;
	Vector& operator+=(const Vector&);
	void Print()const;
	friend std::ostream& operator>>(std::ostream&, const Vector&);

};

template <typename Type>
void Vector<Type>::copy(const Vector<Type>& other) {
	this->curSize = other.curSize;
	this->capacity = other.capacity;

	this->vector = new Type[other.capacity];
	for (size_t i = 0; i < this->curSize; ++i) {
		this->vector[i] = other.vector[i];
	}
}

template <typename Type>
void Vector<Type>::resize() {
	this->capacity *= 2;
	Type* newVect = new Type[capacity];
	for (size_t i = 0; i < this->curSize; ++i) {
		newVect[i] = this->vector[i];
	}

	delete[] this->vector;
	vector = newVect;
}

template<typename Type>
void Vector<Type>::destroy() {
	delete[] this->vector;
	this->vector = nullptr;
	this->curSize = 0;
	this->capacity = 8;
}

template <typename Type>
Vector<Type>::Vector() {
	this->curSize = 0;
	this->capacity = 8;
	this->vector = new Type[this->capacity];
}

template <typename Type>
Vector<Type>::Vector(const Vector<Type>& other) {
	this->copy(other);
}

template <typename Type>
Vector<Type>& Vector<Type>::operator=(const Vector<Type>& other) {
	if (this != &other) {
		this->destroy();
		this->copy(other);
	}
	return *this;
}

template <typename Type>
Vector<Type>::~Vector() {
	this->destroy();
}

template <typename Type>
void Vector<Type>::addElement(const Type& element) {
	if (this->curSize == this->capacity) {
		this->resize();
	}

	this->vector[curSize] = element;
	this->curSize++;

}

template <typename Type>
size_t Vector<Type>::get_curSize()const {
	return this->curSize;
}

template <typename Type>
const Type Vector<Type>::operator[](size_t index)const {
	return this->vector[index];
}

template<typename Type>
Type& Vector<Type>::operator[](size_t index) {
	return this->vector[index];
}


template <typename Type>
Vector<Type> Vector<Type>::operator+(const Vector& _vector)const {

	Vector newVector(*this);
	newVector += _vector;

	return newVector;
}


template <typename Type>
Vector<Type>& Vector<Type>::operator+=(const Vector& other) {

	for (int i = 0; i < other.curSize; ++i) {
		this->addElement(other.vector[i]);
	}
	return *this;
}

template <typename Type>
void Vector<Type>::Print()const {
	for (size_t i = 0; i < this->curSize; ++i) {
		std::cout << this->vector[i] << std::endl;
	}
}

template <typename Type>
std::ostream& operator>>(std::ostream& out, const Vector<Type>& data) {
	for (int i = 0; i < data.curSize; ++i) {
		out << data.vector[i] << std::endl;
	}
	return out;
}
