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

#include "RBF_sink.h"

// read next value from all connected output->input buffers
void* RBF_sink_read(RBF_sink_connection_t const*const* table)
{
	while (!!*table)
	{
		rbf_buffer_index_t idx= input_buffer_read_impl((*table)->input, (*table)->output);
		if (idx!=RBF_outbuf_invalid)
			return ((char*)((*table)->output_values))+idx*(*table)->elem_size;
		++table;
	}
	return 0;
}
