example0: example0.o RBF.o
	$(CC) -o $@ $^ -Wl,example0.lds -Wl,-Map=example0.map

