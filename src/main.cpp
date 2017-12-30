#include <cstring>
#include <iostream>
#include <string>

#include "storage.hpp"

int main() {
	Storage storage(4, 4, 8);

	std::string word;
	while (true) {
		std::cout << ">> ";
		std::cin >> word;
		if (std::cin.eof())
			break;

		if (word == "read") {
			size_t i_page, pos, len;
			std::cin >> i_page >> pos >> len;
			uint8_t *buffer = new uint8_t[len];

			if (storage.modify(i_page, buffer, pos, len))
				std::cout << "error\n";
			else
				std::cout << buffer << "\n";
			delete[] buffer;
		} else if (word == "write") {
			size_t i_page, pos;
			std::string data;
			std::cin >> i_page >> pos >> data;
			uint8_t *buffer = new uint8_t[data.length()];
			std::memcpy(buffer, data.data(), data.length());

			if (storage.modify(i_page, buffer, pos, data.length(), true))
				std::cout << "error\n";
			delete[] buffer;
		} else if (word == "dump") {
			storage.dump();
		}
	}

	return 0;
}
