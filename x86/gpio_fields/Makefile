##########################################################################
#
# Basic, target/architecture independent makefile for building an
# application that runs on its own stack. 
#

TOOL_PREFIX	:= i686-linux

AR		= 	$(TOOL_PREFIX)-ar
LD		= 	$(TOOL_PREFIX)-ld
AS		= 	$(TOOL_PREFIX)-as
CC		= 	$(TOOL_PREFIX)-gcc
OBJCOPY		= 	$(TOOL_PREFIX)-objcopy
OBJDUMP		= 	$(TOOL_PREFIX)-objdump
CPP		= 	$(CC) -E

LOAD_ADDR 	= 0x40000

#########################################################################

CPPFLAGS= -ffunction-sections -fvisibility=hidden -fno-builtin -ffreestanding -nostdinc  -pipe  -fno-strict-aliasing -Wstrict-prototypes -ffreestanding -fno-toplevel-reorder -fno-stack-protector -mpreferred-stack-boundary=2 -fno-dwarf2-cfi-asm -DREALMODE_BASE=0x7c0 -DCONFIG_X86 -D__I386__ -march=i386 -Werror -Wall -Wstrict-prototypes -fno-stack-protector -Wno-format-nonliteral -Wno-format-security -fstack-usage -fno-toplevel-reorder   

OBJCFLAGS += --gap-fill=0xff

gccincdir := $(shell $(CC) -print-file-name=include)

CPPFLAGS := -g -O0 $(CPPFLAGS) -D__KERNEL__ 

CPPFLAGS += -I$(TOPDIR)include -I../../u-boot/include -I../../u-boot/arch/x86/include
CPPFLAGS += -fno-builtin -ffreestanding -nostdinc -isystem $(gccincdir) -pipe $(PLATFORM_CPPFLAGS)

CFLAGS := $(CPPFLAGS) $(TWOFLAGS) -Wall -Wstrict-prototypes


AFLAGS := -g -D__ASSEMBLY__ $(CPPFLAGS)


#########################################################################

TARGET  = main

OBJ = gpio_c.o gpio_asm.o

CRT = crt0.o stubs.o

all:	$(TARGET)	

clean:
		rm -f *.o *.a *.bin *.su
		rm -f $(TARGET)

#########################################################################

$(TARGET): %: %.o $(OBJ) $(CRT)
		$(LD) -g $(EX_LDFLAGS) -T asm.lds -o $@ $^ 
		$(OBJCOPY) -O binary $@ $@.bin 2>/dev/null

#########################################################################

# Inference rules

%.s:	%.S
	$(CPP) $(AFLAGS) -o $@ $<
%.o:	%.S
	$(CC) $(AFLAGS) -c -o $@ $<
%.o:	%.c
	$(CC) $(CFLAGS) -c -o $@ $<

#########################################################################
