#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void main() 
{
    uint8_t* vga_low_accesor = (uint8_t*)0xb8000;
    for (uint16_t index = 0; index < 80 * 25 * 2; index++) {
        vga_low_accesor[index] = 0;
    }
}
