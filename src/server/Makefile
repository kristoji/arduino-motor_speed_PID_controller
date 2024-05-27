TARGET=server
HEX = $(TARGET).hex
BINS=$(TARGET).elf 
OBJS=my_uart.o \
	 shaft_encoder.o \
	 h_bridge.o 
HEADERS=my_uart.h \
		shaft_encoder.h \
		h_bridge.h \
		server.h 

# the file below contains the actual rules
include ./avr_common/avr.mk
