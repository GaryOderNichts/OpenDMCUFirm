/* Interrupt Vectors
   Copyright (C) 2000, 2001, 2002, 2003 Free Software Foundation, Inc.
   Written by Stephane Carrez (stcarrez@nerim.fr)	

This file is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.

In addition to the permissions in the GNU General Public License, the
Free Software Foundation gives you unlimited permission to link the
compiled version of this file with other programs, and to distribute
those programs without any restriction coming from the use of this
file.  (The General Public License restrictions do apply in other
respects; for example, they cover modification of the file, and
distribution when not linked into another program.)

This file is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#ifndef _M68HC11_INTERRUPTS_H
#define _M68HC11_INTERRUPTS_H

/*! @defgroup interrupts Interrupts.

 */
/*@{*/

/*! Interrupt handler prototype.  */
typedef void (* interrupt_t) (void);

/*! Interrupt vectors as a struct.  */
struct interrupt_vectors
{
  interrupt_t res0_handler;
  interrupt_t res1_handler;
  interrupt_t res2_handler;
  interrupt_t res3_handler;
  interrupt_t res4_handler;
  interrupt_t res5_handler;
  interrupt_t res6_handler;
  interrupt_t res7_handler;
  interrupt_t res8_handler;
  interrupt_t res9_handler;
  interrupt_t res10_handler;

  /** SCI interrupt handler.  */
  interrupt_t sci_handler;

  /** SPI interrupt handler.  */
  interrupt_t spi_handler;

  /** Accumulator input handler.  */
  interrupt_t acc_input_handler;

  /** Accumulator overflow interrupt handler.  */
  interrupt_t acc_overflow_handler;

  /** Timer overflow interrupt handler.  */
  interrupt_t timer_overflow_handler;

  /** Output compare 5 interrupt handler.  */
  interrupt_t output5_handler;

  /** Output compare 4 interrupt handler.  */
  interrupt_t output4_handler;

  /** Output compare 3 interrupt handler.  */
  interrupt_t output3_handler;

  /** Output compare 2 interrupt handler.  */
  interrupt_t output2_handler;

  /** Output compare 1 interrupt handler.  */
  interrupt_t output1_handler;

  /** Input capture 3 interrupt handler.  */
  interrupt_t capture3_handler;

  /** Input capture 2 interrupt handler.  */
  interrupt_t capture2_handler;

  /** Input capture 1 interrupt handler.  */
  interrupt_t capture1_handler;

  /** Realtime timer interrupt handler.  */
  interrupt_t rtii_handler;

  /** External interrupt handler.  */
  interrupt_t irq_handler;

  /** Non-maskable interrupt handler.  */
  interrupt_t xirq_handler;

  /** Software interrupt handler.  */
  interrupt_t swi_handler;

  /** Illegal instruction interrupt handler.  */
  interrupt_t illegal_handler;

  /** COP fail interrupt handler.  */
  interrupt_t cop_fail_handler;

  /** COP clock failure interrupt handler.  */
  interrupt_t cop_clock_handler;

  /** Reset handler.  */
  interrupt_t reset_handler;
};
typedef struct interrupt_vectors interrupt_vectors_t;

/*! Interrupt vector id. */
enum interrupt_vector_id
{
  RES0_VECTOR = 0,
  RES1_VECTOR,
  RES2_VECTOR,
  RES3_VECTOR,
  RES4_VECTOR,
  RES5_VECTOR,
  RES6_VECTOR,
  RES7_VECTOR,
  RES8_VECTOR,
  RES9_VECTOR,
  RES10_VECTOR,
  SCI_VECTOR,
  SPI_VECTOR,
  ACC_INPUT_VECTOR,
  ACC_OVERFLOW_VECTOR,
  TIMER_OVERFLOW_VECTOR,
  TIMER_OUTPUT5_VECTOR,
  TIMER_OUTPUT4_VECTOR,
  TIMER_OUTPUT3_VECTOR,
  TIMER_OUTPUT2_VECTOR,
  TIMER_OUTPUT1_VECTOR,
  TIMER_INPUT3_VECTOR,
  TIMER_INPUT2_VECTOR,
  TIMER_INPUT1_VECTOR,
  RTI_VECTOR,
  IRQ_VECTOR,
  XIRQ_VECTOR,
  SWI_VECTOR,
  ILLEGAL_OPCODE_VECTOR,
  COP_FAIL_VECTOR,
  COP_CLOCK_VECTOR,
  RESET_VECTOR,
  MAX_VECTORS
};
typedef enum interrupt_vector_id interrupt_vector_id;

/*! Interrupt vector table.

    The interrupt vector table is in general located at `0xffc0'
    in memory.  It is at the same address as the interrupt
    vectors structure (alias).  */
extern interrupt_t _vectors_addr[MAX_VECTORS];

extern interrupt_vectors_t _vectors __asm__("_vectors_addr");

/*@}*/
#endif
