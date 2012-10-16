all:
	cp ../lib_x86_64/sunvox.dylib ./
	cp ../../songs/test.sunvox ./
	gcc -m64 -g -c test.cpp
	gcc -m64 -lstdc++ -o test test.o

clean:
	rm -f *.o *.dylib test *.sunvox