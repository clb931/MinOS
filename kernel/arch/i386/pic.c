#include "kernel/common.h"
#include "kernel/vga.h"
#include "kernel/portio.h"
#include "kernel/pic.h"


int pic_init(int msr_offset, int slv_offset)
{
	pic_remap(msr_offset, slv_offset);
	return 0;
}

void pic_remap(int msr_offset, int slv_offset)
{
	BYTE msr_mask = inb(MSR_PIC_DAT);
	BYTE slv_mask = inb(SLV_PIC_DAT);

	outb(MSR_PIC_CMD, ICW1_INIT + ICW1_ICW4);
	io_wait();
	outb(SLV_PIC_CMD, ICW1_INIT + ICW1_ICW4);
	io_wait();
	outb(MSR_PIC_DAT, msr_offset);
	io_wait();
	outb(SLV_PIC_DAT, slv_offset);
	io_wait();
	outb(MSR_PIC_DAT, 4);
	io_wait();
	outb(SLV_PIC_DAT, 2);
	io_wait();

	outb(MSR_PIC_DAT, ICW4_8086);
	io_wait();
	outb(SLV_PIC_DAT, ICW4_8086);
	io_wait();

	outb(MSR_PIC_DAT, msr_mask);
	outb(SLV_PIC_DAT, slv_mask);
}

void pic_sendEOI(int irq)
{
	irq &= 0xFF;
	if(irq >= 8)
		outb(SLV_PIC_CMD, PIC_EOI);
 
	outb(MSR_PIC_CMD, PIC_EOI);
}

void irq_set_mask(BYTE line) {
    WORD port;
    BYTE value;
 
    if(line < 8) {
        port = MSR_PIC_DAT;
    } else {
        port = SLV_PIC_DAT;
        line -= 8;
    }
    value = inb(port) | (1 << line);
    outb(port, value);        
}
 
void irq_clear_mask(BYTE line) {
    WORD port;
    BYTE value;
 
    if(line < 8) {
        port = MSR_PIC_DAT;
    } else {
        port = SLV_PIC_DAT;
        line -= 8;
    }
    value = inb(port) & ~(1 << line);
    outb(port, value);        
}
