PRJ_NAME   = Blink
CC         = arm-none-eabi-gcc
SRCDIR     = src
SRC        = $(wildcard $(SRCDIR)/*.c)
ASRC       = $(wildcard $(SRCDIR)/*.s)
OBJ        = $(SRC:.c=.o) $(ASRC:.s=.o)
OBJCOPY    = arm-none-eabi-objcopy
OBJDUMP    = arm-none-eabi-objdump
PROGRAMMER = openocd
PGFLAGS    = -f openocd.cfg -c "program $(PRJ_NAME).elf verify reset" -c shutdown
DEVICE     = STM32F103xB
OPT       ?= -Og
LDSCRIPT   = stm32f103c8tx.ld
CFLAGS     = -fdata-sections -ffunction-sections -g3 -Wall -mcpu=cortex-m3 -mlittle-endian -mthumb -I inc/ -D $(DEVICE) $(OPT)
ASFLAGS    =  $(CFLAGS)
LDFLAGS    = -T $(LDSCRIPT) -Wl,--gc-sections --specs=nano.specs --specs=nosys.specs

.PHONY: all clean flash burn hex bin

all: $(PRJ_NAME).elf

$(PRJ_NAME).elf: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS)
	arm-none-eabi-size $(PRJ_NAME).elf

%.o: %.c $(DEPS)
	$(CC) -MMD -c $(CFLAGS) $< -o $@

%.o: %.s $(DEPS)
	$(CC) -MMD -c $(ASFLAGS) $< -o $@

-include $(SRCDIR)/*.d

clean:
	rm -f $(OBJ) $(PRJ_NAME).elf $(PRJ_NAME).hex $(PRJ_NAME).bin $(SRCDIR)/*.d

flash: $(PRJ_NAME).elf
	$(PROGRAMMER) $(PGFLAGS)

burn: $(PRJ_NAME).elf
	$(PROGRAMMER) $(PGFLAGS)

hex: $(PRJ_NAME).elf
	$(OBJCOPY) -O ihex $(PRJ_NAME).elf $(PRJ_NAME).hex

bin: $(PRJ_NAME).elf
	$(OBJCOPY) -O binary $(PRJ_NAME).elf $(PRJ_NAME).bin
