/* m68hc11/ports.h -- Definition of 68HC11 ports
   Copyright 1999, 2000, 2003 Free Software Foundation, Inc.
   Written by Stephane Carrez (stcarrez@nerim.fr)

This file is part of GDB, GAS, and the GNU binutils.

GDB, GAS, and the GNU binutils are free software; you can redistribute
them and/or modify them under the terms of the GNU General Public
License as published by the Free Software Foundation; either version
1, or (at your option) any later version.

GDB, GAS, and the GNU binutils are distributed in the hope that they
will be useful, but WITHOUT ANY WARRANTY; without even the implied
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this file; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#ifndef _M68HC11_PORTS_H
#define _M68HC11_PORTS_H

#include <asm-m68hc11/ports_def.h>

extern inline unsigned short
get_timer_counter (void)
{
  return ((unsigned volatile short*) &_io_ports[M6811_TCNT_H])[0];
}

extern inline void
set_timer_counter (unsigned short value)
{
  ((unsigned volatile short*) &_io_ports[M6811_TCNT_H])[0] = value;
}

extern inline unsigned short
get_input_capture_1 (void)
{
  return ((unsigned volatile short*) &_io_ports[M6811_TIC1_H])[0];
}

extern inline void
set_input_capture_1 (unsigned short value)
{
  ((unsigned volatile short*) &_io_ports[M6811_TIC1_H])[0] = value;
}

extern inline unsigned short
get_input_capture_2 (void)
{
  return ((unsigned volatile short*) &_io_ports[M6811_TIC2_H])[0];
}

extern inline void
set_input_capture_2 (unsigned short value)
{
  ((unsigned volatile short*) &_io_ports[M6811_TIC2_H])[0] = value;
}

extern inline unsigned short
get_input_capture_3 (void)
{
  return ((unsigned volatile short*) &_io_ports[M6811_TIC3_H])[0];
}

extern inline void
set_input_capture_3 (unsigned short value)
{
  ((unsigned volatile short*) &_io_ports[M6811_TIC3_H])[0] = value;
}

/* Get output compare 16-bit register.  */
extern inline unsigned short
get_output_compare_1 (void)
{
  return ((unsigned volatile short*) &_io_ports[M6811_TOC1_H])[0];
}

extern inline void
set_output_compare_1 (unsigned short value)
{
  ((unsigned volatile short*) &_io_ports[M6811_TOC1_H])[0] = value;
}

extern inline unsigned short
get_output_compare_2 (void)
{
  return ((unsigned volatile short*) &_io_ports[M6811_TOC2_H])[0];
}

extern inline void
set_output_compare_2 (unsigned short value)
{
  ((unsigned volatile short*) &_io_ports[M6811_TOC2_H])[0] = value;
}

extern inline unsigned short
get_output_compare_3 (void)
{
  return ((unsigned volatile short*) &_io_ports[M6811_TOC3_H])[0];
}

extern inline void
set_output_compare_3 (unsigned short value)
{
  ((unsigned volatile short*) &_io_ports[M6811_TOC3_H])[0] = value;
}

extern inline unsigned short
get_output_compare_4 (void)
{
  return ((unsigned volatile short*) &_io_ports[M6811_TOC4_H])[0];
}

extern inline void
set_output_compare_4 (unsigned short value)
{
  ((unsigned volatile short*) &_io_ports[M6811_TOC4_H])[0] = value;
}

extern inline unsigned short
get_output_compare_5 (void)
{
  return ((unsigned volatile short*) &_io_ports[M6811_TOC5_H])[0];
}

extern inline void
set_output_compare_5 (unsigned short value)
{
  ((unsigned volatile short*) &_io_ports[M6811_TOC5_H])[0] = value;
}


/* Set the board in the expanded mode to get access to external bus.  */
extern inline void
set_bus_expanded (void)
{
  _io_ports[M6811_HPRIO] |= M6811_MDA;
}


/* Set the board in single chip mode.  */
extern inline void
set_bus_single_chip (void)
{
  _io_ports[M6811_HPRIO] &= ~M6811_MDA;
}

/* Reset the COP.  */
extern inline void
cop_reset (void)
{
  _io_ports[M6811_COPRST] = 0x55;
  _io_ports[M6811_COPRST] = 0xAA;
}

extern inline void
cop_optional_reset (void)
{
#if defined(M6811_USE_COP) && M6811_USE_COP == 1
  cop_reset ();
#endif
}

/* Acknowledge the timer interrupt.  */
extern inline void
timer_acknowledge (void)
{
  _io_ports[M6811_TFLG2] = M6811_RTIF;
}

/* Initialize the timer.  */
extern inline void
timer_initialize_rate (unsigned char divisor)
{
  _io_ports[M6811_TMSK2] = M6811_RTII | divisor;
}

#endif /* _M68HC11_PORTS_H */

