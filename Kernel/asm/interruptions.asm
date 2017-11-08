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

%include "./asm/macros.m"

EXTERN irqDispatcher
EXTERN sysCallHandler
EXTERN sendEOI
EXTERN pageFaultHandler
EXTERN runModule
EXTERN timerTickHandler

section .text

TTHandler:
	push rax
	pushaq

	call timerTickHandler

	mov rdi, 0x20
	call sendEOI

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

	call sysCallHandler

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
