section .text
bits 64

global hookStub
hookStub:
    push rax
    ret
    int10
