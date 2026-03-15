CFLAGS=-Wall -Iinclude -marm # compilator flags
ARFLAGS=rcs # archiver flags
LDFLAGS=-T $(SCRIPT_LD) # linker flags
CC=arm-linux-gnueabihf-gcc # compilateur
AR=arm-linux-gnueabihf-ar # archiver
LD=arm-linux-gnueabihf-ld # linker
CV=arm-linux-gnueabihf-objcopy # conversion elf -> binaire brut

APP = raspdrone
LIB = libdrone.a
BIN = raspdrone.bin

BLD_DIR = .build/

APP_PATH = $(BLD_DIR)$(APP)
LIB_PATH = $(BLD_DIR)$(LIB)
BIN_PATH = $(BLD_DIR)$(BIN)

LIB_SRC = src/motor.c src/gyroscope.c src/camera.c
LIB_OBJ = $(LIB_SRC:src/%.c=$(BLD_DIR)%.o)
APP_SRC = src/engine.c src/autopilot.c src/navigation.c src/pwm_drone.c
APP_OBJ = $(APP_SRC:src/%.c=$(BLD_DIR)%.o)
ASM_SRC	= asm/crt0.S
ASM_OBJ = $(ASM_SRC:asm/%.S=$(BLD_DIR)%.o)

SCRIPT_LD = asm.lds

# Create app
.PHONY: all
all: $(APP) binary

# .build/ directory
$(BLD_DIR):
	mkdir -p $(BLD_DIR)

# Build for each source file
$(LIB_OBJ) $(APP_OBJ): $(BLD_DIR)%.o: src/%.c | $(BLD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Build assembly code
$(ASM_OBJ): $(BLD_DIR)%.o: asm/%.S | $(BLD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create libdrone library
.PHONY: $(LIB)
$(LIB): $(LIB_PATH)
$(LIB_PATH): $(LIB_OBJ)
	$(AR) $(ARFLAGS) $@ $^
	@echo "Libdrone is ready"

# Create raspdrone elf
.PHONY: $(APP)
$(APP): $(APP_PATH)
$(APP_PATH): $(ASM_OBJ) $(APP_OBJ) $(LIB_PATH)
	$(LD) $(LDFLAGS) $(ASM_OBJ) $(APP_OBJ) $(LIB_PATH) -o $@
	@echo "App is ready"

# Create raspdrone binary
.PHONY: binary
binary: $(BIN_PATH)
$(BIN_PATH): $(APP_PATH)
	$(CV) -O binary $< $@

# Delete .build/ folder
.PHONY: clean
clean:
	rm -rf $(BLD_DIR)
	@echo "Deleted $(BLD_DIR) folder successfully"