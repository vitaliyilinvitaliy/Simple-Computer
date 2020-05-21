OBJ=gcc  -I../ -c $< -o $@

.PHONY: clean

all: folder basic

folder:
	mkdir -p build

basic: build/basic.o
	g++ build/basic.o  -L../libs/ -l1 -l2 -o basic

build/basic.o: basic.c
	$(OBJ)

clean:
	rm -rf build
	rm -rf basic