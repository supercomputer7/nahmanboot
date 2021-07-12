/*
 * Copyright (c) 2021, Liav A. <liavalb@hotmail.co.il>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/StringView.h>
#include <AK/Types.h>

namespace Multiboot {

enum MemoryEntryType {
    Available = 1,
    Reserved = 2,
    ACPIReclaimable = 3,
    ACPINVS = 4,
    FaultyRAM = 5,
};

StringView parse_entry_type(MemoryEntryType);

struct [[gnu::packed]] MemoryMapEntry {
    u32 size;
    u32 addr_low;
    u32 addr_high;
    u32 len_low;
    u32 len_high;
    u32 type;
};

struct aout_symbol_table {
    u32 tabsize;
    u32 strsize;
    u32 addr;
    u32 reserved;
};
typedef struct aout_symbol_table aout_symbol_table_t;

struct elf_section_header_table {
    u32 num;
    u32 size;
    u32 addr;
    u32 shndx;
};
typedef struct elf_section_header_table elf_section_header_table_t;

enum FramebufferType {
    INDEXED = 0,
    RGB = 1,
    TEXT = 2,
};

struct ProvidedInfo {
    // Multiboot info version number.
    u32 flags;

    // Available memory from BIOS.
    u32 mem_lower;
    u32 mem_upper;

    // "root" partition.
    u32 boot_device;

    // Kernel command line.
    u32 cmdline;

    // Boot-Module list.
    u32 mods_count;
    u32 mods_addr;

    union {
        aout_symbol_table_t aout_sym;
        elf_section_header_table_t elf_sec;
    } u;

    // Memory Mapping buffer.
    u32 mmap_length;
    u32 mmap_addr;

    // Drive Info buffer.
    u32 drives_length; // Not being used
    u32 drives_addr;   // Not being used

    // ROM configuration table.
    u32 config_table; // Not being used

    // Boot Loader Name.
    u32 boot_loader_name;

    // APM table.
    u32 apm_table;

    // Video.
    u32 vbe_control_info;
    u32 vbe_mode_info;
    u16 vbe_mode;
    u16 vbe_interface_seg;
    u16 vbe_interface_off;
    u16 vbe_interface_len;

    u64 framebuffer_addr;
    u32 framebuffer_pitch;
    u32 framebuffer_width;
    u32 framebuffer_height;
    u8 framebuffer_bpp;
    u8 framebuffer_type;
    union {
        struct
        {
            u32 framebuffer_palette_addr;
            u16 framebuffer_palette_num_colors;
        };
        struct
        {
            u8 framebuffer_red_field_position;
            u8 framebuffer_red_mask_size;
            u8 framebuffer_green_field_position;
            u8 framebuffer_green_mask_size;
            u8 framebuffer_blue_field_position;
            u8 framebuffer_blue_mask_size;
        };
    };
};

}
