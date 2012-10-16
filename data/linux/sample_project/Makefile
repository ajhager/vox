all:
	cp ../lib_x86/sunvox.so ./
	cp ../../songs/test.sunvox ./
	gcc -m32 -g -c test.cpp
	gcc -m32 -lstdc++ -ldl -lm -o test test.o

clean:
	rm -f *.o *.so test *.sunvox