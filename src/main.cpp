#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <unordered_map>

#include "config.hpp"
#include "storage.hpp"
#include "storage_item.hpp"

typedef std::unordered_map<int, StorageItem*> ItemsMap;

int main(int argc, const char **argv) {
	size_t page_size = (argc > 1) ? std::atol(argv[1]) : DEFAULT_PAGE_SIZE;
	size_t n_pages_prim = (argc > 2) ? std::atol(argv[2]) :
		DEFAULT_N_PAGES_PRIM;
	size_t n_pages_sec = (argc > 3) ? std::atol(argv[3]) :
		DEFAULT_N_PAGES_SEC;

	Storage storage(page_size, n_pages_prim, n_pages_sec);
	ItemsMap items;
	int i_item = 0;

	std::string word;
	while (true) {
		std::cout << ">> ";
		std::cin >> word;
		if (std::cin.eof())
			break;

		if (word == "page-alloc") {
			std::cout << storage.alloc() << "\n";
		} else if (word == "page-free") {
			size_t i_page;
			std::cin >> i_page;
			storage.free(i_page);
		} else if (word == "page-read") {
			size_t i_page, pos, size;
			std::cin >> i_page >> pos >> size;
			uint8_t *buffer = new uint8_t[size];

			storage.modify(i_page, buffer, pos, size);
			std::cout << buffer << "\n";
			delete[] buffer;
		} else if (word == "page-write") {
			size_t i_page, pos, size;
			std::string data;
			std::cin >> i_page >> pos >> size >> data;
			uint8_t *buffer = new uint8_t[size];
			std::memcpy(buffer, data.data(), size);

			storage.modify(i_page, buffer, pos, size, true);
			delete[] buffer;
		} else if (word == "storage-dump") {
			std::cerr << storage;
		} else if (word == "item-create") {
			size_t size;
			std::cin >> size;
			try {
				items[++i_item] = new StorageItem(size, storage);
				std::cout << i_item << "\n";
			} catch (...) {
				std::cerr << "error: unable to create an item\n";
			}
		} else if (word == "item-delete") {
			int i;
			std::cin >> i;
			delete items[i];
			items[i] = nullptr;
		} else if (word == "item-read") {
			int i;
			size_t pos, size;
			std::cin >> i >> pos >> size;

			uint8_t *buffer = new uint8_t[size];
			if (
				(items[i] == nullptr) ||
				items[i]->read(pos, size, buffer)
			)
				std::cerr << "error: unable to read item's data\n";
			else
				std::cout << buffer << "\n";
			delete[] buffer;
		} else if (word == "item-write") {
			int i;
			size_t pos, size;
			std::string data;
			std::cin >> i >> pos >> size >> data;

			uint8_t *buffer = new uint8_t[size];
			std::memcpy(buffer, data.data(), size);
			if (
				(items[i] == nullptr) ||
				items[i]->write(pos, size, buffer)
			)
				std::cerr << "error: unable to write item's data\n";
			delete[] buffer;
		} else if (word == "item-dump") {
			int i;
			std::cin >> i;

			if (items[i] == nullptr)
				std::cout << "error";
			else
				std::cout << *items[i];
		} else if (word == "help") {
			std::cout << "page-alloc\t-> <i_page>\n";
			std::cout << "page-free\t<i_page>\n";
			std::cout << "page-read\t<i_page>\t<pos>\t<size>\t-> <data>\n";
			std::cout << "page-write\t<i_page>\t<pos>\t<size>\t<data>\n";
			std::cout << "storage-dump\t-> <storage_info>\n";
			std::cout << "item-create\t<size>\t-> <i_item>\n"; 
			std::cout << "item-delete\t<i_item>\n";
			std::cout << "item-read\t<i_item>\t<pos>\t<size>\t-> <data>\n";
			std::cout << "item-write\t<i_item>\t<pos>\t<size>\t<data>\n";
			std::cout << "item-dump\t<i_item>\t-> <item_info>\n";
		}
	}

	return 0;
}
