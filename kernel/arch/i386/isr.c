#include "kernel/common.h"
#include "kernel/isr.h"
#include "kernel/vga.h"
#include "kernel/portio.h"
#include "kernel/pic.h"
#include "kernel/keyboard.h"
#include <stdio.h>
#include <stdlib.h>


void isr_int_unhandled()
{
	printf("Interupt: Unhandled\n", COLOR_BLACK, COLOR_RED);
	pic_sendEOI(0x0F);
}

void isr_fault_de()
{
	printf("%k *** Exception: Division-by-zero Error *** %k\n", COLOR_BLACK, COLOR_RED);
	pic_sendEOI(0x00);
}

void isr_trap_db()
{
	BYTE result;
	vga_setstatustext("Debug Break: Press SPACE to continue...", COLOR_BLACK, COLOR_MAGENTA);

	do {
		result = inb(PORT_KB);
	} while (kb_getkeycode(result) != ' ');
	outb(PORT_KB, CMD_ECHO);

	pic_sendEOI(0x01);
}

void isr_int_nonmaskable()
{
	printf("%k *** Exception: Non-maskable Interrupt *** %k\n", COLOR_BLACK, COLOR_RED);
	pic_sendEOI(0x02);
}

void isr_trap_bp()
{
	printf("%k *** Exception: Breakpoint *** %k\n", COLOR_BLACK, COLOR_RED);
	pic_sendEOI(0x03);
}

void isr_trap_of()
{
	printf("%k *** Exception: Overflow *** %k\n", COLOR_BLACK, COLOR_RED);
	pic_sendEOI(0x04);
}

void isr_fault_br()
{
	printf("%k *** Exception: Bound Range Exceeded *** %k\n", COLOR_BLACK, COLOR_RED);
	pic_sendEOI(0x05);
}

void isr_fault_ud()
{
	printf("%k *** Exception: Invalid Opcode *** %k\n", COLOR_BLACK, COLOR_RED);
	pic_sendEOI(0x06);
}

void isr_fault_nm()
{
	printf("%k *** Exception: Device Not Available *** %k\n", COLOR_BLACK, COLOR_RED);
	pic_sendEOI(0x07);
}

void isr_abort_df()
{
	DWORD error = 0;
	asm("mov (%%esp), %0" :"=r"(error));
	printf("%k *** Exception: Double Fault (0x%.8x) *** %k\n", COLOR_BLACK, COLOR_RED, error);
	pic_sendEOI(0x08);
	abort();
}

void isr_fault_cso()
{
	printf("%k *** Exception: Coprocessor Segment Overrun *** %k\n", COLOR_BLACK, COLOR_RED);
	pic_sendEOI(0x09);
}

void isr_fault_ts()
{
	DWORD error = 0;
	asm("mov (%%esp), %0" :"=r"(error));
	printf("%k *** Exception: Invalid TSS (0x%.8x) *** %k\n", COLOR_BLACK, COLOR_RED, error);
	pic_sendEOI(0x0A);
}

void isr_fault_np()
{
	DWORD error = 0;
	asm("mov (%%esp), %0" :"=r"(error));
	printf("%k *** Exception: Segment Not Present (0x%.8x) *** %k\n", COLOR_BLACK, COLOR_RED, error);
	pic_sendEOI(0x0B);
}

void isr_fault_ss()
{
	DWORD error = 0;
	asm("mov (%%esp), %0" :"=r"(error));
	printf("%k *** Exception: Stack-Segment Fault (0x%.8x) *** %k\n", COLOR_BLACK, COLOR_RED, error);
	pic_sendEOI(0x0C);
}

void isr_fault_gp()
{
	DWORD error = 0;
	asm("mov (%%esp), %0" :"=r"(error));
	printf("%k *** Exception: General Protection Fault (0x%.8x) *** %k\n", COLOR_BLACK, COLOR_RED, error);
	pic_sendEOI(0x0D);
}

void isr_fault_pf()
{
	DWORD error = 0;
	asm("mov (%%esp), %0" :"=r"(error));
	printf("%k *** Exception:Page Fault (0x%.8x) *** %k\n", COLOR_BLACK, COLOR_RED, error);
	//printf("P: %i RW: %i US: %i\n", error & 1, (error >> 1) & 1, (error >> 2) & 1);
	pic_sendEOI(0x0E);
}

void isr_fault_mf()
{
	printf("%k *** Exception: x87 Floating-Point Exception *** %k\n", COLOR_BLACK, COLOR_RED);
	pic_sendEOI(0x10);
}

void isr_fault_ac()
{
	DWORD error = 0;
	asm("mov (%%esp), %0" :"=r"(error));
	printf("%k *** Exception: Alignment Check (0x%.8x) *** %k\n", COLOR_BLACK, COLOR_RED, error);
	pic_sendEOI(0x11);
}

void isr_abort_mc()
{
	printf("%k *** Exception: Machine Check *** %k\n", COLOR_BLACK, COLOR_RED);
	pic_sendEOI(0x12);
	abort();
}

void isr_fault_xf()
{
	printf("%k *** Exception: SIMD Floating-Point Exception *** %k\n", COLOR_BLACK, COLOR_RED);
	pic_sendEOI(0x13);
}

void isr_fault_ve()
{
	printf("%k *** Exception: Virtualization Exception *** %k\n", COLOR_BLACK, COLOR_RED);
	pic_sendEOI(0x14);
}

void isr_fault_sx()
{
	DWORD error = 0;
	asm("mov (%%esp), %0" :"=r"(error));
	printf("%k *** Exception: Security Exception (0x%.8x) *** %k\n", COLOR_BLACK, COLOR_RED, error);
	pic_sendEOI(0x1E);
}

void isr_timer()
{
	static int i = 0;

	char str[33] = "";
	itoa(i++, str, 10);
	vga_settimer(str);

	pic_sendEOI(0x20);
}

void isr_keyboard()
{
	BYTE result = inb(PORT_KB);
	if (result >= 0x80) {
		io_wait();
		result = inb(PORT_KB);
		kb_send_keyup(result);
	} else {
		kb_send_keydown(result);
	}
	pic_sendEOI(0x21);
}

void isr_cascade()
{
	printf("Interupt: cascade\n");
	pic_sendEOI(0x22);
}

void isr_com2()
{
	printf("Interupt: com2\n");
	pic_sendEOI(0x23);
}

void isr_com1()
{
	printf("Interupt: com1\n");
	pic_sendEOI(0x24);
}

void isr_lpt2()
{
	printf("Interupt: lpt2\n");
	pic_sendEOI(0x25);
}

void isr_floppy()
{
	printf("Interupt: floppy\n");
	pic_sendEOI(0x26);
}

void isr_lpt1()
{
	printf("Interupt: lpt1\n");
	pic_sendEOI(0x27);
}

void isr_clock()
{
	printf("Interupt: clock\n");
	pic_sendEOI(0x30);
}

void isr_free1()
{
	printf("Interupt: free1\n");
	pic_sendEOI(0x31);
}

void isr_free2()
{
	printf("Interupt: free2\n");
	pic_sendEOI(0x32);
}

void isr_free3()
{
	printf("Interupt: free3\n");
	pic_sendEOI(0x33);
}

void isr_mouse()
{
	printf("Interupt: mouse\n");
	pic_sendEOI(0x34);
}

void isr_coprocessor()
{
	printf("Interupt: coprocessor\n");
	pic_sendEOI(0x35);
}

void isr_ata1()
{
	printf("Interupt: ata1\n");
	pic_sendEOI(0x36);
}

void isr_ata2()
{
	printf("Interupt: ata2\n");
	pic_sendEOI(0x37);
}

