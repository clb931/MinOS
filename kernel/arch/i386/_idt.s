.section .text
.global _lidt
.type _lidt, @function
_lidt:
	mov 4(%esp), %eax
	lidt (%eax)
	ret
