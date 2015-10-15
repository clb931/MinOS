#ifdef __linux__
#error "You are not using a cross-compiler!"
#endif

#ifndef __i386__
#error "You are not using an i386 compiler!"
#endif


#include "kernel/common.h"
#include "kernel/vga.h"
#include "kernel/gdt.h"
#include "kernel/idt.h"
#include "kernel/pic.h"
#include "kernel/multiboot.h"
#include "kernel/paging.h"
#include "kernel/cpuid.h"
#include "kernel/keyboard.h"
#include "kernel/heap.h"
#include "kernel/kfs.h"
#include <stdio.h>


#ifdef __cplusplus
extern "C"
{
#endif


void pass_fail(int pass, const char *str)
{
	int fg_old, bg_old;
	vga_getcolor_alt(&fg_old, &bg_old);

	vga_putchar('[');
	if (pass >= 0) {
		vga_setcolor(COLOR_GREEN, bg_old);
		vga_putstr("PASS");
	} else {
		vga_setcolor(COLOR_RED, bg_old);
		vga_putstr("FAIL");
	}
	
	vga_setcolor(fg_old, bg_old);
	vga_putstr("] ");
	vga_putstr(str);
}

void kernel_early(mb_pointer mbp)
{
	vga_init();
	vga_settitletext("MinOS v0.0.1", COLOR_BLACK, COLOR_GREEN);
	vga_setstatustext("Booting...", COLOR_BLACK, COLOR_LIGHT_GRAY);

	pass_fail(mbp.magic == 0x2BADB002, "Checking magic number\n");

	vga_putstr("Checking System Info...");
	cpuid_init();
	vga_putchar('\n');

	vga_putstr("Reading Multiboot Header...");
	mbh_init(mbp);
	vga_putchar('\n');

	pass_fail(gdt_init(), "Initializing GDT\n");
	pass_fail(idt_init(), "Initializing IDT\n");
	pass_fail(pic_init(0x20, 0x30), "Initializing PIC @ 0x20 & 0x30\n");
	pass_fail(kb_init(), "Initializing Keyboard\n");

	DWORD i = 0;
	pass_fail(i = paging_init(mbp.address), "Initializing Paging, ");
	printf("Size: %li pages\n", i);

	pass_fail(i = heap_init(0x10000000), "Initializing Heap ");
	printf("@ 0x10000000, Size: %li bytes\n", i);
		
	pass_fail(kfs_init(), "Initializing KFS\n");
}

void kernel_main()
{
	vga_setstatustext("Ready", COLOR_BLACK, COLOR_LIGHT_GRAY);
	for (int i = 0; i < 0xFF; ++i)
		vga_putchar((unsigned char)i);
	while(1);
}


#ifdef __cplusplus
}
#endif
