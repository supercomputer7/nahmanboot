qemu-system-i386 -chardev stdio,id=stdout,mux=on \
                 -m 512M \
                 -device isa-debugcon,chardev=stdout \
                 -kernel ../Build/Core/image \
                 -append "hello"
