; specify 64bit for NASM
bits 64

section .data
    hello db 'Hello, World!', 0xA   ; String with a newline character
    hello_len equ $ - hello         ; Calculate the length of the string

section .text
    global _start

_start:
    ; Write the string to stdout
    mov rax, 1          ; syscall: write (sys_write)
    mov rdi, 1          ; file descriptor: stdout
    mov rsi, hello      ; pointer to string
    mov rdx, hello_len  ; length of string
    syscall           

    ; Exit the program
    mov rax, 60         ; syscall: exit (sys_exit)
    xor rdi, rdi        ; exit code: 0
    syscall
