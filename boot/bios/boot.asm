BITS 16
ORG 0x7C00

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    mov [boot_drive], dl

    mov si, boot_msg
    call print_string

    mov bx, 0x8000             ; stage2 load address
    mov dh, STAGE2_SECTORS
    mov dl, [boot_drive]
    call disk_load

    jmp 0x0000:0x8000

hang:
    hlt
    jmp hang

print_char:
    mov ah, 0x0E
    int 0x10
    ret

print_string:
.next:
    lodsb
    test al, al
    jz .done
    call print_char
    jmp .next
.done:
    ret

; Load DH sectors from CHS(0,0,2) into ES:BX
; Very small loader for QEMU default floppy geometry assumptions.
disk_load:
    push ax
    push bx
    push cx
    push dx

    mov ah, 0x02
    mov al, dh
    mov ch, 0x00
    mov dh, 0x00
    mov cl, 0x02
    int 0x13
    jc disk_error

    pop dx
    pop cx
    pop bx
    pop ax
    ret

disk_error:
    mov si, disk_err_msg
    call print_string
    jmp hang

boot_drive db 0
boot_msg db 'AetherOS stage1', 0

disk_err_msg db ' Disk read error', 0

STAGE2_SECTORS equ 64

times 510-($-$$) db 0
dw 0xAA55
