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
C_SRCS = $(NAME).c cstart.c malloc.c printf.c gl.c fb.c cpu/CPU.c
S_SRCS = start.s

all : $(NAME).bin

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

install: $(NAME).bin
	rpi-install.py $(NAME).bin

.PHONY: all clean

.PRECIOUS: %.exe %.o

# include dependency info
#  "-" suppresses errors
-include $(SRCS:.c=.d)
