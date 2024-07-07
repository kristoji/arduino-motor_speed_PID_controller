TARGET=server
HEX = $(TARGET).hex
BINS=$(TARGET).elf 
OBJS=my_uart.o \
	 shaft_encoder.o \
	 pid.o \
	 h_bridge.o \
	 odometry.o \
	 time.o
HEADERS=my_uart.h \
		shaft_encoder.h \
		pid.h \
		h_bridge.h \
		server.h \
		odometry.h \
		time.h
# the file below contains the actual rules
include ./avr.mk
