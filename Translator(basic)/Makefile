OBJ=gcc  -I../ -c $< -o $@

.PHONY: clean

all: folder lib1.a basic

folder:
	mkdir -p build
	mkdir -p libs

basic: build/basic.o
	g++ build/basic.o -L libs/ -l1 -L../libs/ -l1 -l2 -o basic

lib1.a: build/parse_line.o 
	ar cr libs/lib1.a build/parse_line.o
	ranlib libs/lib1.a

build/basic.o: basic.c
	$(OBJ)

build/parse_line.o: parse_line.c
	$(OBJ)

clean:
	rm -rf build
	rm -rf basic
	rm -rf libs