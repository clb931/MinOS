.section .init
_init:
	# gcc puts crtend.o's .init section here...
	popl %ebp
	ret
.size _init, . - _init

.section .fini
_fini:
	# gcc puts crtend.o's .fini section here...
	popl %ebp
	ret
.size _fini, . - _fini
