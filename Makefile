CFLAGS=-O0 -g

all: example0

OBJECTS=example0.o RBF.o linux_timer.o

clean:
	rm example0 *.o

example0: $(OBJECTS) example0_cygwin.lds
	$(CC) -o $@ $(OBJECTS) -Wl,-Map=example0.map

