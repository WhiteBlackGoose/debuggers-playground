run: build-dbg build-target
	./dbg target

build-target:
	nasm -f elf64 a.asm -o a.o
	ld a.o -o target

build-dbg:
	gcc ./main.c -o dbg
