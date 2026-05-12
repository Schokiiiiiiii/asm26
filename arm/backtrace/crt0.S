.global __start
.extern app_startup
.extern main

.section ".head", "ax"

__start:
  stmfd sp!, {r0-r11, lr}

  bl app_startup

  @ Artificially put -1, -1 on the stack
  eor r0, r0
  sub r0, #1
  mov r1, r0
  stmfd sp!, {r0, r1} @ stmfd sp!, {fp, lr}
  @ Adjust fp so that it is pointing to the right place when saved by main
  add fp, sp, #4

  bl main

  @ Restore the stack by pop'ing
  ldmfd sp!, {r0, r1}

  ldmfd sp!, {r0-r11, pc}