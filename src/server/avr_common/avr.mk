CC=avr-gcc
INCLUDE_DIRS=-I. -I../avr_common
CC_OPTS = -O1 -mmcu=atmega2560 $(INCLUDE_DIRS) -D__AVR_3_BYTE_PC__ -DF_CPU=16000000UL 
CC_OPTS += -Wall --std=gnu99 -funsigned-char -funsigned-bitfields  -fshort-enums -Wstrict-prototypes 
# CC_OPTS += -Wl,-u,vfprintf -lprintf_flt -lm

AVRDUDE=avrdude

AVRDUDE_PORT = /dev/ttyACM0

AVRDUDE_WRITE_FLASH = -U flash:w:$(HEX):i
AVRDUDE_FLAGS = -p m2560 -P $(AVRDUDE_PORT) -b 115200
AVRDUDE_FLAGS += -C /usr/share/arduino/hardware/tools/avrdude.conf
AVRDUDE_FLAGS += -D -q -V -c wiring

.phony:	clean all run

all:	$(HEX) 

run:	$(HEX)
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)

%.o:	%.c 
	$(CC) $(CC_OPTS) -c  -o $@ $<

%.elf:	%.o $(OBJS)
	$(CC) $(CC_OPTS) -o $@ $< $(OBJS) $(LIBS) -lm -Wl,-u,vfprintf -lprintf_flt

%.hex:	%.elf
	avr-objcopy -O ihex -R .eeprom $< $@

clean:	
	rm -rf $(OBJS) $(BINS) *.hex *~ *.o

.SECONDARY:	$(OBJS)

# Include the dependency files
-include $(OBJS:.o=.d)

# Generate dependency files
%.d: %.c
	@$(CC) -MM $(CC_OPTS) $< > $@.tmp
	@sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.tmp > $@
	@rm -f $@.tmp