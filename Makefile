CFLAGS=-O0 -g

all: example0

clean:
	rm example0 *.o

example0: example0.o RBF.o linux_timer.o
	$(CC) -o $@ $^ -Wl,example0.lds -Wl,-Map=example0.map

