TOOL_PREFIX	=

CC		= gcc

TARGET	= exo1 exo2 exo3 exo4-debug exo4-optimized

LIBLEDS = test_leds.a
LIBT1   = libtests_exo1.a
LIBT2   = libtests_exo2.a
LIBT3   = libtests_exo3.a

all: $(TARGET)

clean:
	rm -f *.o
	rm -f $(TARGET)

%.o: %.c
	$(CC) -g -I include -c $<

exo4-optimized.o: exo4.c
	$(CC) -O3 -g -I include -c $< -o $@

exo4-debug.o: exo4.c
	$(CC) -O0 -g -I include -c $< -o $@

# Link exo1 with its test lib
exo1: exo1.o $(LIBLEDS) $(LIBT1)
	$(CC) $< $(LIBLEDS) $(LIBT1) -o $@

# Link exo2 with its test lib
exo2: exo2.o $(LIBLEDS) $(LIBT2)
	$(CC) $< $(LIBLEDS) $(LIBT2) -o $@

# Link exo3 with its test lib
exo3: exo3.o $(LIBLEDS) $(LIBT3)
	$(CC) $< $(LIBLEDS) $(LIBT3) -o $@

# exo4: no tests
exo4-debug: exo4-debug.o $(LIBLEDS)
	$(CC) $< $(LIBLEDS) -o $@

exo4-optimized: exo4-optimized.o $(LIBLEDS)
	$(CC) $< $(LIBLEDS) -o $@

.PHONY: all clean
