.globl _start
_start:
    mov sp, #0x7000  @ Set SVC stack pointer
    bl _cstart
hang: b reboot

/* 
 * Enable/disable interrupts.  
 *
 * CPSR = current program status register
 *        upper bits are differen carry flags.
 *        lower 8:
 *   		7 6 5 4 3 2 1 0
 *  		+-+-+-+---------+
 *  		|I|F|T|   Mode  |
 *  		+-+-+-+---------+
 *
 *	I : disables IRQ when = 1.
 *	F : disables FIQ when = 1.
 * 	T : = 0 indicates ARM execution, = 1 is thumb execution.
 *      Mode = current mode.
 */
.globl system_enable_interrupts
system_enable_interrupts:
    mrs r0,cpsr     // mrs: move coprocessor register to CPU register
    bic r0,r0,#0x80 // bic: bit clear
    msr cpsr_c,r0   // msr: CPU register to coprocessor register
    bx lr

.globl system_disable_interrupts
system_disable_interrupts:
    mrs r0,cpsr
    orr r0,r0,#0x80
    msr cpsr_c,r0
    bx lr

/*
 * Interrupt vectors.
 */
.globl _vectors
.globl _vectors_end
        
_vectors:
  ldr pc, _reset_asm
  ldr pc, _undefined_instruction_asm
  ldr pc, _software_interrupt_asm
  ldr pc, _prefetch_abort_asm
  ldr pc, _data_abort_asm
  ldr pc, _reset_asm
  ldr pc, _interrupt_asm
fast_interrupt_asm:
  ldr pc, _fast_asm

_reset_asm:                   .word impossible_asm
_undefined_instruction_asm:   .word impossible_asm
_software_interrupt_asm:      .word impossible_asm
_prefetch_abort_asm:          .word impossible_asm
_data_abort_asm:              .word impossible_asm
_interrupt_asm:               .word interrupt_asm
_fast_asm:                    .word impossible_asm

_vectors_end:

interrupt_asm:
  mov sp, #0x8000
  sub lr, lr, #4
  push  {r0-r12, lr}
  mov  r0, lr
  bl  interrupt_vector
  pop  {r0-r12, lr}
  // jump back
  subs    pc, lr, #0

impossible_asm:
  mov sp, #0x7000  @ Set SVC stack pointer
  sub   lr, lr, #4
  bl    impossible_vector    @ C function
  b     reboot

/*
 * Helpers to get cpsr/spsr
 */
.globl set_cpsr
set_cpsr:
	msr cpsr, r0
	bx lr

.globl get_cpsr
get_cpsr:
	mrs r0, cpsr
	bx lr

.globl get_spsr
get_spsr:
	mrs r0, spsr
	bx lr
