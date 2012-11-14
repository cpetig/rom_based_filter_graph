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
#include "RBF_buffer.h"

define_output_buffer(char,outp1,4);
define_interval_timer(timer1,33);

static char next = '@';

void generator_function(task_t const* t)
{
    unsigned i;
    
    for (i=0;i<5;++i)
    {
	++next;
	if (next>'Z') next='A';
	output_buffer_prepare(outp1)= next;
	output_buffer_available(outp1);
    }
}

define_task(generator, generator_function);
connect(timer1, generator);

define_input_buffer(outp1, printer_buf);

void printer_function(task_t const* t)
{
	rbf_buffer_index_t idx;
	LOOP_OVER_BUFFER(idx, printer_buf)
	{
		putchar(buffer_get(outp1, idx));
		putchar(' ');
	}
	putchar('\n');
}

define_task(printer, printer_function);
connect(outp1, printer);

