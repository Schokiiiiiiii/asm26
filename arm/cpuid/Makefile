-include ../arm.mk

#########################################################################

########################
# Application specific #
########################

# Used in target application
CFLAGS += -D__QEMU_BARE__=1

TARGET = main

CRT = crt0.o stubs.o

all : $(TARGET)

#########################################################################

$(TARGET) : % : %.o $(OBJ) $(CRT)
	$(LD) -g -T asm.lds -o $@ $^
	$(OBJCOPY) -O binary $@ $@.bin 2>/dev/null

#########################################################################

clean :
	rm -f *.o *.a *.bin *.su
	rm -f $(TARGET)

.PHONY : clean