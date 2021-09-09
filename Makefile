# Makefile Written by:
# Santiago R Guiral - Emmanuel Gomez
# UdeA - RTS - Sep 2021

XENO_CONFIG := /bin/xeno-config

CC = gcc
RM = rm -rf
ENCODEFLAGS = -lavcodec -lavutil -lz -lm
CCFLAGS = -O0 - Wall -ggdb3
THREADS = -lpthread

CFLAGS := $(shell $(XENO_CONFIG)   --posix --alchemy --cflags)
LDFLAGS := $(shell $(XENO_CONFIG)  --posix --alchemy --ldflags)

#FILES
COARSE = coarse
ENCODE = encode
UTILS = utils
QUEUE =pt_queue
MARS = mars


$(MARS): $(MARS).c $(ENCODE).o $(UTILS).o $(QUEUE).o $(COARSE).o
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

$(ENCODE).o: $(ENCODE).c $(ENCODE).h
	$(CC) $(CCFLAGS) -c $(ENCODE).c $(ENCODEFLAGS)

$(UTILS).o: $(UTILS).c $(UTILS).h
	$(CC) $(CCFLAGS) -c $(UTILS).c

$(QUEUE).o: $(QUEUE).c $(QUEUE).h
	$(CC) $(CCFLAGS) -c $(QUEUE).c

$(COARSE).o: $(COARSE).c $(COARSE).h
	$(CC) $(CCFLAGS) -c $(COARSE).c $(THREADS)

clean:
	$(RM) *.o
	$(RM) $(MARS)
