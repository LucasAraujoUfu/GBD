all:
	make Obj
	mkdir -p bin
	g++ -o bin/main obj/disco.o obj/main.o

Obj:
	make disco
	make main

disco:
	mkdir -p obj
	g++ -c lib/disco.cpp -o obj/disco.o

main:
	mkdir -p obj
	g++ -c main.cpp -o obj/main.o