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

//define_event(idle_source);
//define_event(program_start_src);

#define ADDR_READY 0 /* and any other valid addresses */
#define ADDR_PROCESSED ((task_t*)1)
//#define ADDR_RUNNING 2
//#define ADDR_KILLED 3

static task_t const* ready_list;
//static task_t const* current;

void wake_process(task const* t)
{
	disable_irq();
	if (t->var->next==ADDR_PROCESSED)
	{
		task_t const* last= ready_list;
		if (!last)
		{
			ready_list= t;
			t->var->next=0;
		}
		else
		{
			while (last->var->next) last=last->var->next;
			@@
		}
	}
	enable_irq();
}

void output_available_impl(task_t const*const* listeners)
{
	while (!!*listeners)
	{
		if ((*listeners)->var->next==ADDR_PROCESSED)
			wake_process(*listeners);
		++listeners;
	}
}

// main loop
void schedule()
{
	output_available(program_start_src);
	while (1)
	{
		task_t const* first= ready_list;
		if (!!first)
		{
			disable_irq();
			first= ready_list;
			ready_list= first->var->next;
			first->var->next= ADDR_PROCESSED;
			enable_irq();

			(*(first->fun))(first);
		}
		else
		{
			output_available(idle_source);
		}
	}
}

// timer tick (increment all timers)
void tick_1ms()
{
}

