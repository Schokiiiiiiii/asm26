-include ../arm.mk

#########################################################################

########################
# Application specific #
########################

TARGETS = backtrace
OBJS = backtrace_asm.o

CRT = crt0.o stubs.o

#########################################################################

all : $(TARGETS)

$(TARGETS) : % : %.o $(OBJS) $(CRT)
	$(LD) -g -T asm.lds -o $@ $^
	$(OBJCOPY) -O binary $@ $@.bin 2>/dev/null
	cp backtrace.bin ../../bt

#########################################################################

clean :
	rm -f *.o *.a *.bin *.su
	rm -f $(TARGETS)
	rm -f ../../bt

.PHONY : clean