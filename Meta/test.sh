qemu-system-i386 -chardev stdio,id=stdout,mux=on \
                 -device isa-debugcon,chardev=stdout \
                 -kernel ../Build/Core/image
