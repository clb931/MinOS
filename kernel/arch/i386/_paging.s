.section .text
.global _pg_ld_dir
.type _pg_ld_dir, @function
_pg_ld_dir:
	mov 4(%esp), %eax
	mov %eax, %cr3
	ret

.global _pg_enable
.type _pg_enable, @function
_pg_enable:
	mov %cr0, %eax
	or $0x80000000, %eax
	mov %eax, %cr0
	ret

.global _invlpg
.type _invlpg, @function
_invlpg:
	mov 4(%esp), %eax
	invlpg (%eax)
	ret
