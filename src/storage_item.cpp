#include <cstddef>
#include <iostream>

#include "storage.hpp"
#include "storage_item.hpp"

StorageItem::StorageItem(size_t size, Storage &storage) :
	storage(storage), size(size)
{
	size_t page_size = storage.get_page_size();
	n_pages = size/page_size + ((size%page_size) ? 1 : 0);

	is_pages = new size_t[n_pages];
	for (size_t i_page = 0; i_page < n_pages; i_page++)
		is_pages[i_page] = storage.alloc();
}

StorageItem::~StorageItem() {
	for (size_t i_page = 0; i_page < n_pages; i_page++)
		storage.free(is_pages[i_page]);
	delete[] is_pages;
}

void StorageItem::access(size_t pos, size_t len, uint8_t *buffer,
	bool write
) {
	if (pos + len > size) {
		// throw?
		return;
	}

	size_t page_size = storage.get_page_size();

	size_t i_page = pos/page_size;
	pos %= page_size;
	while (len > 0) {
		size_t l = (pos + len > page_size) ? page_size - pos : len;
		storage.modify(is_pages[i_page], buffer, pos, l, write);

		i_page++;
		buffer += l;
		pos = 0;
		len -= l;
	}
}

size_t StorageItem::get_size() const
	{return size;}

void StorageItem::read(size_t pos, size_t len, uint8_t *buffer)
	{access(pos, len, buffer, false);}

void StorageItem::write(size_t pos, size_t len, uint8_t *buffer)
	{access(pos, len, buffer, true);}

std::ostream &operator<< (std::ostream &stream, const StorageItem &item) {
	stream << "Size: " << item.size << "; ";
	stream << "#pages: " << item.n_pages << "\n";

	stream << "is_pages:\n";
	for (size_t i_page = 0; i_page < item.n_pages; i_page++)
		stream << i_page << ": " << item.is_pages[i_page] << "\n";

	return stream;
}
