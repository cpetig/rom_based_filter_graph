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

/****************** Optional event buffering code starts here ********************************************/

typedef unsigned char rbf_buffer_index_t;

// these structures keep the number of arguments for prepare+available down
typedef struct buffer_ram_s
{
	rbf_buffer_index_t avail_idx;
	rbf_buffer_index_t write_idx;
} buffer_ram_t;

typedef struct buffer_s
{
	buffer_ram_t volatile*const var;
	rbf_buffer_index_t volatile*const*const readptrs;
	task_t const*const*const listeners;
	const rbf_buffer_index_t max;
} buffer_t;

#define RBF_outbuf_invalid ((rbf_buffer_index_t)-1)
#define define_output_buffer(TYPE,NAME,SIZE) \
		_ROM_table_define_addr(task_t,NAME##_tasks); \
		_ROM_table_define_addr(volatile rbf_buffer_index_t,NAME##_readptrs); \
		typedef TYPE NAME##_type; \
		NAME##_type NAME##_values[SIZE]; \
		static buffer_ram_t NAME##_usage = \
		{ RBF_outbuf_invalid,RBF_outbuf_invalid }; \
		const buffer_t NAME##_properties = \
		{ &NAME##_usage, (volatile rbf_buffer_index_t*const*)_ROM_table_addr(NAME##_readptrs), _ROM_table_addr(NAME##_tasks), SIZE }

#define output_buffer_available(NAME) \
	output_buffer_available_impl(&NAME##_properties)
#define output_buffer_prepare(NAME) \
	(NAME##_values[output_buffer_prepare_impl(&NAME##_properties)])

void output_buffer_available_impl(buffer_t const* buf);
rbf_buffer_index_t output_buffer_prepare_impl(buffer_t const* buf);

#define declare_source_buffer(TYPE, NAME) \
		_ROM_table_import_addr(task_t,NAME##_tasks); \
		typedef TYPE NAME##_type; \
		extern const buffer_t NAME##_properties; \
		extern NAME##_type NAME##_values[]
		
#define define_input_buffer(SRCNAME,BUFFER) \
		static volatile rbf_buffer_index_t BUFFER = RBF_outbuf_invalid; \
		_ROM_table_entry(volatile rbf_buffer_index_t, SRCNAME##_##TASK##_buf, SRCNAME##_readptrs, BUFFER); \
		static buffer_t const*const BUFFER##_properties = &SRCNAME##_properties
		
// returns RBF_outbuf_invalid when empty (careful: contents of value might change during buffer overflow)
rbf_buffer_index_t input_buffer_read_impl(rbf_buffer_index_t volatile*read,buffer_t const*props);

#define LOOP_OVER_BUFFER(IDX, BUF) \
    	for (IDX= buffer_eval(BUF); IDX!=RBF_outbuf_invalid; IDX=buffer_eval(BUF))

#define buffer_get(SRCNAME,IDX) \
	(SRCNAME##_values[IDX])
#define buffer_eval(BUF) \
	input_buffer_read_impl(&BUF,BUF##_properties)

#define buffer_getlast(SRCNAME) \
	(SRCNAME##_values[SRCNAME##_properties.var->avail_idx])

