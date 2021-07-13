

# NahmanBoot Byte-Stream Language

Please note (!) this is only a draft of the design I'm working on, so this might
change significantly as I keep working on this.

## Terms

"Header" - A structure that its layout defines the characteristics of the "Payload".

"Payload" - A stream of bytes that represent objects.
It can be decoded to instruct the bootloader on how to behave. 
For example, we can instruct the bootloader to load an image from the first detected 
hard-drive at LBA 2049, to offset 0x100000 at physical memory, and to give that image 
the artificial ID of 0x78.
Then, we can define another object in the byte stream that tries to apply settings like
the appropriate command line for that image (but also to link these settings to more object IDs!),
together with defining and loading ramdisks for the image.

"Object" - A sized amount of bytes that represent an entity that the bootloader can
decode and learn how to behave.

## Goals

This language was designed with 3 major goals:
1. Being flexible and extensible as possible. This means this language can be easily
extended to support more features in the future without requiring all of the non-sense
of backwards-compatibility. This means everything is designed and written carefully
to ensure the project vision for future needs.
2. To be lightweight as possible in terms of computer resources. This is somewhat
an obscure goal, because it's a trade-off between CPU time and computer storage.
The project vision is to make a reasonable compromise between the two, so it's indeed
something between the scale of being like AML (ACPI Machine language, which can be
generated dynamically) to SMBIOS data (which is generated statically).
3. Nothing hidden, nor executing on your machine without you know about it.
This means this is not a byte-code language, but rather a language of definitions
of what the bootloader needs to be aware of, and what the user wants it to do when
it executing.

The first 2 goals are really about what this is all about - flexibility and lightweight
as key components of the design.
The last goal is more focused on privacy issues - I simply don't want people to write
malicious payloads of this language, as this potentially creates a bunch of security issues
I don't want to deal with in the project. Also, this is related to the "lightweight" feature
of the design of the language.

Needless to say, nobody should need to write the byte-stream by themselves. For that,
like in ACPI, we need an ASL-like language.

Since the project vision is to allow booting our bootloader in a chain with other bootloaders,
it should be supported to load the payload either as ramdisk (e.g. Multiboot module), or from 
a storage device, together with bootloader image.

## Do you intend to use this language for something else besides this project?

This language is not intended to be used outside the scope of this project.

## What is the project vision of the ASL-like language?

As of writing this document, the decision is to use TOML. Yes, there are lots of good,
self-documenting formats out there. I decided to use TOML just because I deem it to 
be a very simple format (and simplicity is a key component in the project vision).
Anyone who wishes to put time into writing a parser from any other format (e.g. JSON)
to the NahmanBoot Byte-Stream Language is welcomed to do so. The project aims to keep
one format as the defacto-standard to keep things simple.

## Encoding and parsing

To ensure the bootloader doesn't need to load complex strings to the computer's RAM
and decode them (e.g. YAML, TOML or JSON), the whole idea is to represent everything as
simple bytes (essentially serializing a human-readable format to byte stream).
Not using a human readable format as the configuration payload for the bootloader makes
it much more easier to detect errors in runtime, and also to be much more concise and restrictive
about errors that were found during parsing.

All words, double-words and quad-words are encoded as little endian by default
Big endian computers need to be able to decode little endian values and swap the bytes.
For convenience purposes only (in terms of CPU time to decode the values), I reserve
in the header, after the 8-byte signature string, a field of one byte that represents
the endianness of the payload. In the future, when we want to support big endian CPUs,
we can come up with a byte stream generator for this type of CPUs.

## The fundamental objects

Objects as a stream of bytes can be ordered in anyway the byte-stream generator
thinks it's appropriate in the payload.

### The simplest object in the universe (the base header)

This is more an header of an object rather an object. Every object should indicate
its type and length. These fields must have a non-zero value in order to be valid.
It consists of two fields:
```c++
struct header {
    u32 type;
    u32 length;
    u32 object_id;
};
```

The value in length field of the object is from the start of the object, so for example,
an object that has two bytes after the header, the value is 4.

### The Package

This object represents a package of the same objects' type together.
For example, all strings can be packed together to ensure the bootloader will
evaluate these strings after it finished to interpret everything else.

The general rule with packages is in case there are two packages of the same objects' type,
this might be OK, but in case there's a collision between two objects with the same
object ID, this is considered an error.

```c++
struct package {
    header h;
    u32 objects_type;
    u8 nested_stream[];
};
```

### The String

This object represents a textual string of characters. It should not mix
encoding formats of a textual representation of characters (e.g. a UTF-16LE string
should be filled only with UTF-16LE characters, and not UTF-8 characters).

```c++
struct string {
    header h;
    u8 encoding_type;
    u8 chars[];
};
```

# The operative objects

## Load Scattered LBA Range 

This object represents one scatter as LBA range on a storage device.

```c++
struct lba_range {
    header h;
    u8 storage_type;
    u32 storage_sequence_id;
    u64 absolute_start_lba;
    u64 absolute_end_lba;
};
```

## The kernel loading objects

### Load additional module header

This header represents the base header for all "load bootable image from X"
operations. `memory_base_storing_address` field represents the desired physical load 
address in RAM.

```c++
struct bootable_image_header {
    header h;
    u64 memory_base_storing_address;
};

### Load bootable image from RAM

This object represents a bootable image that was preloaded by a previous bootloader in the
chain prior to our execution time (e.g. GRUB loaded our bootloader together with a
multiboot module that represents a bootable image).

```c++
struct bootable_preloaded_image {
    bootable_image_header h;
    u32 preloaded_boot_module_sequence_number;
};
```

### Load bootable image from storage device(s)

This object represents a bootable module that resides on one to N storage devices.

```c++
struct bootable_stored_image {
    bootable_image_header h;
    u32 lba_range_object_ids[];
};
```

### Load bootable image from a filesystem

This object represents a bootable module that resides on one to N storage devices.

```c++
struct bootable_file_image {
    bootable_image_header h;
    u8 storage_type;
    u32 storage_sequence_id;
    u32 parition_number;
    u8 paritition_uuid[16];
    u32 path_string_object_id;
};
```

## The kernel modules' loading objects

### Load additional module header

This header represents the base header for all "load additional module from X"
operations. `memory_base_storing_address` field 

```c++
struct additional_module_header {
    header h;
    u64 memory_base_storing_address;
};

### Load additional module from RAM

This object represents a module that was preloaded by a previous bootloader in the
chain prior to our execution time (e.g. GRUB loaded our bootloader together with a
multiboot module that represents a ramdisk/initramfs image).

```c++
struct additional_preloaded_module {
    additional_module_header h;
    u32 preloaded_boot_module_sequence_number;
};
```

### Load additional module from storage device(s)

This object represents a bootable module that resides on one to N storage devices.

```c++
struct additional_stored_module {
    additional_module_header h;
    u32 lba_range_object_ids[];
};
```

### Load additional module from a filesystem

This object represents a bootable module that resides on one to N storage devices.

```c++
struct additional_file_module {
    additional_module_header h;
    u8 storage_type;
    u32 storage_sequence_id;
    u32 parition_number;
    u8 paritition_uuid[16];
    u32 path_string_object_id;
};
```

## Menu objects

### Define a Bootable Menu Entry

This object represents a bootable menu entry.

```c++
struct bootable_menu_entry {
    header h;
    u32 title_string_object_id;
    u32 bootable_image_object_id;
    u32 command_line_string_object_id;
};
```

### Define a menu

This object represents a menu of entries.

```c++
struct menu {
    header h;
    u32 default_menu_entry_number;
    u32 timeout_in_seconds;
    u32 menu_entries_object_ids[];
};
```
