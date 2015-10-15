.section .text
.global wrapper_int_unhandled
.align 4
wrapper_int_unhandled:
	pushal
	cld
	call isr_int_unhandled
	popal
	iret
	
.global wrapper_fault_de
.align 4
wrapper_fault_de:
	pushal
	cld
	call isr_fault_de
	popal
	iret
	
.global wrapper_trap_db
.align 4
wrapper_trap_db:
	pushal
	cld
	call isr_trap_db
	popal
	iret
	
.global wrapper_int_nonmaskable
.align 4
wrapper_int_nonmaskable:
	pushal
	cld
	call isr_int_nonmaskable
	popal
	iret
	
.global wrapper_trap_bp
.align 4
wrapper_trap_bp:
	pushal
	cld
	call isr_trap_bp
	popal
	iret
	
.global wrapper_trap_of
.align 4
wrapper_trap_of:
	pushal
	cld
	call isr_trap_of
	popal
	iret
	
.global wrapper_fault_br
.align 4
wrapper_fault_br:
	pushal
	cld
	call isr_fault_br
	popal
	iret
	
.global wrapper_fault_ud
.align 4
wrapper_fault_ud:
	pushal
	cld
	call isr_fault_ud
	popal
	iret
	
.global wrapper_fault_nm
.align 4
wrapper_fault_nm:
	pushal
	cld
	call isr_fault_nm
	popal
	iret
	
.global wrapper_abort_df
.align 4
wrapper_abort_df:
	pushal
	cld
	call isr_abort_df
	popal
	iret
	
.global wrapper_fault_cso
.align 4
wrapper_fault_cso:
	pushal
	cld
	call isr_fault_cso
	popal
	iret
	
.global wrapper_fault_ts
.align 4
wrapper_fault_ts:
	pushal
	cld
	call isr_fault_ts
	popal
	iret
	
.global wrapper_fault_np
.align 4
wrapper_fault_np:
	pushal
	cld
	call isr_fault_np
	popal
	iret
	
.global wrapper_fault_ss
.align 4
wrapper_fault_ss:
	pushal
	cld
	call isr_fault_ss
	popal
	iret
	
.global wrapper_fault_gp
.align 4
wrapper_fault_gp:
	pushal
	cld
	call isr_fault_gp
	popal
	iret
	
.global wrapper_fault_pf
.align 4
wrapper_fault_pf:
	pushal
	cld
	call isr_fault_pf
	popal
	iret
	
.global wrapper_fault_mf
.align 4
wrapper_fault_mf:
	pushal
	cld
	call isr_fault_mf
	popal
	iret
	
.global wrapper_fault_ac
.align 4
wrapper_fault_ac:
	pushal
	cld
	call isr_fault_ac
	popal
	iret
	
.global wrapper_abort_mc
.align 4
wrapper_abort_mc:
	pushal
	cld
	call isr_abort_mc
	popal
	iret
	
.global wrapper_fault_xf
.align 4
wrapper_fault_xf:
	pushal
	cld
	call isr_fault_xf
	popal
	iret
	
.global wrapper_fault_ve
.align 4
wrapper_fault_ve:
	pushal
	cld
	call isr_fault_ve
	popal
	iret
	
.global wrapper_fault_sx
.align 4
wrapper_fault_sx:
	pushal
	cld
	call isr_fault_sx
	popal
	iret
	
.global isr_timer_wrapper
.align 4
isr_timer_wrapper:
	pushal
	cld
	call isr_timer
	popal
	iret
	
.global isr_keyboard_wrapper
.align 4
isr_keyboard_wrapper:
	pushal
	cld
	call isr_keyboard
	popal
	iret
	
.global isr_cascade_wrapper
.align 4
isr_cascade_wrapper:
	pushal
	cld
	call isr_cascade
	popal
	iret
	
.global isr_com2_wrapper
.align 4
isr_com2_wrapper:
	pushal
	cld
	call isr_com2
	popal
	iret
	
.global isr_com1_wrapper
.align 4
isr_com1_wrapper:
	pushal
	cld
	call isr_com1
	popal
	iret
	
.global isr_lpt2_wrapper
.align 4
isr_lpt2_wrapper:
	pushal
	cld
	call isr_lpt2
	popal
	iret
	
.global isr_floppy_wrapper
.align 4
isr_floppy_wrapper:
	pushal
	cld
	call isr_floppy
	popal
	iret
	
.global isr_lpt1_wrapper
.align 4
isr_lpt1_wrapper:
	pushal
	cld
	call isr_lpt1
	popal
	iret
	
.global isr_clock_wrapper
.align 4
isr_clock_wrapper:
	pushal
	cld
	call isr_clock
	popal
	iret
	
.global isr_free1_wrapper
.align 4
isr_free1_wrapper:
	pushal
	cld
	call isr_free1
	popal
	iret
	
.global isr_free2_wrapper
.align 4
isr_free2_wrapper:
	pushal
	cld
	call isr_free2
	popal
	iret
	
.global isr_free3_wrapper
.align 4
isr_free3_wrapper:
	pushal
	cld
	call isr_free3
	popal
	iret
	
.global isr_mouse_wrapper
.align 4
isr_mouse_wrapper:
	pushal
	cld
	call isr_mouse
	popal
	iret
	
.global isr_coprocessor_wrapper
.align 4
isr_coprocessor_wrapper:
	pushal
	cld
	call isr_coprocessor
	popal
	iret
	
.global isr_ata1_wrapper
.align 4
isr_ata1_wrapper:
	pushal
	cld
	call isr_ata1
	popal
	iret
	
.global isr_ata2_wrapper
.align 4
isr_ata2_wrapper:
	pushal
	cld
	call isr_ata2
	popal
	iret
