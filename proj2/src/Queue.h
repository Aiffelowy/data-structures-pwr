#ifndef Queue
#define Queue

#include <functional>
#include <iostream>

// Priority Queue based on Linked List

//check test

//Node class

template<class T>
class Node {
public:
	T element;
	T priority;

	Node<T>* next;
	Node<T>* previous;

	// constructor
	Node(void) : previous(nullptr), next(nullptr) {}
	//destructor
	~Node() {}		
};


template<class T>
class PriorityQueue {
private:
	int size;

public:
	Node<T>* head;
	Node<T> tail;


	//construstor
	PriorityQueue(void) : head(nullptr), tail(), size(0) {}
	//destructor
	~PriorityQueue(void) {}

	//add to queue
	void insert(const T element, const T priority);
	//remove and return the highest priority
	T extract_max();
	//return single element with the highest priority
	void peek();
	//changing priority 
	void modify_key(const T& element, const T& priority);
	//return size of Queue
	int return_size();
	//show the Queue
	void show() const;
};

template<class T>
void PriorityQueue<T>::insert(const T element, const T priority) {
	Node<T>* newNode = new Node<T>;
	newNode->element = element;
	newNode->priority = priority;

	if (head == nullptr || priority > head->priority) {
		newNode->next = head;
		if (head != nullptr) {
			head->previous = newNode;
		}
		head = newNode;
	}
	// searching for proper place
	else {
		Node<T>* current = head;
		while (current->next != nullptr && current->next->priority >= priority) {
			current = current->next;
		}
		newNode->next = current->next;
		newNode->previous = current;
		if (current->next != nullptr) {
			current->next->previous = newNode;
		}
		current->next = newNode;
	}
	size++;
}

template<class T>
void PriorityQueue<T>::show() const {
	Node<T>* current = head;
	std::cout << "Numbers from Priority Queue: \n";
	while (current != nullptr) {
		std::cout << "Element: " << current->element << " Priority: " << current->priority << std::endl;
		current = current->next;
	}
}

#endif