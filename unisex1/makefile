CC=gcc
CFLAGS=-lpthread
LDFLAGS=
EXEC=unisex

all: $(EXEC)

$(EXEC): unisex1.o
	@ $(CC) -o $(EXEC) unisex1.o $(CFLAGS)

unisex.o: unisex1.c unisex1.h
	@ $(CC) -o unisex1.o -c unisex1.c $(CFLAGS)

clean:
	@ rm -rf *.o

mrproper: clean
	@ rm -rf $(EXEC)