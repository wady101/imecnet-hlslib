CC=g++
CFLAGS=-std=c++17 -I../HLSgenerated/
OCFLAGS=-lstdc++fs

all: test clean

clean:
	rm -f ./a.out
test:
	@$(CC) $(CFLAGS) accel_loader.cpp $(OCFLAGS)
	./a.out

