
.global __start
.extern app_startup
.extern main

.section ".head", "ax"

__start:
  push %ebp
  mov %esp, %ebp

  push %eax

  call app_startup

  call main

  pop %eax

  leave
  ret

