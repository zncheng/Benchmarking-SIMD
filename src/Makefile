CC =  g++

CFLAGS = -std=c++11 -mavx2 -mavx -mfma

all: td tf ti

td: test_double.cpp
	$(CC) test_double.cpp $(CFLAGS) -o td

tf: test_float.cpp
	$(CC) test_float.cpp $(CFLAGS) -o tf

ti: test_int.cpp
	$(CC) test_int.cpp $(CFLAGS) -o ti

clean:
	rm tf td ti
