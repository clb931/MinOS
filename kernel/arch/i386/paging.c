#include "kernel/common.h"
#include "kernel/multiboot.h"
#include "kernel/paging.h"
#include "kernel/vga.h"
#include <stdio.h>
#include <string.h>


static DWORD *frame_map;
static DWORD *start_frame;
static DWORD npages = 0;

static inline DWORD create_entry(void *addr, DWORD flags)
{
	return ((DWORD)addr & PG_ADDR) | (flags & PG_FLAGS);
}

static void *page_alloc_frame()
{
	DWORD *frame = start_frame;
	for (DWORD i = 0; i < npages; ++i) {
		DWORD j = i / 32;
		DWORD k = (1 << (i % 32));
		if ((frame_map[j] & k) == 0) {
			frame_map[j] |= k;
			return (void *)frame;
		} else {
			frame += 0x400;
		}
	}

	printf("Could not allocate frame: Out of Memory!\n");
	debug_break();
	return NULL;
}

static void page_free_frame(void *addr)
{
	addr = (void *)((DWORD)addr - (DWORD)start_frame);
	DWORD i = PG_ADDR_TO_I(addr);
	DWORD j = i / 32;
	DWORD k = (1 << (i % 32));
	frame_map[j] &= ~k;
}

static void *paging_mapto(void *pAddr, void *vAddr, DWORD flags)
{
	DWORD pdi = (DWORD)vAddr >> 22;
	DWORD pti = (DWORD)vAddr >> 12 & 0x03FF;

	DWORD *pd = (DWORD *)0xFFFFF000;
	if (!PG_IS_PRESENT(pd[pdi])) {
		DWORD *page_table = (DWORD *)page_alloc_frame();
		memset(page_table, 0, 0x1000);
		pd[pdi] = create_entry(page_table, flags | PG_P);
	}

	DWORD *pt = ((DWORD *)0xFFC00000) + (0x400 * pdi);
	if (PG_IS_PRESENT(pt[pti])) {
		printf("Virtual Address (%.8lX) already in use!\n", (DWORD)vAddr);
		debug_break();
		return NULL;
	}

	pt[pti] = create_entry(pAddr, flags | PG_P);
	_invlpg(vAddr);
	return vAddr;
}

void *paging_getaddr(void *vAddr)
{
	DWORD pdi = (DWORD)vAddr >> 22;
	DWORD pti = (DWORD)vAddr >> 12 & 0x03FF;

	DWORD *pd = (DWORD *)0xFFFFF000;
	if (!PG_IS_PRESENT(pd[pdi]))
		return NULL;

	DWORD *pt = ((DWORD *)0xFFC00000) + (0x400 * pdi);
	if (!PG_IS_PRESENT(pt[pti]))
		return NULL;

	return  (void *)((pt[pti] & ~0xFFF) + ((DWORD)vAddr & 0xFFF));
}

void *paging_map(void *vAddr, DWORD flags)
{
	void *pAddr = page_alloc_frame(); 
	if (pAddr != NULL) {
		return paging_mapto(pAddr, vAddr, flags);
	} else {
		printf("Could not map address: Out of Memory!\n");
		debug_break();
		return NULL;
	}
}

void paging_unmap(void *vAddr)
{
	DWORD pdi = (DWORD)vAddr >> 22;
	DWORD pti = (DWORD)vAddr >> 12 & 0x03FF;

	DWORD *pd = (DWORD *)0xFFFFF000;
	if (!PG_IS_PRESENT(pd[pdi]))
		return;

	DWORD *pt = ((DWORD *)0xFFC00000) + (0x400 * pdi);
	if (!PG_IS_PRESENT(pt[pti]))
		return;

	page_free_frame(paging_getaddr(vAddr));
	pt[pti] = create_entry(PG_I_TO_ADDR(0), PG_R);
	_invlpg(vAddr);
}

int paging_init(void *p)
{
	DWORD kstart = (DWORD)&__kernel_start__;
	DWORD kend = (DWORD)&__kernel_end__;
	DWORD ksize = kend - kstart;
	mb_header *mbh = (mb_header *)p;
	if (mbh->flags & (1 << 1)) {
		npages = (mbh->mem.upper - ksize) / PG_SIZE;
		npages = (npages > 0x8000) ? 0x8000 : npages - 1;
	} else {
		return -1;
	}
	
	DWORD i;
	frame_map = (DWORD *)(kend + 0x1000 - (kend % 0x1000));
	start_frame = frame_map + 0x400;
	memset(frame_map, 0, 0x1000);		// Clear frame_map

	DWORD *page_directory = (DWORD *)page_alloc_frame();
	if (page_directory == NULL)
		return -2;
	DWORD *page_table_1 = (DWORD *)page_alloc_frame();
	if (page_table_1 == NULL)
		return -3;

	for (i = 0; i < 1024; ++i)	// Fill the page directory
		page_directory[i] = create_entry(PG_I_TO_ADDR(0), PG_R);
	for (i = 0; i < 1024; ++i)	// Fill the first page table
		page_table_1[i] = create_entry(PG_I_TO_ADDR(i), PG_P | PG_R);

	page_directory[0] = create_entry(page_table_1, PG_P | PG_R);
	page_directory[1023] = create_entry(page_directory, PG_P | PG_R);

	_pg_ld_dir(page_directory);
	_pg_enable();

	return npages;
}

void paging_drawdir()
{
	int fg_old, bg_old;
	vga_getcolor_alt(&fg_old, &bg_old);

	DWORD l = (VGA_WIDTH - 64) / 2;
	for (DWORD x = 0; x < l - 1; ++x)
		putchar(' ');
	printf("Page Directory:\n");

	for (DWORD x = 0; x < l - 1; ++x)
		putchar(' ');
	for (DWORD x = 0; x < 66; ++x)
		putchar('-');
	putchar('\n');

	DWORD *pd = (DWORD *)0xFFFFF000;
	for (DWORD y = 0; y < 16; ++y) {
		for (DWORD x = 0; x < l - 1; ++x)
			putchar(' ');

		putchar('|');
		vga_setcolor(COLOR_GREEN, COLOR_BLACK);
		for (DWORD x = 0; x < 64; ++x) {
			if (PG_IS_PRESENT(pd[y * 64 + x]))
				putchar(8);
			else
				putchar(7);
		}
		vga_setcolor(fg_old, bg_old);
		puts("|");
	}

	for (DWORD x = 0; x < l - 1; ++x)
		putchar(' ');
	for (DWORD x = 0; x < 66; ++x)
		putchar('-');

	putchar('\n');
}

void paging_drawtable(int index)
{
	int fg_old, bg_old;
	vga_getcolor_alt(&fg_old, &bg_old);

	DWORD l = (VGA_WIDTH - 64) / 2;
	for (DWORD x = 0; x < l - 1; ++x)
		putchar(' ');
	printf("Page Table: %i\n", index);

	for (DWORD x = 0; x < l - 1; ++x)
		putchar(' ');
	for (DWORD x = 0; x < 66; ++x)
		putchar('-');
	putchar('\n');

	DWORD *pd = (DWORD *)0xFFFFF000;
	if (!PG_IS_PRESENT(pd[index])) {
		for (DWORD x = 0; x < l - 1; ++x)
			putchar(' ');

		printf("Page not present!\n");
		return;
	}

	DWORD *pt = ((DWORD *)0xFFC00000) + (0x400 * index);
	for (DWORD y = 0; y < 16; ++y) {
		for (DWORD x = 0; x < l - 1; ++x)
			putchar(' ');

		putchar('|');
		vga_setcolor(COLOR_GREEN, COLOR_BLACK);
		for (DWORD x = 0; x < 64; ++x) {
			if (PG_IS_PRESENT(pt[y * 64 + x]))
				putchar(8);
			else
				putchar(7);
		}
		vga_setcolor(fg_old, bg_old);
		puts("|");
	}

	for (DWORD x = 0; x < l - 1; ++x)
		putchar(' ');
	for (DWORD x = 0; x < 66; ++x)
		putchar('-');

	putchar('\n');
}
