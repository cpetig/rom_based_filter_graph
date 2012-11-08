CFLAGS=-O0 -g -Wall

all: example0 example1 example2 example3

COMMONOBJECTS=RBF.o linux_timer.o linux_crit_sect.o
COMMONBUFOBJECTS=$(COMMONOBJECTS) RBF_buffer.o

clean:
	rm example0 *.o

example0: example0.o $(COMMONOBJECTS) 
	$(CC) -o $@ $^ -Wl,-Map=example0.map

example1: example1.o $(COMMONOBJECTS) 
	$(CC) -o $@ $^

example2: example2.o $(COMMONBUFOBJECTS) 
	$(CC) -o $@ $^

example3: example3.o $(COMMONBUFOBJECTS) 
	$(CC) -o $@ $^

