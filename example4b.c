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

// this example file demonstrates cross file event propagation using declare statements

#include <stdio.h>
#include "RBF_sink.h"

declare_event(timer1);
declare_sink(char,printer);

define_output_buffer(char,outp2,2);
connect_sink(outp2,printer);

static char next = 'N';

static void generator_function(task_t const* t)
{
    unsigned i;
    
    for (i=0;i<3;++i)
    {
		++next;
		if (next>'Z') next='A';
		output_buffer_prepare(outp2)= next;
		output_buffer_available(outp2);
    }
}

define_task(generator2, generator_function);
connect(timer1, generator2);
