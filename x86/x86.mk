##########################################################################

#############
# Toolchain #
#############

TOOL_PREFIX	:= i686-linux

AR      = $(TOOL_PREFIX)-ar
LD      = $(TOOL_PREFIX)-ld
AS      = $(TOOL_PREFIX)-as
CC      = $(TOOL_PREFIX)-gcc
OBJCOPY = $(TOOL_PREFIX)-objcopy
OBJDUMP = $(TOOL_PREFIX)-objdump
CPP     = $(CC) -E

#########################################################################

#################
# General flags #
#################
# (preprocessor flags)

# GCC include directory (of specific toolchain)
gccincdir := $(shell $(CC) -print-file-name=include)

# Debug and Disable optimisation
CPPFLAGS += -g -O0
# 32-bit Architecture
CPPFLAGS += -march=i386 -mpreferred-stack-boundary=2
# Warn on everything
CPPFLAGS += -Wall
# Include directories (do not use default system directories for header files)
CPPFLAGS += -nostdinc -I../../u-boot/include -I../../u-boot/arch/x86/include -isystem $(gccincdir)
# For calls to u-boot functions
CPPFLAGS += -D__KERNEL__ -DREALMODE_BASE=0x7c0 -DCONFIG_X86 -D__I386__
# Emit DWARF unwind info as compiler generated .eh_frame section instead of using GAS .cfi_* directives.
CPPFLAGS += -fno-dwarf2-cfi-asm
# A unit compiled with -fstack-usage will generate an extra file that specifies
# the maximum amount of stack used, on a per-function basis. The file has the
# same basename as the target object file with a .su extension. Each line of
# this file is made up of three fields:
# * The name of the function.
# * A number of bytes.
# * One or more qualifiers: static, dynamic, bounded. The second field
#   corresponds to the size of the known part of the function frame. The
#   qualifier static means that the function frame size is purely static. It
#   usually means that all local variables have a static size. In this case, the
#   second field is a reliable measure of the function stack utilization. The
#   qualifier dynamic means that the function frame size is not static. It
#   happens mainly when some local variables have a dynamic size. When this
#   qualifier appears alone, the second field is not a reliable measure of the
#   function stack analysis. When it is qualified with bounded, it means that
#   the second field is a reliable maximum of the function stack utilization. A
#   unit compiled with -Wstack-usage will issue a warning for each subprogram
#   whose stack usage might be larger than the specified amount of bytes. The
#   wording is in keeping with the qualifier documented above.
CPPFLAGS += -fstack-usage
# Don't append extra code for stack protection
CPPFLAGS += -fno-stack-protector
# Do not reorder top-level functions, variables, and asm statements
CPPFLAGS += -fno-toplevel-reorder
# https://interrupt.memfault.com/blog/best-and-worst-gcc-clang-compiler-flags#-ffunction-sections--fdata-sections----gc-sections
# Give each function its own section
CPPFLAGS += -ffunction-sections
# A freestanding environment is one in which the standard library may not exist,
# and program startup may not necessarily be at "main". The option -ffreestanding
# directs the compiler to not assume that standard functions have their usual definition.
CPPFLAGS += -ffreestanding

##########
# Others #
##########

# Remove warnings ...
# -Wno-format-security -Wno-format-nonliteral

# C++ options
# CXXFLAGS += -fvisibility=hidden -fno-strict-aliasing

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
