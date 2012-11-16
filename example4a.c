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

#include <stdio.h>
#include "RBF_sink.h"

define_output_buffer(char,outp1,2);
define_interval_timer(timer1,32);

static char next = '@';

static void generator_function(task_t const* t)
{
    unsigned i;
    
    for (i=0;i<2;++i)
    {
		++next;
		if (next>'Z') next='A';
		output_buffer_prepare(outp1)= next;
		output_buffer_available(outp1);
    }
}

define_task(generator, generator_function);
connect(timer1, generator);

define_sink(char, printer);
connect_sink(outp1, printer);

#if 0 // is this worth the trouble?
void printer_function(char const* value)
{
	putchar(*value);
	putchar('\n');
}

sink_calls(printer, printer_function);

#else
void printer_function(task_t const* t)
{
	char const* value;
	LOOP_OVER_SINK(value, printer)
	{
		putchar(*value);
		putchar(' ');
	}
	putchar('\n');
}

sink_implementation(printer, printer_function);
#endif
