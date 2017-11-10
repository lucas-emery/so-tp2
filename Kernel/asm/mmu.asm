GLOBAL buildStack
GLOBAL loadTR
GLOBAL loadGDTR

%include "./asm/macros.m"

loadTR:
  mov rax,rdi
  mov rbx, cs
  ltr ax
  ret

gdtr dw 100 ;Limit
     dq 0x1000 ;Base

loadGDTR:
  cli
  mov rax, rdi
  ;mov [gdtr+2], eax
  mov rax, rsi
  ;mov [gdtr], ax
  lgdt [gdtr]
  ret
  ;jmp dword 0x08:flush

flush:
  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  ret

buildStack: ;argc, argv, rip
  ;Save return ptr
  pop rax
  mov rbx, rsp

  ;For iretq
  xor r15, r15
  mov r15, ss
  push r15
  push rbx  ;RSP

  pushfq
  pop rbx
  or rbx, 0x9
  push rbx  ;Rflags with IF in 1

  xor r15, r15
  mov r15, cs
  push r15
  push rdx ;RIP

  ;For TTHandler
  pushaq

  ;Restore return ptr
  push rax
  ret
