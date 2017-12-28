#include <cstdint>

#include "storage.hpp"
#include "debug.hpp"

Storage::Storage(
	size_t page_size,
	size_t n_pages_prim,
	size_t n_pages_sec
) :
	page_size(page_size),
	n_pages_prim(n_pages_prim),
	n_pages_sec(n_pages_sec),
	n_pages_used(0),
	i_next_item(1),
	i_free_page(0)
{
	storage_prim = new uint8_t[n_pages_prim*page_size];
	storage_sec= new uint8_t[n_pages_sec*page_size];
	pages_used = new bool[n_pages_sec]();

	debug_log("storage: created\n");
	debug_logf("\tpage_size: %ld\n", page_size);
	debug_logf("\tn_pages_prim: %ld\n", n_pages_prim);
	debug_logf("\tn_pages_sec: %ld\n", n_pages_sec);
}

Storage::~Storage() {
	delete[] storage_prim;
	delete[] storage_sec;

	debug_log("storage: destroyed\n");
}

storage_item Storage::alloc(size_t size) {
	debug_log("storage: allocation request\n");
	debug_logf("\trequested size [bytes]: %ld\n", size);

	size_t n_pages = size/page_size + ((size%page_size) ? 1 : 0);
	debug_logf("\trequested size [pages]: %ld\n", n_pages);
	debug_logf("\t#pages already allocated: %ld\n", n_pages_used);
	if (n_pages_used + n_pages > n_pages_sec) {
		debug_log("storage: allocation request failed\n");
		debug_log("\treason: no free pages\n");
		return 0;
	}
	debug_log("storage: allocation request ok\n");

	page_map[i_next_item] = nullptr;

	for (size_t i_page = 0; i_page < n_pages; i_page++) {
		while (pages_used[i_free_page])
			i_free_page = (i_free_page + 1)%n_pages_sec;

		PageMapEntry *pme = new PageMapEntry;
		pme->i_page = i_free_page;
		pme->next = page_map[i_next_item];
		page_map[i_next_item] = pme;

		pages_used[i_free_page] = true;

		debug_logf("\tallocated page #%ld\n", i_free_page);
	}
	n_pages_used += n_pages;

	debug_logf("storage: allocated as item #%ld\n", i_next_item);
	return i_next_item++;
}

void Storage::free(storage_item item) {
	debug_log("storage: free request\n");
	debug_logf("\titem #: %d\n", item);

	PageMapEntry *pme = page_map[item], *tmp;
	while (pme) {
		debug_logf("freed page #%ld\n", pme->i_page);

		pages_used[pme->i_page] = false;
		tmp = pme->next;
		delete pme;
		pme = tmp;
		n_pages_used--;
	}
}
