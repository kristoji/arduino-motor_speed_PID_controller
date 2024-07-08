CC = gcc
CCOPTS = -std=gnu99 
TARGET = client
BINS = $(TARGET).elf
SRCS = serial_linux.c client.c sample.c plot.c jstick.c
OBJS = $(SRCS:.c=.o)

.PHONY: clean all

all: $(BINS)

$(BINS): $(OBJS)
	$(CC) $(CCOPTS) -o $@ $^ -lm

%.o: %.c
	$(CC) $(CCOPTS) -c -o $@ $<

# Include the dependency files
-include $(OBJS:.o=.d)

# Generate dependency files
%.d: %.c
	@$(CC) -MM $(CCOPTS) $< > $@.tmp
	@sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.tmp > $@
	@rm -f $@.tmp

# serial_linux <serial_file> <baudrate>
run: $(BINS)
	./$(BINS) /dev/ttyACM0 19200

debug: $(BINS)
	./$(BINS) /dev/ttyACM0 19200 1

clean:
	rm -rf *~ $(BINS) $(OBJS) $(OBJS:.o=.d)
