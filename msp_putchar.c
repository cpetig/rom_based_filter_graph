/*  Rom based filter graph
    Copyright (C) 2012  Christof Petig

    This program is free software; you can redistribute it and/or modify
    it under the terms of either the the Apache License 2.0 or the 
    GNU General Public License as published by the Free Software Foundation;
    either version 2 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License or Apache License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

// output character to serial device

#include "RBF.h"
#include "msp430x552x.h"
#include <signal.h>

int putchar(int x)
{
  UCA1TXBUF=x; // next_byte_into_tx_reg
  while ((UCA1IFG & UCTXIFG)==0); // wait_transmitter_empty
  return x&0xff;
}

static void start_msp_serial()
{
  P4SEL |= (BIT4 | BIT5);
  UCA1CTL1= UCSSEL_2 | UCSWRST;
  UCA1CTL0= UCMODE0;
  UCA1BRW= 138;// 69; // Table 26-4
  UCA1MCTL= UCBRS_7;
  UCA1IE= 0;
  UCA1IE |= UCRXIE;
  UCA1CTL1 &= ~UCSWRST;
}

define_task(start_serial, start_msp_serial);
connect(program_start_src, start_serial);
