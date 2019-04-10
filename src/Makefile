CC =  g++

CFLAGS = -std=c++11 -mavx2 -mfma

all: td tf

td: test_double.cpp
	$(CC) test_double.cpp $(CFLAGS) -o td

tf: test_float.cpp
	$(CC) test_float.cpp $(CFLAGS) -o tf


clean:
	rm tf td
