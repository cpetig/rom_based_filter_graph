CFLAGS=-O0 -g

all: example0 example1 example2

COMMONOBJECTS=RBF.o linux_timer.o

clean:
	rm example0 *.o

example0: example0.o $(COMMONOBJECTS) 
	$(CC) -o $@ $^ -Wl,-Map=example0.map

example1: example1.o $(COMMONOBJECTS) 
	$(CC) -o $@ $^

example2: example2.o $(COMMONOBJECTS) 
	$(CC) -o $@ $^

