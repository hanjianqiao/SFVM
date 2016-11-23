all:
	g++ src/sfvm.cpp src/debug.cpp src/string.cpp -o sfvm -std=c++0x

clean:
	rm sfvm
