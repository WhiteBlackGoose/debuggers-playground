run-all: run-p1 run-p2

run-p1: build-dbg-p1 build-target-p1
	./dbg1 target1

run-p2: build-dbg-p2 build-target-p2
	./dbg2 target2

run-target-p1: build-target-p1
	./target1

run-target-p2: build-target-p2
	./target2

inspect-target-p2: build-target-p2
	objdump -d target2
	readelf -h target2

build-target-p1:
	nasm -f elf64 ./part1/target.asm -o a.o
	ld a.o -o target1

build-target-p2:
	nasm -f elf64 ./part2/target.asm -o a.o
	ld a.o -o target2

build-dbg-p1:
	gcc ./part1/dbg.c -o dbg1

build-dbg-p2:
	gcc ./part2/dbg.c -o dbg2
