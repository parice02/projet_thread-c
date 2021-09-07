CC=gcc
CFLAGS=-lpthread
LDFLAGS=
EXEC=unisex

all: $(EXEC)

$(EXEC): unisex3.o
	@ $(CC) -o $(EXEC) unisex3.o $(CFLAGS)

unisex.o: unisex3.c unisex3.h
	@ $(CC) -o unisex3.o -c unisex3.c $(CFLAGS)

clean:
	@ rm -rf *.o

mrproper: clean
	@ rm -rf $(EXEC)