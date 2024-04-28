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

//ss

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
	T peek() const;
	//changing priority 
	void modify_key(const T element, const T priority);
	//return size of Queue
	T return_size() const;
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

template<class T>
T PriorityQueue<T>::extract_max() {
	if (head == nullptr) {
		std::cout << "Queue is empty " << std::endl;
		return T();
	}
	T maxElement = head->element;
	Node<T>* temp = head;
	head = head->next;
	if (head != nullptr) {
		head->previous = nullptr;
	}
	delete temp;
	size--;
	std::cout << "Removed element: " << maxElement << std::endl;
	
	return maxElement;
}

template<class T>
T PriorityQueue<T>::return_size() const {
	std::cout << "The size of the PriorityQueue is: " << size << std::endl;
	return size;
}

template<class T>
T PriorityQueue<T>::peek() const {
	if (head == nullptr) {
		std::cout << "Queue is empty \n";
		return T();
	}
	std::cout << "Element with the highest priority is: " << head->element << std::endl;
	return head->element;
}
#endif