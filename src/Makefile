CC =  g++

CFLAGS = -std=c++11 -O3 -mavx -mavx2 -mfma
CFLAGS1 = -std=c++11 -mavx512f

all: td tf ti ed ef mf

td: test_double.cpp
	$(CC) test_double.cpp $(CFLAGS) -o td

tf: test_float.cpp
	$(CC) test_float.cpp $(CFLAGS) -o tf

ti: test_int.cpp
	$(CC) test_int.cpp $(CFLAGS) -o ti

ed: EC2_double.cpp
	$(CC) EC2_double.cpp $(CFLAGS1) -o ed

ef: EC2_float.cpp
	$(CC) EC2_float.cpp $(CFLAGS1) -o ef

mf: 2d_double.cpp
	$(CC) 2d_double.cpp $(CFLAGS) -o mf

clean:
	rm tf td ti ed ef mf
