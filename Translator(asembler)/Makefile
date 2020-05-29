OBJ=gcc -I../ -c $< -o $@

.PHONY: clean

all: folder sat

folder:
	mkdir -p build

sat: build/sat.o
	gcc build/sat.o  -L../libs/ -l1 -l2 -o sat

build/sat.o: sat.c
	$(OBJ)

clean:
	rm -rf build
	rm -rf sat
	rm -rf libs