ENTRY(__start)

SECTIONS
{
  . = 0x40000;
  
  
  .head : { *(.head) }
  
  .text : { *(.text) }
  .data : { *(.data) }
  
  .bss : { 
  	__bss_start = .;
  	*(.bss)
  }
 _end = .;
}
