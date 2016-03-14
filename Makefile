ARMGNU ?= arm-none-eabi

CC = $(ARMGNU)-gcc
LD = $(ARMGNU)-ld
AS = $(ARMGNU)-as
OBJCOPY = $(ARMGNU)-objcopy
OBJDUMP = $(ARMGNU)-objdump
STRIP = $(ARMGNU)-strip

CPPFLAGS = -Iinclude/ -Icpu/ -Iroms/
CFLAGS = -std=c99 -Wall -Og -g -ffreestanding $(CPPFLAGS)
ASFLAGS =
LDFLAGS  = -nostdlib -T memmap -Llib/
LDLIBS = -lpi -lgcc

.SUFFIXES:

NAME = main
CONTROLLER = main_controller
GPU = main_gpu

C_SRCS = $(NAME).c cstart.c malloc.c printf.c gl.c fb.c cpu/CPU.c cpu/MEM.c roms/cpu_instrs.c
S_SRCS = start.s

ROMS = Pokemon_Red.gb Tetris.gb

CONTROLLER_SRCS = $(CONTROLLER).c cstart.c malloc.c printf.c gl.c fb.c interrupt.c controller/controller.c 
GPU_SRCS = $(GPU).c cstart.c malloc.c printf.c gl.c fb.c interrupt.c controller/controller.c cpu/GPU.c cpu/MEM.c roms/bgblogo.c cpu/CPU.c

# all : $(NAME).bin
all : $(GPU).c

# GPU Specific Make
$(GPU).exe : $(S_SRCS:.s=.o) $(GPU_SRCS:.c=.o)
	$(LD) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(GPU).bin : $(GPU).exe
	$(OBJCOPY) $< -O binary $@

gpu : $(GPU).bin

install-gpu: $(GPU).bin
	rpi-install.py $(GPU).bin
	rm screenlog.0 
	screen -L /dev/tty.SLAB_USBtoUART 115200

# Controller Specific Make - don't know if this is correct/efficient but it works
$(CONTROLLER).exe : $(S_SRCS:.s=.o) $(CONTROLLER_SRCS:.c=.o)
	$(LD) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(CONTROLLER).bin : $(CONTROLLER).exe
	$(OBJCOPY) $< -O binary $@

controller : $(CONTROLLER).bin

install-controller: $(CONTROLLER).bin
	rpi-install.py $(CONTROLLER).bin

# General Make Rules
%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	$(CC) -MM $(CFLAGS) -c $< > $*.d

%.exe: $(S_SRCS:.s=.o) $(C_SRCS:.c=.o)
	$(LD) $(LDFLAGS) $^ $(LDLIBS) -o $@

%.bin: %.exe
	$(OBJCOPY) $< -O binary $@

%.list: %.o
	$(OBJDUMP) -d --no-show-raw-insn $< > $@

clean :
	rm -rf *.bin *.exe *.o *.d *.list
	rm -rf lib/*.bin lib/*.exe lib/*.o lib/*.d lib/*.list
	rm -rf cpu/*.bin cpu/*.exe cpu/*.o cpu/*.d cpu/*.list
	rm -rf controller/*.bin controller/*.exe controller/*.o controller/*.d controller/*.list
	rm -rf roms/*.d
	clear
	

install: $(NAME).bin
	rpi-install.py $(NAME).bin
	rm screenlog.0 
	screen -L /dev/tty.SLAB_USBtoUART 115200

Ubuntuinstall: $(NAME).bin
	rpi-install.py /dev/ttyUSB0 $(NAME).bin
	rm screenlog.0 
	screen -L /dev/ttyUSB0 115200


#rom_build: $(ROMS:.gb=.h)
#
#%.h: %.gb
#	xxd -i roms/$< > roms/$@

# Should definitely generalize this to dump all files in the rom directory
rom_dump:
	xxd -i roms/Pokemon_Red.gb > roms/Pokemon_Red.h
	xxd -i roms/cpu_instrs.gb > roms/cpu_instrs.h
	xxd -i roms/bgblogo.gb > roms/bgblogo.c

.PHONY: all clean

.PRECIOUS: %.exe %.o

# include dependency info
#  "-" suppresses errors
-include $(SRCS:.c=.d)
