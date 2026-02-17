#!/bin/bash
qemu/arm-softmmu/qemu-system-arm $1 -s \
	-M vexpress-a15 -cpu cortex-a15\
	-serial mon:stdio  \
	-kernel u-boot/u-boot \
	-m 1024 \
        -net user,id=n0,tftp=. \
	-net nic,macaddr=00:01:29:53:97:BA \
	-nographic 
        
  
