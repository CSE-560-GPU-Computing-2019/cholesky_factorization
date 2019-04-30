.PHONY: all clean

all:
	mkdir -p bin
	g++ -o ./bin/cpu Cholesky.cpp
	nvcc -std=c++11 Cholesky.cu -o ./bin/gpu

clean:
	rm -rf ./bin
