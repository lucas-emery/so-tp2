GLOBAL cpuVendor
GLOBAL readPort
GLOBAL writePort
GLOBAL hang
GLOBAL dispatch
GLOBAL testAndSet
GLOBAL updateCR3

%include "./asm/macros.m"



section .text

updateCR3:
	mov rax, cr3
	mov cr3, rax
	ret

dispatch:
	mov rsp, rdi
	cmp rsi,0
	je .L1

	mov QWORD [rsp], rsi
	mov QWORD [rsp + 8], 0x0 ;no se cuales son los flags que corresponden para un proceso nuevo
	pushaq

.L1:
	popaq
	iretq

readPort:	;(Recibe el puerto a leer en rdi)
	push rbp
	mov rbp, rsp

	mov rdx, rdi
	in al, dx

	mov rsp, rbp
	pop rbp
	ret


writePort: ;(Recibe el puerto a escribir en rdi y en rsi lo que hay que escribir)
	push rbp
	mov rbp, rsp

	mov rax, rsi
	mov rdx, rdi
	out dx, al

	mov rsp, rbp
	pop rbp
	ret

cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid

	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

hang:
	cli
	hlt	; halt machine should kernel return
	jmp hang

testAndSet:
	mov rdx, 1
	.spin:
		mov rax, [rdi]
		test rax, rax
		jnz .spin

		lock cmpxchg rdx, rdi

		test rax, rax
		jnz .spin

		ret
