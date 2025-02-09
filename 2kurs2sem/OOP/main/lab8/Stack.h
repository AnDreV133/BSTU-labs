#pragma once

#include <vector>

template<class T>
class Stack
{
private:
	std::vector<T> *stack;
public:
	Stack() {
		stack = new std::vector<T>();
	}
	~Stack() {
		delete stack;
	}
	void add(T t) {
		stack->push_back(t);
	}

	T pop() {
		T temp = see();
		stack->pop_back();
		return temp;
	}

	T see() {
		return *(stack->end()-1);
	}

	int size() {
		return stack->size();
	}

	bool isEmpty() {
		return stack->empty();
	}

	void clear() {
		stack->clear();
	}
};

