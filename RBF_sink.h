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

#include "RBF_buffer.h"

/****************** Optional sink support code starts here ********************************************/

typedef struct RBF_sink_connection_s
{
	buffer_t const*const output;
	void const*const output_values;
	unsigned elem_size;
	volatile rbf_buffer_index_t *const input;
} RBF_sink_connection_t;

#define define_sink(TYPE, NAME)	\
	typedef TYPE NAME##_type;	\
	extern const task_t NAME##_task;	\
	_ROM_table_define_addr(RBF_sink_connection_t, NAME##srcs)

#define declare_sink(TYPE, NAME)	\
	typedef TYPE NAME##_type;	\
	extern const task_t NAME##_task;	\
	_ROM_table_import_addr(RBF_sink_connection_t, NAME##srcs)

#define connect_sink(OUTPUT, SINK)	\
	define_input_buffer(OUTPUT, OUTPUT##_##SINK##_ibuf);	\
	connect(OUTPUT, SINK);	\
	static const RBF_sink_connection_t OUTPUT##_##SINK##_info =	\
	{ &OUTPUT##_properties, &OUTPUT##_values, 	\
		sizeof(OUTPUT##_values[0]), &OUTPUT##_##SINK##_ibuf };	\
	_ROM_table_entry(RBF_sink_connection_t, OUTPUT##_##SINK##_link, SINK##srcs, OUTPUT##_##SINK##_info)

#define LOOP_OVER_SINK(VARIABLE, SINKNAME)	\
	for (VARIABLE=(SINKNAME##_type const*)RBF_sink_read(_ROM_table_addr(SINKNAME##srcs));	\
		VARIABLE;VARIABLE=(SINKNAME##_type const*)RBF_sink_read(_ROM_table_addr(SINKNAME##srcs))) 
#define sink_implementation(NAME, FUNCTION)	\
	_define_task_i3( , NAME, FUNCTION)

void* RBF_sink_read(RBF_sink_connection_t const*const* table);

#define sink_calls(NAME, FUNCTION) TBD
