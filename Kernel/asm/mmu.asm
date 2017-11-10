GLOBAL buildStack
GLOBAL loadTR

%include "./asm/macros.m"

loadTR:
  ltr rdi
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
