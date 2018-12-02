UNAME := $(shell uname -s)

sp-generate:	sp-generate.cpp
	g++ -c -std=c++14 -O3 -o sp-generate.o sp-generate.cpp

ifeq ($(UNAME),Linux)
	g++ -o sp-generate sp-generate.o -lbsd
endif

ifeq ($(UNAME),Darwin)
	g++ -o sp-generate sp-generate.o
endif

clean:
	rm sp-generate
	rm sp-generate.o
	rm sp-network.c
	inverse-sp-network.c

	
