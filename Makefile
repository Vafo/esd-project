TARGET := main
SRC_FOLDER := src
BUILD_FOLDER := build

SRCS := \
	$(SRC_FOLDER)/app.c \
	$(SRC_FOLDER)/uart0.c \
	$(SRC_FOLDER)/uart1.c

INC_FOLDER := . \
	./src 

OBJS := $(SRCS:%.c=./build/%.o)
DEPS := $(OBJS:%.o=%.d)

MCU := atmega128

INC := $(addprefix -I, $(INC_FOLDER))

CC := avr-gcc
OBJCOPY := avr-objcopy
AVRDUDE := avrdude

C_FLAGS := $(INC) -mmcu=$(MCU) -funsigned-char -funsigned-bitfields -O3 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax -Wall
LDFLAGS := -Wl,-Map=$(TARGET).map,--cref

program: build
	$(AVRDUDE) -c arduino -p m128 -P /dev/ttyUSB0 -U flash:w:$(TARGET).hex	

build: $(TARGET).hex

%.hex: %.elf
	$(OBJCOPY) -O ihex -R .eeprom -R .fuse -R .lock -R .signature $< $@

$(TARGET).elf: $(OBJS)
	$(CC) $(C_FLAGS) $^ --output $@ $(LDFLAGS)

-include $(DEPS)

$(BUILD_FOLDER)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) -MMD -c $< -o $@

.PHONY: clean connect

connect:
	picocom /dev/ttyUSB0 -b 9600 --omap crcrlf -c

clean:
	rm -rf build
	rm $(TARGET).elf
	rm $(TARGET).map
	rm $(TARGET).hex
