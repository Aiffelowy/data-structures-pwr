#include <iostream>
#include "Double_Linked_List.h"

int main() {

	DoubleLinkedList<int> list;

	

	
	list.push_head(4);
	list.push_tail(64);
	list.push(1, 10);
	list.push(3,9);
	list.pop(2);
	list.show();
	std::cout << std::endl;
	// list.show_backward();
	std::cout << list.search(2);
	return 0;
}