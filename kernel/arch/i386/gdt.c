#include "kernel/common.h"
#include "kernel/gdt.h"


typedef struct
{
	DWORD link;
	DWORD esp0;
	DWORD ss0;
	DWORD esp1;
	DWORD ss1;
	DWORD esp2;
	DWORD ss2;
	DWORD cr3;
	DWORD eip;
	DWORD eflags;
	DWORD eax;
	DWORD ecx;
	DWORD edx;
	DWORD ebx;
	DWORD esp;
	DWORD ebp;
	DWORD esi;
	DWORD edi;
	DWORD es;
	DWORD cs;
	DWORD ss;
	DWORD ds;
	DWORD fs;
	DWORD gs;
	DWORD ldtr;
	DWORD iopb_offset;
} TSS;

static GDTR gdtr;
static QWORD gdt[6];
int gdt_init()
{
	TSS tss0 = { 0 };
	
	gdt[0] = gdt_createentry(0x0, 0x0, 0x0);
	gdt[1] = gdt_createentry(0x0, 0xFFFFFFFF, GDT_CODE_PL0);
	gdt[2] = gdt_createentry(0x0, 0xFFFFFFFF, GDT_DATA_PL0);
	gdt[3] = gdt_createentry(0x0, 0xFFFFFFFF, GDT_CODE_PL3);
	gdt[4] = gdt_createentry(0x0, 0xFFFFFFFF, GDT_DATA_PL3);
	gdt[5] = gdt_createentry((DWORD)&tss0, sizeof(TSS), GDT_TSS_PL0);

	gdtr.size = sizeof(gdt) - 1;
	gdtr.offset = (DWORD)&gdt;
	_lgdt(&gdtr);

	return 0;
}

unsigned long long gdt_createentry(unsigned base, unsigned limit, unsigned flag)
{
	flag &= 0xFFFF;
	QWORD descriptor = 0;

	// Create the high 32 bit segment
	descriptor  =  limit		& 0x000F0000;	// set limit bits 19:16
	descriptor |= (flag <<  8)	& 0x00F0FF00;	// set type, p, dpl, s,
												// g, d/b, l, and avl fields
	descriptor |= (base >> 16)  & 0x000000FF;	// set base bits 23:16
	descriptor |=  base			& 0xFF000000;	// set base bits 31:24

	// Shift by 32 to allow for low part of segment
	descriptor <<= 32;

	// Create the low 32 bit segment
	descriptor |= base << 16;	//set base bits 15:0
	descriptor |= limit & 0x0000FFFF;			// set limit bits 15:0

	return descriptor;
}
