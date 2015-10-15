NAME=MinOS
PREFIX?=../bin
TARGET?=i686-elf
ARCH?=i386

INCLUDE_DIR?=../../Common/include
LIBRARY_DIR?=../../Common/lib

.PHONY: all libc test iso run clean

all:
	mkdir -p $(PREFIX)/usr/include
	cp -RTv $(INCLUDE_DIR) $(PREFIX)/usr/include
	cp -RTv $(LIBRARY_DIR)/*/include $(PREFIX)/usr/include
	# cp -v kernel/*.h $(PREFIX)/usr/include/kernel/
	mkdir -p $(PREFIX)/usr/lib
	cp $(LIBRARY_DIR)/lib*.a $(PREFIX)/usr/lib/
	make -C ./kernel NAME=$(NAME) PREFIX=../$(PREFIX) TARGET=$(TARGET) ARCH=$(ARCH)

libc:
	make -C $(LIBRARY_DIR)/libc TARGET=$(TARGET) ARCH=$(ARCH)
	cp -v $(LIBRARY_DIR)/libc/*.a $(LIBRARY_DIR)/

test:
	qemu-system-$(ARCH) -kernel $(PREFIX)/iso/boot/$(NAME).kernel

iso:
	mkdir -p $(PREFIX)/iso/boot/grub
	echo "menuentry \"$(NAME)\" {\n\tmultiboot /boot/$(NAME).kernel\n}" > $(PREFIX)/iso/boot/grub/grub.cfg
	grub-mkrescue -o $(NAME).iso $(PREFIX)/iso

run:
	qemu-system-$(ARCH) $(NAME).iso

clean:
	make clean -C ./kernel NAME=$(NAME) PREFIX=../$(PREFIX) TARGET=$(TARGET) ARCH=$(ARCH)
	rm -r $(PREFIX)
