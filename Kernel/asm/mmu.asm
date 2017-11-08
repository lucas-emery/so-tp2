
GLOBAL getStackPtr
GLOBAL setStackPtr
GLOBAL buildStack
GLOBAL swapStack
GLOBAL cleanReturnAddress
GLOBAL injectReturnAddress

%include "./asm/macros.m"

PD equ 0x10000

getStackPtr:
  lea rax, [rsp+8]
  ret

setStackPtr:
  pop rbx
  mov rsp, rax
  push rbx
  ret

swapStack: ;Params: newRsp, stackIndex, stackAddress - Returns: oldRsp
  pop rbx       ;Save ret addr
  mov rax, rsp  ;Save oldRsp

  xor rcx, rcx  ;Clean rcx
  mov rcx, PD   ;Load PD address
.loop:
  add rcx, 8    ;Move on the PD
  dec rsi       ;Rsi has the stack page PDE index
  cmp rsi, 0
  jne .loop

  or rdx, 0x8F    ;Prepare new PDE
  mov [rcx], rdx  ;Set new PDE

  mov rsp, rdi  ;Set newRsp

  push rbx      ;Restore ret address
  ret           ;Returns oldRsp

cleanReturnAddress: ;Cleans and returns the previous function's return address
  pop rbx
  pop rax
  push rbx
  ret

injectReturnAddress: ;Params: newRetAddress
  pop rbx
  push rdi
  push rbx
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
