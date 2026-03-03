BITS 16
ORG 0x8000

stage2_start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x9000

    call enable_a20
    call load_kernel

    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp CODE32_SEL:protected_mode

; Load the kernel image from disk into physical 0x00100000 using INT13h extensions.
load_kernel:
    mov si, dap_packet
    mov byte [si], 0x10
    mov byte [si + 1], 0x00
    mov word [si + 2], KERNEL_SECTORS
    mov word [si + 4], 0x0010      ; offset
    mov word [si + 6], 0xFFFF      ; segment -> 0x00100000 physical
    mov dword [si + 8], KERNEL_LBA_START
    mov dword [si + 12], 0

    mov ah, 0x42
    int 0x13
    jc disk_error
    ret

disk_error:
    mov si, disk_error_msg
    call print_string
.hang:
    hlt
    jmp .hang

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

[BITS 32]
protected_mode:
    mov ax, DATA32_SEL
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x9F000

    ; Build minimal paging structures (identity map first 1GiB using 2MiB pages)
    mov edi, pml4_table
    xor eax, eax
    mov ecx, 4096*3/4
    rep stosd

    mov eax, pdpt_table
    or eax, 0x003
    mov dword [pml4_table], eax
    mov dword [pml4_table + 4], 0

    mov eax, pd_table
    or eax, 0x003
    mov dword [pdpt_table], eax
    mov dword [pdpt_table + 4], 0

    mov ecx, 512
    xor ebx, ebx
.map_pd:
    mov eax, ebx
    shl eax, 21
    or eax, 0x083          ; present + writable + huge(2MiB)
    mov [pd_table + ebx*8], eax
    mov dword [pd_table + ebx*8 + 4], 0
    inc ebx
    loop .map_pd

    mov eax, pml4_table
    mov cr3, eax

    mov eax, cr4
    or eax, 1 << 5         ; PAE
    mov cr4, eax

    mov ecx, 0xC0000080    ; EFER
    rdmsr
    or eax, 1 << 8         ; LME
    wrmsr

    mov eax, cr0
    or eax, (1 << 31) | 1  ; PG + PE
    mov cr0, eax

    jmp CODE64_SEL:long_mode

[BITS 64]
long_mode:
    mov ax, DATA64_SEL
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax

    mov rsp, 0x9F000
    mov rax, KERNEL_ENTRY
    jmp rax

enable_a20:
    in al, 0x92
    or al, 2
    out 0x92, al
    ret

align 8
gdt_start:
    dq 0x0000000000000000
    dq 0x00CF9A000000FFFF ; 32-bit code
    dq 0x00CF92000000FFFF ; 32-bit data
    dq 0x00AF9A000000FFFF ; 64-bit code
    dq 0x00AF92000000FFFF ; 64-bit data
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

dap_packet:
    times 16 db 0

disk_error_msg db 'AetherOS: kernel load failed', 0

CODE32_SEL equ 0x08
DATA32_SEL equ 0x10
CODE64_SEL equ 0x18
DATA64_SEL equ 0x20

KERNEL_ENTRY equ 0x100000
KERNEL_LBA_START equ 65
KERNEL_SECTORS equ 200

align 4096
pml4_table:
    times 512 dq 0
align 4096
pdpt_table:
    times 512 dq 0
align 4096
pd_table:
    times 512 dq 0

TIMES (64*512)-($-$$) db 0
