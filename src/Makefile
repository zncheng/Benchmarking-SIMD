CC =  g++

CFLAGS = -std=c++11 -mavx2 -mavx -mfma
CFLAGS1 = -std=c++11 -mavx512f

all: td tf ti ed ef

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

clean:
	rm tf td ti ed ef
