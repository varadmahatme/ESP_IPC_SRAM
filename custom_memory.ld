/* Define a specific region for our shared memory within ESP32's SRAM */
MEMORY
{
  /* This creates a 4K region at a specific address that's safely within ESP32's SRAM */
  SHARED_MEMORY_SEG (RW): ORIGIN = 0x3FFBDB00, LENGTH = 4K
}

SECTIONS
{
  /* Create our custom section at a known address */
  .my_shared_section (NOLOAD) :
  {
    /* Create symbols to mark section boundaries */
    _shared_mem_start = .;
    
    /* Include all objects with this section */
    *(.my_shared_section)
    
    /* Align to 4-byte boundary */
    . = ALIGN(4);
    _shared_mem_end = .;
  } > SHARED_MEMORY_SEG
}

/* Include the existing memory layout - MUST BE AFTER our custom sections */
INCLUDE esp32_out.ld