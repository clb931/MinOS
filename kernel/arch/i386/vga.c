#include "kernel/common.h"
#include "kernel/vga.h"
#include "kernel/portio.h"
#include <string.h>


typedef BYTE	VGA_Color;
typedef WORD	VGA_Entry;

static VGA_Entry* const	VGA_MEMORY = (VGA_Entry *)0xB8000;

static unsigned		 	vga_row;
static unsigned		 	vga_col;
static VGA_Color	 	vga_clr;


static inline VGA_Color make_vgacolor(int fg, int bg)
{
	return (VGA_Color)(fg | bg << 4);
}

static inline VGA_Entry make_vgaentry(char c, VGA_Color color)
{
	return (VGA_Entry)(c | color << 8);
}

static void scroll_down()
{
	for (unsigned y = 2; y < VGA_HEIGHT - 1; ++y)
		for (unsigned x = 0; x < VGA_WIDTH; ++x)
			VGA_MEMORY[(y-1) * VGA_WIDTH + x] = VGA_MEMORY[y * VGA_WIDTH + x];

	for (unsigned x = 0; x < VGA_WIDTH; ++x)
		VGA_MEMORY[(VGA_HEIGHT - 2) * VGA_WIDTH + x] = make_vgaentry(' ', vga_clr);
	
	vga_row = VGA_HEIGHT - 2;
}

static void update_cursor()
{
	WORD position = (vga_row * VGA_WIDTH) + vga_col;
	outb(0x3D4, 0x0F);
	outb(0x3D5, (BYTE)(position & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (BYTE)((position >> 8) & 0xFF));
}

void vga_init()
{
	vga_clr = make_vgacolor(COLOR_LIGHT_GRAY, COLOR_BLACK);
	vga_clear();
}

void vga_clear()
{
	vga_row = 1;
	vga_col = 0;
	
	update_cursor();
	for (unsigned y = vga_row; y < VGA_HEIGHT - 1; ++y)
		vga_clearline(y);
}

void vga_clearline(int y)
{
	VGA_Entry entry = make_vgaentry(' ', vga_clr);
	for (unsigned x = 0; x < VGA_WIDTH; ++x)
		VGA_MEMORY[y * VGA_WIDTH + x] = entry;
}

int vga_getcolor()
{
	return vga_clr;
}

int vga_getcolor_alt(int *fg, int *bg)
{
	*fg = vga_clr & 0xFF;
	*bg = (vga_clr >> 4) & 0xFF;
	
	return vga_clr;
}

int vga_setcolor(int fg, int bg)
{
	int old = (int)vga_clr;
	vga_clr = make_vgacolor(fg, bg);
	return old;
}

int vga_setcolor_alt(int color)
{
	int old = (int)vga_clr;
	vga_clr = color;
	return old;
}

void vga_putcharat(char c, int x, int y)
{
	VGA_MEMORY[y * VGA_WIDTH + x] = make_vgaentry(c, vga_clr);
}

void vga_putchar(char c)
{
	switch (c) {
		case '\n': {
			for (; vga_col < VGA_WIDTH; ++vga_col)
				vga_putcharat(' ', vga_col, vga_row);

			vga_col = 0;
			if (++vga_row == VGA_HEIGHT - 1)
				scroll_down();
		} break;

		case '\r': {
			if ((VGA_MEMORY[vga_row * VGA_WIDTH + vga_col] & 0xFF) == '\n')
				return;

			for (; vga_col < VGA_WIDTH; ++vga_col)
				vga_putcharat(' ', vga_col, vga_row);

			vga_col = 0;
			if (++vga_row == VGA_HEIGHT - 1)
				scroll_down();
		} break;

		case '\t': {
			for (int i = vga_col%4; i < 4; ++i) {
				vga_putcharat(' ', vga_col, vga_row);
				if (++vga_col == VGA_WIDTH) {
					vga_col = 0;

					if (++vga_row == VGA_HEIGHT - 1)
						scroll_down();

					update_cursor();
					return;
				}
			}
		} break;

		default: {
			vga_putcharat(c, vga_col, vga_row);
			if (++vga_col == VGA_WIDTH) {
				vga_col = 0;
				if (++vga_row == VGA_HEIGHT - 1) {
					scroll_down();
				}
			}
		} break;
	}
	update_cursor();
}

void vga_putnstr(const char *str, unsigned len)
{
	for (unsigned i = 0; i < len; ++i)
		vga_putchar(str[i]);
}

void vga_putstr(const char *str)
{
	for (unsigned i = 0; str[i] != '\0'; ++i)
		vga_putchar(str[i]);
}

VGA_Color title_color;
void vga_settitletext(const char *str, int fg, int bg)
{
	VGA_Color old_color = vga_setcolor(fg, bg);
	vga_putcharat(' ', 0, 0);

	DWORD j = 0;
	for (DWORD i = 1; i < VGA_WIDTH; ++i) {
		if (str[j] != '\0') {
			if (str[j] == '\n' || str[j] == '\r' || str[j] == '\t')
				vga_putcharat(' ', i, 0);
			else
				vga_putcharat(str[j], i, 0);

			j++;
		} else {
			vga_putcharat(' ', i, 0);
		}
	}

	title_color = vga_setcolor_alt(old_color);
}

void vga_setstatustext(const char *str, int fg, int bg)
{
	DWORD y = VGA_HEIGHT - 1;
	VGA_Color old_color = vga_setcolor(fg, bg);
	vga_putcharat(' ', 0, y);

	DWORD j = 0;
	for (DWORD i = 1; i < VGA_WIDTH; ++i) {
		if (str[j] != '\0') {
			if (str[j] == '\n' || str[j] == '\r' || str[j] == '\t')
				vga_putcharat(' ', i, y);
			else
				vga_putcharat(str[j], i, y);

			j++;
		} else {
			vga_putcharat(' ', i, y);
		}
	}

	vga_setcolor_alt(old_color);
}

void vga_settimer(const char *str)
{
	VGA_Color old_color = vga_setcolor_alt(title_color);
	DWORD x = strlen(str);
	DWORD j = 0;

	while (x > 0) {
		vga_putcharat(str[j++], VGA_WIDTH - x - 1, 0);
		x--;
	}

	vga_setcolor_alt(old_color);
}
