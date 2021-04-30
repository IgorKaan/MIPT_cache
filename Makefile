CC = g++-10 -std=c++2a

all:		clear build_main link_all

build_main: 	main.cpp cache.hpp
		$(CC) main.cpp cache.hpp -Wall -Werror -c

build_all:      build_main

link_all: 	main.o	
		$(CC) main.o -o main_cache

clear: 
		rm -rf *.o *.lst main_cache *.out
