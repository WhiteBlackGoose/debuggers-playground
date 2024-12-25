; specify 64bit for NASM
bits 64

section .data
    hello1 db 'Hello'
    hello_len1 equ $ - hello1
    hello2 db ' world', 0xA
    hello_len2 equ $ - hello2

section .text
    global _start

_start:
    ; Write the string to stdout
    mov rdi, 1

    mov rax, 1
    mov rsi, hello1
    mov rdx, hello_len1
    syscall             ; 0x40101b (make inspect-target-p2)

    mov rax, 1
    mov rsi, hello2
    mov rdx, hello_len2
    syscall           

    ; Exit the program
    mov rax, 60         ; syscall: exit (sys_exit)
    xor rdi, rdi        ; exit code: 0
    syscall
