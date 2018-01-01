#ifndef _STORAGE_HPP
#define _STORAGE_HPP

#include <cstdint>
#include <cstddef>
#include <iostream>

struct Page {
	bool used = false;
	bool in_prim = false;
	size_t i_prim;
};

class Storage {
	private:
		const size_t page_size, n_pages_prim, n_pages_sec;
		uint8_t *memory_prim, *memory_sec;

		Page *page_table;
		size_t *is_pages_prim;

		size_t n_pages_used;
		size_t i_free;
		size_t i_swap;

	public:
		Storage(size_t page_size, size_t n_pages_prim, size_t n_pages_sec);
		~Storage();

	private:
		// i_page - index of page to be brought into primary memory
		void swap(size_t i_page);

	public:
		size_t get_page_size() const;

		size_t alloc();
		void free(size_t i_page);

		int modify(
			size_t i_page, uint8_t *buffer,
			size_t pos, size_t len,
			bool write = false
		);

	friend std::ostream &operator<< (std::ostream&, const Storage&);
};

std::ostream &operator<< (std::ostream&, const Storage&);

#endif // _STORAGE_HPP
