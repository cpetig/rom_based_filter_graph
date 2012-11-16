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
#include "RBF.h"

define_output(char,outp1);
define_interval_timer(timer1,61);
define_interval_timer(timer2,30);

typedef struct mytask_s
{
	task_ram_t task;
	char next;
} mytask_t;

static mytask_t generator_info = { TASK_RAM_T_INIT_VALUE, '@' };
static mytask_t generator_info2 = { TASK_RAM_T_INIT_VALUE, 'N' };

void generator_function(task_t const* t)
{
	char* i= &((mytask_t*)(t->var))->next;
	++*i;
	if (*i>'Z') *i='A';
	output_prepare(outp1)= *i;
	output_available(outp1);
}

define_task3(generator, generator_function, generator_info);
connect(timer1, generator);
define_task3(generator2, generator_function, generator_info2);
connect(timer2, generator2);

void printer_function(task_t const* t)
{
	printf("%c\n", output_get(outp1));
}

define_task(printer, printer_function);
connect(outp1, printer);

