OUTPUT_ARCH( "riscv" )
ENTRY( _entry )

MEMORY
{
  RAM (rwx) : ORIGIN = 0x80000000, LENGTH = 0x10000
  ROM (rx)  : ORIGIN = 0x04400000, LENGTH = 0x10000
}

SECTIONS
{
  /*
   * ensure that entry.S / _entry is at 0x80000000,
   * where qemu's -kernel jumps.
   */
  . = 0x80000000;


  .text : {
    *(.text)
  } > RAM AT > ROM

  .data : {
    *(.data)
  } > RAM

  .bss : {
    *(.bss)
  } > RAM

}
