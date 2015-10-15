#include "kernel/common.h"
#include "kernel/vga.h"
#include "kernel/multiboot.h"
#include <stdio.h>


int mbh_init(mb_pointer mbp)
{
	int fg_old, bg_old;
	vga_getcolor_alt(&fg_old, &bg_old);

	if (mbp.magic == 0x2BADB002) {
		mb_header *mbh = (mb_header *)mbp.address;

		if (mbh->flags & (1 << 9)) {
			printf("\n    Bootloader:");
			printf("\n       \"%k%s%k\"",
				COLOR_WHITE, COLOR_BLACK,
				mbh->bootloader_name);
		}

		if (mbh->flags & (1 << 2)) {
			printf("\n    Command Line:");
			printf("\n        \"%k%s%k\"",
				COLOR_WHITE, COLOR_BLACK,
				mbh->cmd_line);
		}

		if ((mbh->flags & (1 << 3)) && mbh->mods.count) {
			printf("\n    Mods:");
			printf("\n        Count: %k%i",
				COLOR_WHITE, COLOR_BLACK,
				mbh->mods.count);
			printf("\n        Address: %k%.8X",
				COLOR_CYAN, COLOR_BLACK,
				mbh->mods.address);
		}

		if (mbh->flags & (1 << 4)) {
			printf(
				"\n    Syms: a.out"
				"\n        Table Size:  %k%u%k"
				"\n        String Size: %k%i%k"
				"\n        Address:     %k%i%k",
				COLOR_GREEN, COLOR_BLACK, mbh->syms.tab_size,
				COLOR_GREEN, COLOR_BLACK, mbh->syms.str_size,
				COLOR_CYAN, COLOR_BLACK, mbh->syms.address);
		} else if (mbh->flags & (1 << 5)) {
			printf(
				"\n    Syms: elf"
				"\n        Number:  %k%i%k"
				"\n        Size:    %k%i%k"
				"\n        Address: %k%.8lX%k"
				"\n        Shndx:   %k%i%k",
				COLOR_WHITE, COLOR_BLACK, mbh->syms.num,
				COLOR_GREEN, COLOR_BLACK, mbh->syms.size,
				COLOR_CYAN, COLOR_BLACK, mbh->syms.address,
				COLOR_WHITE, COLOR_BLACK, mbh->syms.shndx);
		}

		if (mbh->flags & (1 << 8)) {
			printf("\n    Config Table: %k%i",
				COLOR_CYAN, COLOR_BLACK,
				mbh->config_table);
		}

		if (mbh->flags & (1 << 10)) {
			printf("\n    APM Table: %k%i",
				COLOR_CYAN, COLOR_BLACK,
				mbh->apm_table);
		}

		if (mbh->flags & (1 << 1)) {
			printf("\n    Boot Device:");
			printf("\n        Device: %k0x%.2X",
				COLOR_WHITE, COLOR_BLACK,
				mbh->boot_device.drive);
		}

		if (mbh->flags & (1 << 7)) {
			printf("\n    Drives:");
			printf("\n        Length: %k%i",
				COLOR_WHITE, COLOR_BLACK,
				mbh->drives.length);
			printf("\n        Address: %k%.8X",
				COLOR_CYAN, COLOR_BLACK,
				mbh->drives.address);
		}

		if (mbh->flags & (1 << 6)) {
			printf("\n    Memory Map:");

			mb_mmap *mmap = (mb_mmap *)mbh->mmap.address;
			while ((unsigned long)mmap < mbh->mmap.address + mbh->mmap.length) {
				printf("\n        Addr: %k0x%X%.8X%k,"
						" Length: %k0x%lX%.8lX%k, type: %k0x%lX%k",
						COLOR_CYAN, COLOR_BLACK, mmap->addr_high, mmap->addr_low,
						mmap->type == 1 ? COLOR_GREEN : COLOR_LIGHT_RED, COLOR_BLACK, mmap->len_high, mmap->len_low,
						COLOR_WHITE, COLOR_BLACK, mmap->type);
				mmap = (mb_mmap *)((unsigned long)mmap + mmap->size + sizeof(mmap->size));
			}
		}
		
		if (mbh->flags & (1 << 0)) {
			printf("\n    Memory:");
			printf("\n        Lower: %k0x%X%k KB",
				COLOR_GREEN, COLOR_BLACK,
				 mbh->mem.lower);
			printf("\n        Upper: %k0x%X%k KB",
				COLOR_GREEN, COLOR_BLACK,
				mbh->mem.upper);
			printf("\n        Total: %k0x%X%k KB",
				COLOR_GREEN, COLOR_BLACK,
				mbh->mem.lower + mbh->mem.upper);
		}

		if (mbh->flags & (1 << 11)) {
			printf("\n    Graphics:");		
		}

		return 0;
	} else if (mbp.magic == 0x36D76289) {
		return -1;
	} else {
		return -2;
	}
}
