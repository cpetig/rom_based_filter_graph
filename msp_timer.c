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

#include "RBF.h"
#include <msp430.h>
#include <legacymsp430.h>

extern void tick_1ms();

interrupt(TIMER1_A0_VECTOR) itimer_cb(void)
{
  tick_1ms();
  TA1IV;
}

static void start_msp_timer()
{
  TA1EX0= 0; // no divider
  TA1CTL= (TASSEL_1 | MC_2 | TACLR ); // up mode
  TA1CCR1= 300; // 1ms, value not verified
  TA1CCTL1= 0;
  TA1CCTL0 = 0;
}

define_task(start_timer, start_msp_timer);
connect(program_start_src, start_timer);
