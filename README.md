# Description
This repository contains source code examples for the ARM Cortex-M3 STM32F103C8T6 microcontroller.

# Dependencies

* arm-none-eabi-gcc
* arm-none-eabi-gdb
* arm-none-eabi-newlib
* openocd (>= 0.8.0)
* make

# How to compile and upload the examples

## Compile
```
$ make
```
You can change the default optimization level (-Og) through the OPT flag:
```
$ make clean
$ make OPT=-O3
```

## Upload: (using ST-Link V2)
```
$ make flash
```

## Clean output files:
```
$ make clean
```

## Generate .bin file:
```
$ make bin
```

## Generate .hex file:
```
$ make hex
```

