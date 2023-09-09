/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */
#include <inttypes.h>
#include <stdio.h>
#include "system.h"
#include "io.h"

int main()
{
  int i;
  unsigned offset = 0;
  // define number of colors, number of WS2812 and the individual color code
  int num_colors = 14, num_regs = 8;
  int colors[14] = {0xff0000, 0xff0087, 0xff00e7, 0xa800ff,
					0x4500ff, 0x0036ff, 0x008eff, 0x00f9ff,
					0x00ff93, 0x00ff01, 0x9fff00, 0xffff00,
					0xffa700, 0xff6200};

  IOWR_32DIRECT(WS2812_INTERFACE_0_BASE, 0, 1);
  while(1){
	// program the data registers with the color code
	for (i=0;i<num_regs;i++){
		IOWR_32DIRECT(WS2812_INTERFACE_0_BASE, 4 * (i + 1), colors[offset++]);
		offset = (offset + 1) % num_colors;
	}
    // delay for a bit
    for(i=0;i<200000;i++);
  }

  return 0;
}