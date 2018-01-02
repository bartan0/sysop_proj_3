#ifndef _STORAGE_ITEM_HPP
#define _STORAGE_ITEM_HPP

#include <cstdint>
#include <cstddef>
#include <iostream>

#include "storage.hpp"

class StorageItem {
	private:
		Storage &storage;
		size_t size;
		size_t n_pages;
		size_t *is_pages;

	public:
		StorageItem(size_t size, Storage&);
		~StorageItem();

	private:
		int access(size_t pos, size_t len, uint8_t *buffer, bool write);

	public:
		size_t get_size() const;

		int read(size_t pos, size_t len, uint8_t *buffer);
		int write(size_t pos, size_t len, uint8_t *buffer);

	friend std::ostream &operator<< (std::ostream&, const StorageItem&);
};

std::ostream &operator<< (std::ostream&, const StorageItem&);

#endif // _STORAGE_ITEM_HPP
