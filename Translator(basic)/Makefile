OBJ=gcc  -I../ -I./ -c $< -o $@

.PHONY: clean

all: folder lib1.a librpn.a basic

folder:
	mkdir -p build
	mkdir -p libs

basic: build/basic.o
	gcc build/basic.o -I./ -L libs/ -l1 -lrpn -L../libs/ -l1 -l2 -o basic

lib1.a: build/parse_line.o 
	ar cr libs/lib1.a build/parse_line.o
	ranlib libs/lib1.a

#libstack.a: build/Stack.o 
#	ar cr libs/libstack.a build/Stack.o 
#	ranlib libs/libstack.a

librpn.a: build/RPN.o 
	ar cr libs/librpn.a build/RPN.o 
	ranlib libs/librpn.a


build/basic.o: basic.c
	$(OBJ)

build/parse_line.o: parse_line.c
	$(OBJ)

#build/Stack.o: Stack.c 
#	$(OBJ)

build/RPN.o: RPN.c 
	$(OBJ)


clean:
	rm -rf build
	rm -rf basic
	rm -rf libs