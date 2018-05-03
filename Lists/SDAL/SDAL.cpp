#include "SDAL.h"

#include <iostream>

int main(void) {

	SDAL<int> sdal(50);
	sdal.push_front(2);
	sdal.push_back(3);
	std::cout << "List should be 2-3:" << std::endl;
	sdal.print(std::cout); std::cout << std::endl;

	sdal.push_front(1);
	sdal.push_back(4);
	std::cout << "List should be 1-2-3-4-:" << std::endl;
	sdal.print(std::cout);
	std::cout << std::endl;
	std::cout << "Inserting 100 lements :" << std::endl;
	for (int i = 5; i < 100; ++i) {

		sdal.push_back(i);
	}
	sdal.print(std::cout);
		std::cout << "Deleting 100 elements :" << std::endl;
	for (int i = 1; i < 100; ++i) {
		if (i == 99) {
			std::cout << "";
		}
		sdal.pop_back();
	}
	sdal.print(std::cout);
	std::cout << "Inserting 100 elements other way :" << std::endl;
	for (int i = 5; i < 100; ++i) {

		sdal.push_front(i);
	}

	sdal.insert(1,0);
	std::cout << "Inserting 1 at position 1:" << std::endl;
	sdal.print(std::cout);
	sdal.replace(5, 1);
	std::cout << "Replacing 5 at position 2:" << std::endl;sdal.print(std::cout);
	sdal.print(std::cout);
	std::cout << std::endl;
	std::cout << "Length is: " << sdal.length() << std::endl;


	SDAL<int>::iterator iter = sdal.begin();
	SDAL<int>::iterator end = sdal.end();
	std::cout << *iter << std::endl;
	std::cout << *end << std::endl;


	SDAL<int>::const_iterator cIter = sdal.begin();
	SDAL<int>::const_iterator cEnd = sdal.end();
	std::cout << *cIter << std::endl;
	std::cout << *cEnd << std::endl;

	std::cout << std::endl;
	return 0;
}
