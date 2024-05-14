# If you want to use this on your own machine, change CLASSDIR and/or INCLUDE and LIBS so
# that they point to the correct directories/library.

CLASSDIR = ./Libfdr
INCLUDE = -I$(CLASSDIR)/include
LIBS = $(CLASSDIR)/lib/libfdr.a
CC = gcc

EX  =  bin/proje

all: $(EX) 

clean: 
	rm -f bin/*

bin/proje: src/proje.c $(LIBS)
	$(CC) $(INCLUDE) -o bin/proje src/proje.c $(LIBS)
