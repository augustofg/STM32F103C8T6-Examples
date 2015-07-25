PRJ_NAME   = Template
CC         = arm-none-eabi-gcc
SRC        = $(wildcard src/*.c)
ASRC       = $(wildcard src/*.s)
OBJ        = $(SRC:.c=.o) $(ASRC:.s=.o)
OBJCOPY    = arm-none-eabi-objcopy
OBJDUMP    = arm-none-eabi-objdump
PROGRAMMER = st-flash
DEVICE     = STM32F103xB
OPTIMIZE   = -O2
LDSCRIPT   = stm32f103c8tx.ld
CFLAGS     = -g -Wall $(OPTIMIZE) -mcpu=cortex-m3 -mlittle-endian -mthumb -I inc/ -D $(DEVICE)
ASFLAGS    =  $(CFLAGS)
LDFLAGS    = -T $(LDSCRIPT) -Wl,--gc-sections


all: $(PRJ_NAME).elf

$(PRJ_NAME).elf: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS)
	$(OBJCOPY) -O ihex $(PRJ_NAME).elf $(PRJ_NAME).hex
	$(OBJCOPY) -O binary $(PRJ_NAME).elf $(PRJ_NAME).bin
	arm-none-eabi-size $(PRJ_NAME).elf
.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

.s.o:
	$(CC) -c $(ASFLAGS) $< -o $@

clean:
	rm -f $(OBJ) *.map *.elf *.hex

burn:
	st-flash write $(PRJ_NAME).bin 0x08000000
rst:

fast: all burn
