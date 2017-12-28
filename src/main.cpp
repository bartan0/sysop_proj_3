#include <iostream>
#include <string>
#include <vector>

#include "storage.hpp"
#include "debug.hpp"

int main()
{
	Storage storage(8);
	// std::vector<storage_item> allocs;

	std::string word;
	while (true) {
		std::cout << ">> ";
		std::cin >> word;
		if (std::cin.eof())
			break;

		if (word == "alloc") {
			size_t size;
			std::cin >> size;
			storage.alloc(size);
		} else if (word == "free") {
			storage_item item;
			std::cin >> item;
			storage.free(item);
		}
	}

	return 0;
}
