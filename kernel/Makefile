NAME?=MyOS
PREFIX?=./
TARGET?=i686-elf
ARCH?=i386

CC=$(TARGET)-gcc --sysroot=$(PREFIX) -isystem=/usr/include
LD_FILE=arch/$(ARCH)/linker.ld

CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=

CFLAGS:=$(CFLAGS) -ffreestanding -fbuiltin -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -D__$(NAME)_LIBC__
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lk -lgcc

include arch/$(ARCH)/make.config
CFLAGS:=$(CFLAGS) $(ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(ARCH_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(ARCH_LDFLAGS)
LIBS:=$(LIBS) $(ARCH_LIBS)

OBJS=\
$(ARCH_OBJS) \
kernel.o

INTERNAL_OBJS=\
arch/$(ARCH)/crti.o \
$(OBJS) \
arch/$(ARCH)/crtn.o

OBJ_LINK_LIST=\
arch/$(ARCH)/crti.o \
$(shell $(CC) -print-file-name=crtbegin.o) \
$(OBJS) \
$(shell $(CC) -print-file-name=crtend.o) \
arch/$(ARCH)/crtn.o

.PHONY: all clean

all: $(OBJ_LINK_LIST) $(LD_FILE)
	mkdir -p $(PREFIX)/iso/boot
	$(CC) -T $(LD_FILE) -o $(PREFIX)/iso/boot/$(NAME).kernel $(CFLAGS) $(OBJ_LINK_LIST) $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

%.o: %.s
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

clean:
	rm -rf $(PREFIX)/iso/boot $(INTERNAL_OBJS)
