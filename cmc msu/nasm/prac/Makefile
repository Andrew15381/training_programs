all:
	nasm -f elf f.asm
	gcc main-int.c f.o $(CFLAGS) -o main-int.exe
clean:
	rm -rf main-int.exe f.o
