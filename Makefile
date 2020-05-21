OBJ=gcc -c $< -o $@

.PHONY: clean

all: folder lib1.a lib2.a lib3.a lib4.a lib5.a lib6.a main

folder:
	mkdir -p build
	mkdir -p libs

main: build/main.o
	gcc build/main.o -L./libs/ -l1 -l2 -l3 -l4 -l5 -l6 -o main

lib1.a: build/mSC.o 
	ar cr libs/lib1.a build/mSC.o
	ranlib libs/lib1.a

lib2.a: build/myTerm.o
	ar cr libs/lib2.a build/myTerm.o
	ranlib libs/lib2.a

lib3.a: build/myBigChars.o
	ar cr libs/lib3.a build/myBigChars.o
	ranlib libs/lib3.a

lib4.a: build/myReadKey.o 
	ar cr libs/lib4.a build/myReadKey.o 
	ranlib libs/lib4.a

lib5.a: build/Signal.o 
	ar cr libs/lib5.a build/Signal.o 
	ranlib libs/lib5.a

lib6.a: build/CPU.o 
	ar cr libs/lib6.a build/CPU.o 
	ranlib libs/lib6.a


build/main.o: main.c
	$(OBJ)

build/mSC.o: mSC.c
	$(OBJ)

build/myTerm.o: myTerm.c
	$(OBJ)

build/myBigChars.o: myBigChars.c
	$(OBJ)

build/myReadKey.o: myReadKey.c 
	$(OBJ)

build/Signal.o: Signal.c 
	$(OBJ)

build/CPU.o: CPU.c 
	$(OBJ)


clean:
	rm -rf build
	rm -rf *.a 
	rm -rf main
	rm -rf libs
	
