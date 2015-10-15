.section .text
.global _lgdt
.type _lgdt, @function
_lgdt:
	mov 4(%esp), %eax
	lgdt (%eax)
 	mov $0x10, %ax
 	mov %ax, %ds
 	mov %ax, %es
 	mov %ax, %fs
 	mov %ax, %gs
 	mov %ax, %ss
 	ljmp $0x08, $reload_cs
reload_cs:
 	ret
