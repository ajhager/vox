all:
	cp ../lib_x86_64/sunvox.so ./
	cp ../../songs/test.sunvox ./
	gcc -m64 -g -c test.cpp
	gcc test.o -m64 -lstdc++ -ldl -lm -o test

clean:
	rm -f *.o *.so test *.sunvox
