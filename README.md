# dasm86

A simple 8086 disassembler and emulator written in C.

This project was built as part of learning low-level programming, instruction decoding, CPU emulation, and binary formats.
It decodes a subset of instructions of the 8086 assembly, and -optionally- emulates them.

## Features
### Disassembler
- Opcode table based decoding
- ModR/M decoding
- Effective address decoding
- Immediate values
- 8-bit and 16-bit operands
- Signed relative jumps
- Instruction printing
### Emulator

The emulator tracks:

- AX, BX, CX, DX
- SP, BP, SI, DI
- IP
- Flags:
-- ZF
-- SF
-- CF
-- OF
  
## Supported Instructions
### MOV

Register ↔ Register
```asm
mov ax, bx
mov al, bl
```
Register ↔ Memory
```asm
mov ax, 1234
mov al, 255
```
Immediate → Memory
```asm
mov [bx], 7
mov [bp+si+5], 4660
```

### ADD | SUB

Register/Memory Arithmetic
```asm
add ax, bx
add al, bl
add [bx], 5
```
Immediate Arithmetic
```asm
add ax, 1000
add al, 5
```

### CMP

```asm
cmp ax, bx
cmp al, 5
cmp [bx], 255
```


### JNZ

Supports signed 8-bit relative jumps
```asm
jnz label
jnz -8
```
## How to Build
Just use
```bash
make
```
## Usage

Disassemble a binary
```bash
./dasm86 <binfile>
```

Run a binary
```bash
./dasm86 -run <binfile>
```

dump emulator memory to a file
```bash
./dasm86 -run -write <binfile>
```
```-write``` flag creates mem.dasm, containing the emulator memory after execution.


## Example

This program writes values to memory that can be interpreted as a 64x64 RGBA image:

```dasm
bits 16

mov bp, 0 ; starting point
mov cx, 0
y_loop:

  mov dx, 0
  x_loop:

  mov word [bp], dx     ; Red
  mov word [bp + 2], cx ; Blue
  mov byte [bp + 3], 255 ; Alpha

  add bp, 4

  add dx, 1
  cmp dx, 64
  jnz x_loop

add cx, 1
cmp cx, 64
jnz y_loop
```

After running the assembled binary with the emulator and dumping the emulated memory with ```-write```,
the resulting file can be opened as a raw RGBA image in GIMP, producing:


<img width="512" style="image-rendering: pixelated;" alt="img64" src="https://github.com/user-attachments/assets/b4707120-4fbd-460f-811f-2d1aa3ad4ac1" />


