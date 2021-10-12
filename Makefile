# Makefile Written by:
# Santiago R Guiral - Emmanuel Gomez
# UdeA - RTS - Sep 2021

XENO_CONFIG := /bin/xeno-config

CC = gcc
RM = rm -rf
CCFLAGS = -O0 -Wall -ggdb3
THREADS = -lpthread
RTLIBS =-lrt

CFLAGS := $(shell $(XENO_CONFIG)   --posix --alchemy --cflags)
LDFLAGS := $(shell $(XENO_CONFIG)  --posix --alchemy --ldflags)

#FILES
COARSE = coarsemm
UTILS = utils
QUEUE =pt_queue
MARS = mars
PC= procons
PCTASK = task_definition
PCTIMES = periodic_settings
IMAGEN = image

$(MARS): $(MARS).o $(UTILS).o $(QUEUE).o $(COARSE).o $(PC).o $(PCTASK).o $(PCTIMES).o $(IMAGEN).o
	$(CC) -o $@ $+ $(CFLAGS) $(LDFLAGS)

$(MARS).o: $(MARS).c $(UTILS).h $(QUEUE).h $(COARSE).h $(PC).h $(PCTASK).h $(PCTIMES).h $(IMAGEN).h
	$(CC) $(CCFLAGS) -c $(MARS).c $(CFLAGS) $(LDFLAGS)

$(UTILS).o: $(UTILS).c $(UTILS).h
	$(CC) $(CCFLAGS) -c $(UTILS).c

$(QUEUE).o: $(QUEUE).c $(QUEUE).h
	$(CC) $(CCFLAGS) -c $(QUEUE).c

$(COARSE).o: $(COARSE).c $(COARSE).h
	$(CC) $(CCFLAGS) -c $(COARSE).c $(THREADS)

$(PCTASK).o: $(PCTASK).c $(PCTASK).h
	$(CC) $(CCFLAGS) -c $(PCTASK).c $(RTLIBS) $(THREADS)

$(PCTIMES).o: $(PCTIMES).c $(PCTIMES).h
	$(CC) $(CCFLAGS) -c $(PCTIMES).c $(RTLIBS) $(THREADS)

$(PC).o: $(PC).c $(PC).h
	$(CC) $(CCFLAGS) -c $(PC).c $(RTLIBS) $(THREADS)

$(IMAGEN).o: $(IMAGEN).c $(IMAGEN).h
	$(CC) $(CCFLAGS) -c $(IMAGEN).c

clean:
	$(RM) *.o
	$(RM) $(MARS)
