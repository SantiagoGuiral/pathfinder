CC = gcc
RM = rm -rf
ENCODEFLAGS = -lavcodec -lavutil -lz -lm
CFLAGS = -O0 - Wall -ggdb3
ENCODE = encode
MARS = mars

$(MARS): $(MARS).c $(ENCODE).o

$(ENCODE).o: $(ENCODE).c $(ENCODE).h
	$(CC) $(CFLAGS) -c $(ENCODE).c $(ENCODEFLAGS)

clean:
	$(RM) *.o
	$(RM) $(MARS)
