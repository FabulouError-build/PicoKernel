/* 内核链接脚本 */

OUTPUT_FORMAT("binary")
ENTRY(_start)

SECTIONS {
    . = 0x10000; /* 内核加载地址 */

    .text : {
        *(.text)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
    }

    /DISCARD/ : {
        *(.comment)
        *(.eh_frame)
        *(.reloc)
        *(.note*)
        *(.debug*)
    }
}