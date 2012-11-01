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
#include <signal.h>
#include <string.h>
#include <sys/time.h>

extern void tick_1ms();

static void itimer_cb(int num)
{
  tick_1ms();
}

static void start_linux_timer()
{
  struct sigaction sa;
  memset(&sa,0,sizeof sa);
  sa.sa_handler=&itimer_cb;
  sigaction(SIGALRM, &sa, 0);   
  struct itimerval itv;
  memset(&itv,0,sizeof itv);
  itv.it_interval.tv_usec=1000;
  itv.it_value.tv_usec=1000;   
  setitimer(ITIMER_REAL, &itv, 0);
}

define_task(start_timer, start_linux_timer);
connect(program_start_src, start_timer);
