CC=gcc
CFLAGS=-lpthread
LDFLAGS=
EXEC=unisex

all: $(EXEC)

$(EXEC): unisex2.o
	@ $(CC) -o $(EXEC) unisex2.o $(CFLAGS)

unisex.o: unisex2.c unisex2.h
	@ $(CC) -o unisex2.o -c unisex2.c $(CFLAGS)

clean:
	@ rm -rf *.o

mrproper: clean
	@ rm -rf $(EXEC)