ARMGNU ?= arm-none-eabi

CC = $(ARMGNU)-gcc
LD = $(ARMGNU)-ld
AS = $(ARMGNU)-as
OBJCOPY = $(ARMGNU)-objcopy
OBJDUMP = $(ARMGNU)-objdump
STRIP = $(ARMGNU)-strip

CPPFLAGS = -Iinclude/ -Icpu/
CFLAGS = -std=c99 -Wall -Og -g -ffreestanding $(CPPFLAGS)
ASFLAGS =
LDFLAGS  = -nostdlib -T memmap -Llib/
LDLIBS = -lpi -lgcc

.SUFFIXES:

NAME = main_cpu
CONTROLLER = main_controller

C_SRCS = $(NAME).c cstart.c malloc.c printf.c gl.c fb.c cpu/CPU.c controller/controller.c
S_SRCS = start.s

CONTROLLER_SRCS = $(CONTROLLER).c cstart.c malloc.c printf.c gl.c fb.c controller/controller.c

all : $(NAME).bin

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
	rm -f *.bin *.exe *.o *.d *.list
	rm -f lib/*.bin lib/*.exe lib/*.o lib/*.d lib/*.list
	clear

install: $(NAME).bin
	rpi-install.py $(NAME).bin

.PHONY: all clean

.PRECIOUS: %.exe %.o

# include dependency info
#  "-" suppresses errors
-include $(SRCS:.c=.d)
