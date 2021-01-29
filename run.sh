#make build directory
mkdir -p build

#assemble boot.s file
as -32 boot.S -o build/boot.o

#compile kernel.c
gcc -m32 -c kernel.c -o build/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

#compile askr-utils.c
gcc -m32 -c askr-utils.c -o build/askr-utils.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

#compile askr-char.c
gcc -m32 -c askr-char.c -o build/askr-char.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

#linking the kernel with object files
ld -m elf_i386 -T linker.ld build/kernel.o build/askr-utils.o build/askr-char.o build/boot.o -o build/askr.bin -nostdlib

#check askr.bin if is x86 multiboot file or not
grub-file --is-x86-multiboot build/askr.bin

#build the iso
mkdir -p build/isodir/boot/grub
cp build/askr.bin build/isodir/boot/askr.bin
cp grub.cfg build/isodir/boot/grub/grub.cfg
grub-mkrescue -o build/askr.iso build/isodir

#run in qemu
qemu-system-x86_64 -cdrom build/askr.iso