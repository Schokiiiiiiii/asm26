##########################################################################
#
# Basic, target/architecture independent makefile for building an
# application that runs on its own stack. 
#

TOOL_PREFIX	:= arm-linux-gnueabihf

AR		= 	$(TOOL_PREFIX)-ar
LD		= 	$(TOOL_PREFIX)-ld
AS		= 	$(TOOL_PREFIX)-as
CC		= 	$(TOOL_PREFIX)-gcc
OBJCOPY		= 	$(TOOL_PREFIX)-objcopy
OBJDUMP		= 	$(TOOL_PREFIX)-objdump
CPP		= 	$(CC) -E

LOAD_ADDR 	= 0x82000000

#########################################################################

CPPFLAGS += -fno-common -ffixed-r8 -msoft-float -Wall -Wstrict-prototypes -DCONFIG_ARM -D__ARM__ -marm  -mabi=aapcs-linux -mno-thumb-interwork -march=armv4

OBJCFLAGS += --gap-fill=0xff

gccincdir := $(shell $(CC) -print-file-name=include)

CPPFLAGS := -g -O0 $(CPPFLAGS) -D__KERNEL__ 

CPPFLAGS += -I$(TOPDIR)include -I../../u-boot/include -I../../u-boot/arch/arm/include
CPPFLAGS += -fno-builtin -ffreestanding -nostdinc -isystem $(gccincdir) -pipe $(PLATFORM_CPPFLAGS)

CFLAGS := $(CPPFLAGS) -Wall -Wstrict-prototypes

AFLAGS := -g -D__ASSEMBLY__ $(CPPFLAGS)


#########################################################################

TARGET  = main
OBJ = copy_string_c.o string_asm.o

CRT = crt0.o stubs.o

all: $(TARGET)

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
