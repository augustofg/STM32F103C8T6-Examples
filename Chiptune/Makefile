PRJ_NAME   = Chiptune
CC         = arm-none-eabi-gcc
SRCDIR     = src
SRC        = $(wildcard $(SRCDIR)/*.c)
ASRC       = $(wildcard $(SRCDIR)/*.s)
OBJ        = $(SRC:.c=.o) $(ASRC:.s=.o)
OBJCOPY    = arm-none-eabi-objcopy
OBJDUMP    = arm-none-eabi-objdump
PROGRAMMER = openocd
PGFLAGS    = -f interface/stlink-v2.cfg -f target/stm32f1x.cfg -c "program $(PRJ_NAME).elf verify reset" -c shutdown
DEVICE     = STM32F103xB
DBG_OPT	   = -Og
REL_OPT    = -O3
LDSCRIPT   = stm32f103c8tx.ld
CFLAGS     = -g3 -Wall -mcpu=cortex-m3 -mlittle-endian -mthumb -I inc/ -D $(DEVICE)
ASFLAGS    =  $(CFLAGS)
LDFLAGS    = -T $(LDSCRIPT) -Wl,--gc-sections --specs=nano.specs --specs=nosys.specs

.PHONY: all rel clean burn hex bin fast fastrel
all: CFLAGS+=$(DBG_OPT)
all: $(PRJ_NAME).elf

rel: CFLAGS+=$(REL_OPT)
rel: $(PRJ_NAME).elf

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

burn:
	$(PROGRAMMER) $(PGFLAGS)

hex:
	$(OBJCOPY) -O ihex $(PRJ_NAME).elf $(PRJ_NAME).hex

bin:
	$(OBJCOPY) -O binary $(PRJ_NAME).elf $(PRJ_NAME).bin

fast: all burn

fastrel: rel burn
