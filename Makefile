CFLAGS=-O0 -g

all: example0

OBJECTS=example0.o RBF.o linux_timer.o

clean:
	rm example0 *.o

example0_linux.lds: linux_lds_example0.patch
	$(LD) --verbose >example0_linux.lds
	patch <linux_lds_example0.patch

example0: $(OBJECTS) example0_linux.lds
	$(CC) -o $@ $(OBJECTS) -Wl,-T -Wl,example0_linux.lds -Wl,-Map=example0.map

