
.global __start
.extern app_startup
.extern main

.section ".head", "ax"

__start:
  stmfd sp!, {r0-r10, lr}

  bl app_startup

  bl main

  ldmfd sp!, {r0-r10, pc}


