#include <cstdint>
#include <cstddef>
#include <cstring>

#include <iostream>

#include "storage.hpp"

Storage::Storage(
	size_t page_size,
	size_t n_pages_prim,
	size_t n_pages_sec
) :
	page_size(page_size),
	n_pages_prim(n_pages_prim),
	n_pages_sec(n_pages_sec),

	memory_prim(new uint8_t[n_pages_prim*page_size]),
	memory_sec(new uint8_t[n_pages_sec*page_size]),

	page_table(new Page[n_pages_sec]()),
	is_pages_prim(new size_t[n_pages_prim]()),

	i_swap(0)
{
	// debug only
	for (size_t i = 0; i < n_pages_prim*page_size; i++)
		memory_prim[i] = '_';
	for (size_t i = 0; i < n_pages_sec*page_size; i++)
		memory_sec[i] = '_';
}

Storage::~Storage() {
	delete[] memory_prim;
	delete[] memory_sec;
	delete[] page_table;
	delete[] is_pages_prim;
}

void Storage::swap(size_t i_page) {
	// copy i_swap-th page from primary memory to secondary memory
	// (if it's used)
	size_t i_page_sec = is_pages_prim[i_swap];
	if (page_table[i_page_sec].used) {
		std::memcpy(
			memory_sec + i_page_sec*page_size,
			memory_prim + i_swap*page_size,
			page_size
		);
		page_table[i_page_sec].in_prim = false;
	}

	// swap-in requested page
	std::memcpy(
		memory_prim + i_swap*page_size,
		memory_sec + i_page*page_size,
		page_size
	);
	is_pages_prim[i_swap] = i_page;
	page_table[i_page].in_prim = true;
	page_table[i_page].i_prim = i_swap;

	// probably not the right place; debug
	page_table[i_page].used = true;

	// increment i_swap
	i_swap = (i_swap + 1)%n_pages_prim;
}

int Storage::modify(
	size_t i_page, uint8_t *buffer, size_t pos, size_t len, bool write
) {
	if (pos + len > page_size)
		return 1;

	if (!page_table[i_page].in_prim)
		swap(i_page);

	if (write)
		std::memcpy(
			memory_prim + page_table[i_page].i_prim*page_size + pos,
			buffer,
			len
		);
	else
		std::memcpy(
			buffer,
			memory_prim + page_table[i_page].i_prim*page_size + pos,
			len
		);

	return 0;
}

void Storage::dump() const {
	std::cout << "Page size: " << page_size << "\n";
	std::cout << "#Pages (primary): " << n_pages_prim << "\n";
	std::cout << "#Pages (secondary): " << n_pages_sec << "\n\n";

	std::cout << "Primary memory:\n" << memory_prim << "\n";
	std::cout << "Secondary memory:\n" << memory_sec << "\n";

	std::cout << "Page table:\n";
	for (size_t i_page = 0; i_page < n_pages_sec; i_page++)
		std::cout << page_table[i_page].used << " " <<
			page_table[i_page].in_prim << " " <<
			page_table[i_page].i_prim << "\n";
}
