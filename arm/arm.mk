##########################################################################

#############
# Toolchain #
#############

TOOL_PREFIX	:= arm-linux-gnueabihf

AR      = $(TOOL_PREFIX)-ar
LD      = $(TOOL_PREFIX)-ld
AS      = $(TOOL_PREFIX)-as
CC      = $(TOOL_PREFIX)-gcc
OBJCOPY	= $(TOOL_PREFIX)-objcopy
OBJDUMP = $(TOOL_PREFIX)-objdump
CPP	    = $(CC) -E

#########################################################################

#################
# General flags #
#################
# (preprocessor flags)

# GCC include directory (of specific toolchain)
gccincdir := $(shell $(CC) -print-file-name=include)

# Debug and Disable optimisation
CPPFLAGS += -g -O0
# ARM Architecture ARMv7-A and ABI
CPPFLAGS += -marm -march=armv7-a -msoft-float -mno-thumb-interwork -mabi=aapcs-linux -ffixed-r8
# Warn on everything
CPPFLAGS += -Wall
# Include directories (do not use default system directories for header files)
CPPFLAGS += -nostdinc -I../../u-boot/include -I../../u-boot/arch/arm/include -isystem $(gccincdir)
# For calls to u-boot functions
CPPFLAGS += -D__KERNEL__ -D__ARM__ -DCONFIG_ARM
# A freestanding environment is one in which the standard library may not exist,
# and program startup may not necessarily be at "main". The option -ffreestanding
# directs the compiler to not assume that standard functions have their usual definition.
CPPFLAGS += -ffreestanding

####################
# C specific flags #
####################

# Warn if a function is declared or defined without specifying the argument types
# Don't use GCC built-in functions
# (C only)
CFLAGS += -Wstrict-prototypes -fno-builtin

############################
# Assembler specific flags #
############################

ASFLAGS += $(CPPFLAGS)
# For calls to u-boot functions
ASFLAGS += -D__ASSEMBLY__

#################
# Objcopy flags #
#################

# Fill gaps between sections with 0xff
OBJCFLAGS += --gap-fill=0xff

#########################################################################