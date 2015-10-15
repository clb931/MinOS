#include "kernel/common.h"
#include "kernel/vga.h"
#include "kernel/paging.h"
#include "kernel/heap.h"
#include <stdio.h>


#define BLOCK_SIG_FREE 0xB10CB10C
#define BLOCK_SIG_USED 0xA10CA10C
#define BLOCK_IS_FREE(x) ((x->sig) == BLOCK_SIG_FREE)
#define IS_BLOCK(x) (((x->sig) == BLOCK_SIG_FREE) || ((x->sig) == BLOCK_SIG_USED))


typedef struct {
	DWORD sig;
	int end		: 1;
	int size	: 31;
} block_header_t;

void *heap = NULL;
int heap_size = 0;
int heap_chunks = 0;

static DWORD growHeap(DWORD size)
{
	DWORD i = 0;
	void *ptr = paging_map(heap + ((i+1) * 0x1000), PG_R);
	if (ptr != NULL)
		i++;

	while ((i < size) && ptr != NULL) {
		ptr = paging_map(heap + ((i+1) * 0x1000), PG_R);
		if (ptr != NULL)
			i++;
	}

	heap_size += i;
	return i;
}

static void shrinkHeap()
{
	
}

int heap_init(unsigned addr)
{
	heap = paging_map((void *)addr, PG_R);
	if (heap == NULL)
		return -1;

	heap_size++;
	growHeap(255);

	block_header_t *hdr = (block_header_t *)heap;
	hdr->sig = BLOCK_SIG_FREE;
	hdr->end = 1;
	hdr->size = (heap_size * 4 * 1024) - sizeof(block_header_t);

	return hdr->size;
}

void *kalloc(int size)
{
	int header_size = sizeof(block_header_t);
	block_header_t *hdr = (block_header_t *)heap;

	while (!BLOCK_IS_FREE(hdr) || hdr->size < size) {
		hdr = (block_header_t *)((BYTE *)hdr + header_size + hdr->size);
		if (hdr->end == 1 && (!BLOCK_IS_FREE(hdr) || hdr->size < size)) {
			printf("%kCould not allocate block: Heap out of memory!\n", COLOR_RED, COLOR_BLACK);
			return NULL;
		}
	}

	block_header_t prev = *hdr;
	prev.sig = BLOCK_SIG_FREE;
	prev.size -= (size + header_size);

	void *ptr = (void *)((BYTE *)hdr + header_size);
	hdr->sig = BLOCK_SIG_USED;
	hdr->size = hdr->size;
	if (prev.size > header_size) {
		hdr->end = 0;
		hdr->size = size;
		hdr = (block_header_t *)((BYTE *)hdr + header_size + hdr->size);
		*hdr = prev;
		heap_chunks++;
	}
	
	return ptr;
}

void kfree(void *ptr)
{
	block_header_t *hdr = (block_header_t *)((BYTE *)ptr - sizeof(block_header_t));
	block_header_t *next = (block_header_t *)((BYTE *)ptr + hdr->size);

	if (hdr->sig != BLOCK_SIG_USED) {
		printf("%kCould not free\n", COLOR_RED, COLOR_BLACK);
		debug_break();
		return;
	}

	hdr->sig = BLOCK_SIG_FREE;
	if (BLOCK_IS_FREE(next)) {
		hdr->size += next->size + sizeof(block_header_t);
		heap_chunks--;
	}
}

void heap_drawchunk(int index)
{
	int fg_old, bg_old;
	vga_getcolor_alt(&fg_old, &bg_old);
	DWORD l = (VGA_WIDTH - 16) / 2;

	for (DWORD x = 0; x < l - 1; ++x)
		putchar(' ');
	for (DWORD x = 0; x < 18; ++x)
		putchar('-');
	putchar('\n');

	for (DWORD x = 0; x < l - 1; ++x)
		putchar(' ');
	printf("| Chunk %.8x |\n", index);

	for (DWORD x = 0; x < l - 1; ++x)
		putchar(' ');
	printf("|----------------|\n");

	block_header_t *hdr = (block_header_t *)heap;
	for (int i = 0; i < index; ++i) {
		if (i >= heap_chunks) {
			for (DWORD x = 0; x < l - 1; ++x)
				putchar(' ');

			printf("| Not Present!   |\n");
			for (DWORD x = 0; x < l - 1; ++x)
				putchar(' ');
			for (DWORD x = 0; x < 18; ++x)
				putchar('-');
			putchar('\n');

			return;
		}

		hdr = (block_header_t *)((BYTE *)hdr + sizeof(block_header_t) + hdr->size);
	}

	for (DWORD x = 0; x < l - 1; ++x)
		putchar(' ');
	printf("|  Sig: %.8lX |\n", hdr->sig);

	for (DWORD x = 0; x < l - 1; ++x)
		putchar(' ');
	printf("| Free: %i        |\n", BLOCK_IS_FREE(hdr));

	for (DWORD x = 0; x < l - 1; ++x)
		putchar(' ');
	printf("| Size: %.8i |\n", hdr->size);

	for (DWORD x = 0; x < l - 1; ++x)
		putchar(' ');
	printf("| Addr: %.8lX |\n", (DWORD)hdr);


	for (DWORD x = 0; x < l - 1; ++x)
		putchar(' ');
	for (DWORD x = 0; x < 18; ++x)
		putchar(('-'));

	putchar('\n');
}
