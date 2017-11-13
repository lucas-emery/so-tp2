
GLOBAL getStackPtr
GLOBAL setStackPtr
GLOBAL buildStack
GLOBAL loadTR
GLOBAL loadGDTR
GLOBAL flushPaging

%include "./asm/macros.m"

PD equ 0x10000

loadTR:
  mov rax,rdi
  mov rbx, cs
  ltr ax
  ret

gdtr dw 0 ;Limit
     dq 0 ;Base

loadGDTR:
  mov [gdtr+2], rdi
  mov rax, rsi
  mov [gdtr], ax
  lgdt [gdtr]
  ret

flushPaging:
	mov rax, cr3
	mov cr3, rax
	ret

  ;Save return ptr
getStackPtr:
  lea rax, [rsp+8]
  ret

setStackPtr:
  pop rbx
  mov rsp, rax
  push rbx
  ret

buildStack: ;Params argc, argv, rip, rsp(Process) - Return: rsp(Kernel)
  ;Save return ptr
  pop rbx

  ;For iretq
  push 0x23  ;SS
  push rcx  ;RSP

  push 0x216  ;Rflags with IF in 1

  push 0x1B ;USER_CS
  push rdx  ;RIP

  ;For TTHandler
  push 0x0  ;Fake rax
  pushaq    ;argc and argv are already in the correct registers

  push 0x23 ;DS

  mov rax, rsp

  ;Restore return ptr
  push rbx
  ret
