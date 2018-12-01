UNAME := $(shell uname -s)

sp_generate:	sp_generate.cpp
	g++ -c -std=c++14 -O3 -o sp_generate.o sp_generate.cpp
ifeq ($(UNAME),Linux)
	g++ -o sp_generate sp_generate.o -lbsd
else
	g++ -o sp_generate sp_generate.o
endif

clean:
	rm sp_generate
	rm sp_generate.o
	
