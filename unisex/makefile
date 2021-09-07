CC=gcc
CFLAGS=-lpthread
LDFLAGS=
EXEC=unisex

all: $(EXEC)

$(EXEC): unisex.o
	@ $(CC) -o $(EXEC) unisex.o $(CFLAGS)

unisex.o: unisex.c unisex.h
	@ $(CC) -o unisex.o -c unisex.c $(CFLAGS)

clean:
	@ rm -rf *.o

mrproper: clean
	@ rm -rf $(EXEC)