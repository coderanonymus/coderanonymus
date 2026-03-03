ENTRY(_start)

SECTIONS
{
    . = 0x100000;

    .text : {
        *(.text*)
    }

    .rodata : {
        *(.rodata*)
    }

    .data : {
        *(.data*)
    }

    .bss : {
        *(COMMON)
        *(.bss*)
    }

    . = ALIGN(16);
    _kernel_symtab_start = .;
    .symtab : {
        *(.symtab)
    }
    .strtab : {
        *(.strtab)
    }
    _kernel_symtab_end = .;
}
