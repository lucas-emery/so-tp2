
GLOBAL getStackPtr
GLOBAL setStackPtr
GLOBAL buildStack
GLOBAL swapStack
GLOBAL cleanReturnAddress
GLOBAL injectReturnAddress
GLOBAL loadTR
GLOBAL loadGDTR

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

  ;Save return ptr
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

buildStack: ;Params argc, argv, rip, rsp(Process) - Return: rsp(Kernel)
  ;Save return ptr
  pop rbx

  ;For iretq
  ;xor r15, r15
  ;mov r15, ss
  push 0x0  ;SS
  push rcx  ;RSP

  pushfq
  pop rax
  or rax, 0x9
  push 0x216  ;Rflags with IF in 1

  ;xor r15, r15
  ;mov r15, cs
  push 0x18 ;USER_CS
  push rdx  ;RIP

  ;For TTHandler
  push 0x0  ;Fake rax
  pushaq    ;argc and argv are already in the correct registers

  mov rax, rsp

  ;Restore return ptr
  push rbx
  ret
