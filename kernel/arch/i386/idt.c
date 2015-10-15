#include "kernel/common.h"
#include "kernel/idt.h"


static IDTR idtr;
static QWORD idt[256];
int idt_init()
{
	DWORD isr = 0;
	asm("movl $wrapper_int_unhandled, %0" :"=r"(isr));
	for(int i = 0; i < 256; ++i)
		idt[i] = idt_createentry(isr, 0x08, IDT_INT_PL0);

	asm("movl $wrapper_fault_de, %0" :"=r"(isr));
	idt[0x00] = idt_createentry(isr, 0x08, IDT_INT_PL0);
	asm("movl $wrapper_trap_db, %0" :"=r"(isr));
	idt[0x01] = idt_createentry(isr, 0x08, IDT_TRP_PL0);
	asm("movl $wrapper_int_nonmaskable, %0" :"=r"(isr));
	idt[0x02] = idt_createentry(isr, 0x08, IDT_INT_PL0);
	asm("movl $wrapper_trap_bp, %0" :"=r"(isr));
	idt[0x03] = idt_createentry(isr, 0x08, IDT_TRP_PL0);
	asm("movl $wrapper_trap_of, %0" :"=r"(isr));
	idt[0x04] = idt_createentry(isr, 0x08, IDT_TRP_PL0);
	asm("movl $wrapper_fault_br, %0" :"=r"(isr));
	idt[0x05] = idt_createentry(isr, 0x08, IDT_INT_PL0);
	asm("movl $wrapper_fault_ud, %0" :"=r"(isr));
	idt[0x06] = idt_createentry(isr, 0x08, IDT_INT_PL0);
	asm("movl $wrapper_fault_nm, %0" :"=r"(isr));
	idt[0x07] = idt_createentry(isr, 0x08, IDT_INT_PL0);
	asm("movl $wrapper_abort_df, %0" :"=r"(isr));
	idt[0x08] = idt_createentry(isr, 0x08, IDT_INT_PL0);
	asm("movl $wrapper_fault_cso, %0" :"=r"(isr));
	idt[0x09] = idt_createentry(isr, 0x08, IDT_INT_PL0);
	asm("movl $wrapper_fault_ts, %0" :"=r"(isr));
	idt[0x0A] = idt_createentry(isr, 0x08, IDT_INT_PL0);
	asm("movl $wrapper_fault_np, %0" :"=r"(isr));
	idt[0x0B] = idt_createentry(isr, 0x08, IDT_INT_PL0);
	asm("movl $wrapper_fault_ss, %0" :"=r"(isr));
	idt[0x0C] = idt_createentry(isr, 0x08, IDT_INT_PL0);
	asm("movl $wrapper_fault_gp, %0" :"=r"(isr));
	idt[0x0D] = idt_createentry(isr, 0x08, IDT_INT_PL0);
	asm("movl $wrapper_fault_pf, %0" :"=r"(isr));
	idt[0x0E] = idt_createentry(isr, 0x08, IDT_INT_PL0);
	asm("movl $wrapper_fault_mf, %0" :"=r"(isr));
	idt[0x10] = idt_createentry(isr, 0x08, IDT_INT_PL0);
	asm("movl $wrapper_fault_ac, %0" :"=r"(isr));
	idt[0x11] = idt_createentry(isr, 0x08, IDT_INT_PL0);
	asm("movl $wrapper_abort_mc, %0" :"=r"(isr));
	idt[0x12] = idt_createentry(isr, 0x08, IDT_INT_PL0);
	asm("movl $wrapper_fault_xf, %0" :"=r"(isr));
	idt[0x13] = idt_createentry(isr, 0x08, IDT_INT_PL0);
	asm("movl $wrapper_fault_ve, %0" :"=r"(isr));
	idt[0x14] = idt_createentry(isr, 0x08, IDT_INT_PL0);
	asm("movl $wrapper_fault_sx, %0" :"=r"(isr));
	idt[0x1E] = idt_createentry(isr, 0x08, IDT_INT_PL0);

	int i = 0x20;
	asm("movl $isr_timer_wrapper, %0" :"=r"(isr));
	idt[i++] = idt_createentry(isr, 0x08, IDT_INT_PL0);
	asm("movl $isr_keyboard_wrapper, %0" :"=r"(isr));
	idt[i++] = idt_createentry(isr, 0x08, IDT_INT_PL0);
	asm("movl $isr_cascade_wrapper, %0" :"=r"(isr));
	idt[i++] = idt_createentry(isr, 0x08, IDT_INT_PL0);
	asm("movl $isr_com2_wrapper, %0" :"=r"(isr));
	idt[i++] = idt_createentry(isr, 0x08, IDT_INT_PL0);
	asm("movl $isr_com1_wrapper, %0" :"=r"(isr));
	idt[i++] = idt_createentry(isr, 0x08, IDT_INT_PL0);
	asm("movl $isr_lpt2_wrapper, %0" :"=r"(isr));
	idt[i++] = idt_createentry(isr, 0x08, IDT_INT_PL0);
	asm("movl $isr_floppy_wrapper, %0" :"=r"(isr));
	idt[i++] = idt_createentry(isr, 0x08, IDT_INT_PL0);
	asm("movl $isr_lpt1_wrapper, %0" :"=r"(isr));
	idt[i++] = idt_createentry(isr, 0x08, IDT_INT_PL0);

	i = 0x30;
	asm("movl $isr_clock_wrapper, %0" :"=r"(isr));
	idt[i++] = idt_createentry(isr, 0x08, IDT_INT_PL0);
	asm("movl $isr_free1_wrapper, %0" :"=r"(isr));
	idt[i++] = idt_createentry(isr, 0x08, IDT_INT_PL0);
	asm("movl $isr_free2_wrapper, %0" :"=r"(isr));
	idt[i++] = idt_createentry(isr, 0x08, IDT_INT_PL0);
	asm("movl $isr_free3_wrapper, %0" :"=r"(isr));
	idt[i++] = idt_createentry(isr, 0x08, IDT_INT_PL0);
	asm("movl $isr_mouse_wrapper, %0" :"=r"(isr));
	idt[i++] = idt_createentry(isr, 0x08, IDT_INT_PL0);
	asm("movl $isr_coprocessor_wrapper, %0" :"=r"(isr));
	idt[i++] = idt_createentry(isr, 0x08, IDT_INT_PL0);
	asm("movl $isr_ata1_wrapper, %0" :"=r"(isr));
	idt[i++] = idt_createentry(isr, 0x08, IDT_INT_PL0);
	asm("movl $isr_ata2_wrapper, %0" :"=r"(isr));
	idt[i++] = idt_createentry(isr, 0x08, IDT_INT_PL0);

	idtr.size = sizeof(idt) - 1;
	idtr.offset = (DWORD)&idt;
	_lidt(&idtr);

	return 0;
}

unsigned long long idt_createentry(unsigned offset, unsigned selector, unsigned flag)
{
	flag &= 0xFF;
	QWORD descriptor = 0;

	// Create the high 32 bit segment
	descriptor |= (offset 	& 0xFFFF0000);	// descriptor 63:48 offset 31:16
	descriptor |= flag << 8;				// descriptor 47:40	flag
	descriptor |= 0;						// descriptor 39:32	0

	// Shift by 32 to allow for low part of segment
	descriptor <<= 32;

	// Create the low 32 bit segment
	descriptor |= selector << 16;			// descriptor 31:16 selector 15:0
	descriptor |= (offset & 0x0000FFFF);	// descripror 15:0 offset 15:0

	return descriptor;
}
