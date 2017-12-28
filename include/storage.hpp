#ifndef _STORAGE_HPP
#define _STORAGE_HPP

#include <cstdint>
#include <cstddef>
#include <unordered_map>

#include "config.hpp"

typedef int storage_item;
struct PageMapEntry {
	size_t i_page;
	PageMapEntry *next;
};
typedef std::unordered_map<storage_item, PageMapEntry*> PageMap;

class Storage {
	private:
		size_t page_size, n_pages_prim, n_pages_sec;
		uint8_t *storage_prim, *storage_sec;

		PageMap page_map;
		bool *pages_used;

		size_t n_pages_used;
		size_t i_next_item;
		size_t i_free_page;

	public:
		Storage(
			size_t page_size = DEFAULT_PAGE_SIZE,
			size_t n_pages_prim = DEFAULT_N_PAGES_PRIM,
			size_t n_pages_sec = DEFAULT_N_PAGES_SEC
		);
		~Storage();

	public:
		storage_item alloc(size_t size);
		void free(storage_item);
};

#endif // _STORAGE_HPP
