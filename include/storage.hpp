#ifndef _STORAGE_HPP
#define _STORAGE_HPP

#include <cstdint>
#include <cstddef>

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

		size_t i_swap;

	public:
		Storage(size_t page_size, size_t n_pages_prim, size_t n_pages_sec);
		~Storage();

	private:
		// i_page - index of page to be brought into primary memory
		void swap(size_t i_page);

	public:
		int modify(
			size_t i_page, uint8_t *buffer,
			size_t pos, size_t len,
			bool write = false
		);

	public:
		void dump() const;
};

#endif // _STORAGE_HPP
