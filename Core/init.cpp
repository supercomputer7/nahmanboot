#include <AK/Types.h>
#include <AK/Array.h>

extern u32 __stack_chk_guard;
u32 __stack_chk_guard;

void main() 
{
    __stack_chk_guard = 0xBADBADBA;
    Array<u8, 40> new_array;
    u8* vga_low_accesor = (u8*)0xb8000;
    for (size_t index = 0; index < 80 * 25 * 2; index++) {
        vga_low_accesor[index] = 0;
    }
    new_array[0] = 1;
}
