/* Linker script for the 68HC11 based DMCU.  */
OUTPUT_FORMAT("elf32-m68hc11", "elf32-m68hc11", "elf32-m68hc11")
OUTPUT_ARCH(m68hc11)
ENTRY(_start)
SEARCH_DIR("/usr/m6811-elf/lib");

/* Fixed definition of the available memory banks. */
MEMORY
{
  page0 (rwx) : ORIGIN = 0x0, LENGTH = 0x100
  eram  (rxw) : ORIGIN = 0x0100, LENGTH = 0x3400-0x100 /* see warning below */
}

/* Setup the stack at the end of eram.  */
PROVIDE (_stack = 0x6000 - 4);

/* DMCU has the io_ports at 0x8000. */
PROVIDE (_io_ports = 0x8000);

/* Warning: Someone at Nintendo/AMD has decided to make nn_cmpt write values
            directly into the middle of ERAM. This can overwrite our code.
            We should find a proper solution to avoid this. */
PROVIDE (_dc_resolutions = 0x3400); /* 0x3400 - 0x340d */
PROVIDE (_dc_op_modes = 0x5800); /* 0x5800 - 0x5802 */

SECTIONS
{
  .hash          : { *(.hash)           }
  .dynsym        : { *(.dynsym)         }
  .dynstr        : { *(.dynstr)         }
  .gnu.version            : { *(.gnu.version) }
  .gnu.version_d          : { *(.gnu.version_d) }
  .gnu.version_r          : { *(.gnu.version_r) }
  .rel.text      :
    {
      *(.rel.text)
      *(.rel.text.*)
      *(.rel.gnu.linkonce.t.*)
    }
  .rela.text     :
    {
      *(.rela.text)
      *(.rela.text.*)
      *(.rela.gnu.linkonce.t.*)
    }
  .rel.data      :
    {
      *(.rel.data)
      *(.rel.data.*)
      *(.rel.gnu.linkonce.d.*)
    }
  .rela.data     :
    {
      *(.rela.data)
      *(.rela.data.*)
      *(.rela.gnu.linkonce.d.*)
    }
  .rel.rodata    :
    {
      *(.rel.rodata)
      *(.rel.rodata.*)
      *(.rel.gnu.linkonce.r.*)
    }
  .rela.rodata   :
    {
      *(.rela.rodata)
      *(.rela.rodata.*)
      *(.rela.gnu.linkonce.r.*)
    }
  .rel.sdata     :
    {
      *(.rel.sdata)
      *(.rel.sdata.*)
      *(.rel.gnu.linkonce.s.*)
    }
  .rela.sdata     :
    {
      *(.rela.sdata)
      *(.rela.sdata.*)
      *(.rela.gnu.linkonce.s.*)
    }
  .rel.sbss      :
    {
      *(.rel.sbss)
      *(.rel.sbss.*)
      *(.rel.gnu.linkonce.sb.*)
    }
  .rela.sbss     :
    {
      *(.rela.sbss)
      *(.rela.sbss.*)
      *(.rel.gnu.linkonce.sb.*)
    }
  .rel.bss       :
    {
      *(.rel.bss)
      *(.rel.bss.*)
      *(.rel.gnu.linkonce.b.*)
    }
  .rela.bss      :
    {
      *(.rela.bss)
      *(.rela.bss.*)
      *(.rela.gnu.linkonce.b.*)
    }
  .rel.stext              : { *(.rel.stest) }
  .rela.stext             : { *(.rela.stest) }
  .rel.etext              : { *(.rel.etest) }
  .rela.etext             : { *(.rela.etest) }
  .rel.sdata              : { *(.rel.sdata) }
  .rela.sdata             : { *(.rela.sdata) }
  .rel.edata              : { *(.rel.edata) }
  .rela.edata             : { *(.rela.edata) }
  .rel.eit_v              : { *(.rel.eit_v) }
  .rela.eit_v             : { *(.rela.eit_v) }
  .rel.ebss               : { *(.rel.ebss) }
  .rela.ebss              : { *(.rela.ebss) }
  .rel.srodata            : { *(.rel.srodata) }
  .rela.srodata           : { *(.rela.srodata) }
  .rel.erodata            : { *(.rel.erodata) }
  .rela.erodata           : { *(.rela.erodata) }
  .rel.got                : { *(.rel.got) }
  .rela.got               : { *(.rela.got) }
  .rel.ctors              : { *(.rel.ctors) }
  .rela.ctors             : { *(.rela.ctors) }
  .rel.dtors              : { *(.rel.dtors) }
  .rela.dtors             : { *(.rela.dtors) }
  .rel.init               : { *(.rel.init) }
  .rela.init              : { *(.rela.init) }
  .rel.fini               : { *(.rel.fini) }
  .rela.fini              : { *(.rela.fini) }
  .rel.plt                : { *(.rel.plt) }
  .rela.plt               : { *(.rela.plt) }
  /* Concatenate .page0 sections.  Put them in the page0 memory bank
     unless we are creating a relocatable file.  */
  .page0 :
  {
    *(.page0)
    *(.softregs)
  }  > page0
  /* DMCU has no ROM, so data is placed in eram. Nintendo placed
     the data section before the text section and provide values
     so no copy to RAM happens when mapping data. */
  PROVIDE (__data_image = .);
  PROVIDE (__data_image_end = __data_image);
  PROVIDE (__data_section_start = 0);
  .data    :
  {
    *(.sdata)
    *(.data)
    *(.data.*)
    *(.data1)
    *(.gnu.linkonce.d.*)
    CONSTRUCTORS
    _edata  =  .;
    PROVIDE (edata = .);
  }  > eram
  /* Start of text section.  */
  .stext   :
  {
    *(.stext)
  }  > eram
  .init   :
  {
    *(.init)
  } =0
  .text  :
  {
    /* Put startup code at beginning so that _start keeps same address.  */
    /* Startup code.  */
    KEEP (*(.install0)) /* Section should setup the stack pointer.  */
    KEEP (*(.install1)) /* Place holder for applications.  */
    KEEP (*(.install2)) /* Optional installation of data sections in RAM.  */
    KEEP (*(.install3)) /* Place holder for applications.  */
    KEEP (*(.install4)) /* Section that calls the main.  */
    *(.init)
    *(.text)
    *(.text.*)
    /* .gnu.warning sections are handled specially by elf32.em.  */
    *(.gnu.warning)
    *(.gnu.linkonce.t.*)
    *(.tramp)
    *(.tramp.*)
    /* Finish code.  */
    KEEP (*(.fini0))    /* Beginning of finish code (_exit symbol).  */
    KEEP (*(.fini1))    /* Place holder for applications.  */
    KEEP (*(.fini2))    /* C++ destructors.  */
    KEEP (*(.fini3))    /* Place holder for applications.  */
    KEEP (*(.fini4))    /* Runtime exit.  */
    _etext = .;
    PROVIDE (etext = .);
  }  > eram
  .eh_frame   :
  {
    KEEP (*(.eh_frame))
  }  > eram
  .gcc_except_table   :
  {
    *(.gcc_except_table)
  }  > eram
  .rodata    :
  {
    *(.rodata)
    *(.rodata.*)
    *(.gnu.linkonce.r*)
  }  > eram
  .rodata1   :
  {
    *(.rodata1)
  }  > eram
  /* Constructor and destructor tables are in ROM.  */
  .ctors   :
  {
     PROVIDE (__CTOR_LIST__ = .);
    KEEP (*(.ctors))
     PROVIDE(__CTOR_END__ = .);
  }  > eram
    .dtors        :
  {
     PROVIDE(__DTOR_LIST__ = .);
    KEEP (*(.dtors))
     PROVIDE(__DTOR_END__ = .);
  }  > eram
  .jcr   :
  {
    KEEP (*(.jcr))
  }  > eram
  .bss   :
  {
    __bss_start = .;
    *(.sbss)
    *(.scommon)
    *(.dynbss)
    *(.bss)
    *(.bss.*)
    *(.gnu.linkonce.b.*)
    *(COMMON)
    PROVIDE (_end = .);
  }  > eram
  __bss_size = SIZEOF(.bss);
  PROVIDE (__bss_size = SIZEOF(.bss));
  /* If the 'vectors_addr' symbol is defined, it indicates the start address
     of interrupt vectors.  This depends on the 68HC11 operating mode:
                        Addr
     Single chip        0xffc0
     Extended mode      0xffc0
     Bootstrap          0x00c0
     Test               0xbfc0
     In general, the vectors address is 0xffc0.  This can be overriden
     with the '-defsym vectors_addr=0xbfc0' ld option.
     Note: for the bootstrap mode, the interrupt vectors are at 0xbfc0 but
     they are redirected to 0x00c0 by the internal PROM.  Application's vectors
     must also consist of jump instructions (see Motorola's manual).  */
  PROVIDE (_vectors_addr = DEFINED (vectors_addr) ? vectors_addr : 0xffc0);
  .vectors DEFINED (vectors_addr) ? vectors_addr : 0xffc0 :
  {
    KEEP (*(.vectors))
  }
  /* Stabs debugging sections.  */
  .stab          0 : { *(.stab) }
  .stabstr       0 : { *(.stabstr) }
  .stab.excl     0 : { *(.stab.excl) }
  .stab.exclstr  0 : { *(.stab.exclstr) }
  .stab.index    0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment       0 : { *(.comment) }
  /* DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to the beginning
     of the section so we begin them at 0.
     Treatment of DWARF debug section must be at end of the linker
     script to avoid problems when there are undefined symbols. It's necessary
     to avoid that the DWARF section is relocated before such undefined
     symbols are found.  */
  /* DWARF 1 */
  .debug         0 : { *(.debug) }
  .line          0 : { *(.line) }
  /* GNU DWARF 1 extensions */
  .debug_srcinfo 0 : { *(.debug_srcinfo) }
  .debug_sfnames 0 : { *(.debug_sfnames) }
  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info) *(.gnu.linkonce.wi.*) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }
}
