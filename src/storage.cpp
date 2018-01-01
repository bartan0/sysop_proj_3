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
	is_pages_prim(new size_t[n_pages_prim]),

	n_pages_used(0), i_free(0), i_swap(0)
{
	for (size_t i_page = 0; i_page < n_pages_prim; i_page++)
		is_pages_prim[i_page] = n_pages_sec;

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
	// clock algorithm
	while (true) {
		Page &page = page_table[is_pages_prim[i_swap]];
		if (!page.used)
			break;
		if (page.old)
			break;
		else
			page.old = true;

		i_swap = (i_swap + 1)%n_pages_prim;
	}

	// copy i_swap-th page from primary memory to secondary memory
	// (if it's used)
	size_t i_page_sec = is_pages_prim[i_swap];
	if ((i_page_sec < n_pages_sec) && (page_table[i_page_sec].used)) {
		std::memcpy(
			memory_sec + i_page_sec*page_size,
			memory_prim + i_swap*page_size,
			page_size
		);
		std::cerr << "swapping out page #" << i_page_sec << "\n";
		page_table[i_page_sec].in_prim = false;
	}

	// swap-in requested page
	std::memcpy(
		memory_prim + i_swap*page_size,
		memory_sec + i_page*page_size,
		page_size
	);
	is_pages_prim[i_swap] = i_page;
	std::cerr << "swapping in page #" << i_page << "\n";
	page_table[i_page].in_prim = true;
	page_table[i_page].i_prim = i_swap;

	// increment i_swap
	i_swap = (i_swap + 1)%n_pages_prim;
}

size_t Storage::get_page_size() const
	{return page_size;}

size_t Storage::alloc() {
	if (n_pages_used == n_pages_sec)
		// throw?
		return 0;

	while (page_table[i_free].used)
		i_free = (i_free + 1)%n_pages_sec;

	page_table[i_free].used = true;
	n_pages_used++;

	return i_free;
}

void Storage::free(size_t i_page) {
	if (page_table[i_page].used) {
		page_table[i_page].used = false;
		n_pages_used--;
	}
}

int Storage::modify(
	size_t i_page, uint8_t *buffer, size_t pos, size_t len, bool write
) {
	if (pos + len > page_size)
		return 1;

	if (!page_table[i_page].in_prim)
		swap(i_page);
	page_table[i_page].old = false;

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

std::ostream &operator<< (std::ostream &stream, const Storage &storage) {
	stream << "Page size: " << storage.page_size << "; ";
	stream << "#pages (primary): " << storage.n_pages_prim << "; ";
	stream << "#pages (secondary): " << storage.n_pages_sec << "\n";

	stream << "#pages used: " << storage.n_pages_used << "; ";
	stream << "i_swap: " << storage.i_swap << "\n";

	stream << "Primary memory:\n";
	stream << storage.memory_prim << "\n";
	stream << "Secondary memory:\n";
	stream << storage.memory_sec << "\n";

	stream << "Primary mem - indices of pages:\n";
	for (size_t i_page = 0; i_page < storage.n_pages_prim; i_page++)
		stream << i_page << ": " << storage.is_pages_prim[i_page] << "\n";

	stream << "Page table:\n";
	for (size_t i_page = 0; i_page < storage.n_pages_sec; i_page++)
		stream << i_page << ": " <<
			((storage.page_table[i_page].used) ? "+" : "-") <<
			((storage.page_table[i_page].in_prim) ? "P" : "S") <<
			((storage.page_table[i_page].old) ? "X " : "  ") <<
			((storage.page_table[i_page].in_prim) ? 
				storage.page_table[i_page].i_prim : 0) << "\n";

	return stream;
}
