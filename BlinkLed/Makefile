PRJ_NAME   = Blink
CC         = arm-none-eabi-gcc
SRC        = $(wildcard src/*.c)
ASRC       = $(wildcard src/*.s)
OBJ        = $(SRC:.c=.o) $(ASRC:.s=.o)
OBJCOPY    = arm-none-eabi-objcopy
OBJDUMP    = arm-none-eabi-objdump
PROGRAMMER = openocd
PGFLAGS    = -f interface/stlink-v2.cfg -f target/stm32f1x.cfg -c "program $(PRJ_NAME).elf verify reset" -c shutdown
DEVICE     = STM32F103xB
OPTIMIZE   = -Og
LDSCRIPT   = stm32f103c8tx.ld
CFLAGS     = -g3 -Wall $(OPTIMIZE) -mcpu=cortex-m3 -mlittle-endian -mthumb -I inc/ -D $(DEVICE)
ASFLAGS    =  $(CFLAGS)
LDFLAGS    = -T $(LDSCRIPT) -Wl,--gc-sections


all: $(PRJ_NAME).elf

$(PRJ_NAME).elf: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS)
	arm-none-eabi-size $(PRJ_NAME).elf
.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

.s.o:
	$(CC) -c $(ASFLAGS) $< -o $@

clean:
	rm -f $(OBJ) $(PRJ_NAME).elf $(PRJ_NAME).hex $(PRJ_NAME).bin

burn:
	$(PROGRAMMER) $(PGFLAGS)

fast: all burn

hex: all
	$(OBJCOPY) -O ihex $(PRJ_NAME).elf $(PRJ_NAME).hex

bin: all
	$(OBJCOPY) -O binary $(PRJ_NAME).elf $(PRJ_NAME).bin
