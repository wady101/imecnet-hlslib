CC=g++
CFLAGS=-std=c++17 -I../HLSgenerated/ -Idrivers/
OCFLAGS=-lstdc++fs
SRC=accel_loader.cpp drivers/platform-xlnk.cpp
DEF_BOARD="PYNQ"
all: test clean

clean:
	rm -f ./a.out
test:
	@$(CC) -D$(DEF_BOARD) $(CFLAGS) $(SRC) $(OCFLAGS)
	./a.out

