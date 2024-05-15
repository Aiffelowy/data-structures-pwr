#ifndef Queue
#define Queue

#include <functional>
#include <iostream>

//Node class

template<class T>
class Node {
public:
	T element;
	T priority;

	Node<T>* next;
	Node<T>* previous;

	//constructor
	Node(void) : previous(nullptr), next(nullptr) {}
	//destructor
	~Node() {}		
	//copy constructor

};

//PriorityQueue class
template<class T>
class PriorityQueue {
private:
	int size;

public:
	Node<T>* head;
	Node<T>* tail;


	//construstor
	PriorityQueue(void) : head(nullptr), tail(), size(0) {}
	//destructor
	~PriorityQueue();
	PriorityQueue(const PriorityQueue<T>& other);
	// copy assignment operator
	PriorityQueue<T>& operator=(const PriorityQueue<T>& other);

	//add to queue
	void insert(const int element, const int priority);
	//remove and return the highest priority
	T extract_max();
	//return single element with the highest priority
	T peek() const;
	//changing priority 
	void modify_key(const int elementToModify, const int newPriority);
	//return size of Queue
	T return_size() const;
	//show the Queue
	void show() const;
};

//copy constructor
template<class T>
PriorityQueue<T>::PriorityQueue(const PriorityQueue<T>& other) : head(nullptr), tail(nullptr), size(0) {
	Node<T>* current = other.head;
	while (current != nullptr) {
		insert(current->element, current->priority);
		current = current->next;
	}
}

//copy operator
template<class T>
PriorityQueue<T>& PriorityQueue<T>::operator=(const PriorityQueue<T>& other) {
	if (this == &other) {
		return *this;
	}
	// Clear existing elements
	Node<T>* current = head;
	Node<T>* next = nullptr;
	while (current != nullptr) {
		next = current->next;
		delete current;
		current = next;
	}
	head = nullptr;
	tail = nullptr;
	size = 0;
	// Copy elements from other
	current = other.head;
	while (current != nullptr) {
		insert(current->element, current->priority);
		current = current->next;
	}
	return *this;
}

//destructor
template<class T>
PriorityQueue<T>::~PriorityQueue() {
	Node<T>* current = head;
	Node<T>* next = nullptr;
	while (current != nullptr) {
		next = current->next;
		delete current;
		current = next;
	}
}

//insert method
template<class T>
void PriorityQueue<T>::insert(const int element, const int priority) {
	Node<T>* newNode = new Node<T>;
	newNode->element = element;
	newNode->priority = priority;

	if (head == nullptr || priority > head->priority) {
		newNode->next = head;
		if (head != nullptr) {
			head->previous = newNode;
		}
		head = newNode;
		if (tail == nullptr) {
			tail = newNode;
		}
	}
	// checking if it's better to add element at the end
	else if (priority <= tail->priority) {
		newNode->next = nullptr;
		newNode->previous = tail;
		tail->next = newNode;
		tail = newNode;
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

//show method
template<class T>
void PriorityQueue<T>::show() const {
	Node<T>* current = head;
	while (current != nullptr) {
		std::cout << "My priority_queue: " << current->element << "," << current->priority << std::endl;
		current = current->next;
	}
}

//extract_max method
template<class T>
T PriorityQueue<T>::extract_max() {
	if (head == nullptr) {
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
	
	return maxElement;
}

//return_size method
template<class T>
T PriorityQueue<T>::return_size() const {
	return size;
}

//peek method
template<class T>
T PriorityQueue<T>::peek() const {
	if (head == nullptr) {
		return T();
	}
	return head->element;
}

//modify_key method
template<class T>
void PriorityQueue<T>::modify_key(const int elementToModify, const int newPriority) {
	if (head == nullptr) {
	}

	Node<T>* current = head;

	while (current != nullptr && current->element != elementToModify) {
		current = current->next;
	}

	if (current == nullptr) {
		return 0;
	}

	current->priority = newPriority;

	while (current->priority != -1 && current->priority > current->previous->priority) {
		Node<T>* temp = current->previous;
		current->previous = temp->previous;
		temp->next = current->next;
		current->next = temp;
		temp->previous = current;

		if (temp->next != nullptr) {
			temp->next->previous = temp;
		}
		if (current->previous != nullptr) {
			current->previous->next = current;
		}

		if (temp == head) {
			head = current;
		}

		if (current->previous == nullptr || current->priority <= current->previous->priority) {
			break;
		}
	}
}

#endif