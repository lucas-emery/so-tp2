GLOBAL sti
GLOBAL cli
GLOBAL irq0Handler
GLOBAL irq1Handler
GLOBAL irq12Handler
GLOBAL PFHandler
GLOBAL TTHandler
GLOBAL setPicMaster
GLOBAL setPicSlave
GLOBAL int80Handler
GLOBAL int80
GLOBAL intTT

%include "./asm/macros.m"

EXTERN irqDispatcher
EXTERN sysCallHandler
EXTERN sendEOI
EXTERN pageFaultHandler
EXTERN runModule
EXTERN timerTickHandler
EXTERN kernelMode
EXTERN userMode

section .text

intTT:
	int 0x20
	ret

TTHandler:
	push rax
	pushaq
	xor rax, rax
	mov rax, ds
	push rax



	mov rdi, rsp
	call timerTickHandler

	mov rdi, 0x20
	call sendEOI

	pop rax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	popaq
	pop rax
	iretq

irq0Handler:
	irqHandler 0

irq1Handler:
	irqHandler 1

irq12Handler:
	irqHandler 12

PFHandler:
	; mov QWORD[rsp], pageFaultHandler
	call pageFaultHandler
	iretq

int80Handler:
	pushaq

	call kernelMode
	call sysCallHandler
	
	push rax
	call userMode
	pop rax

	popaq
	iretq

sti:
	sti
	ret

cli:
	cli
	ret

setPicMaster:
	setPicMask 0x21

setPicSlave:
	setPicMask 0xA1
