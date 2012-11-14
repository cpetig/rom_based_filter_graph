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

rbf_buffer_index_t input_buffer_read_impl(rbf_buffer_index_t volatile*read,buffer_t const*props)
{
    int state;
    rbf_buffer_index_t res;
    if (*read==RBF_outbuf_invalid) return RBF_outbuf_invalid;
    state= RBF_enter_critical_section();
    res=*read;
    if (res==props->var->avail_idx)
        *read= RBF_outbuf_invalid;
    else
    {
        ++*read;
        if (*read>=props->max) 
            *read=0;
    }
    RBF_leave_critical_section(state);
    return res;
}

// update empty readers
static void fill_readers(rbf_buffer_index_t value, rbf_buffer_index_t volatile*const* readptrs)
{
    int state= RBF_enter_critical_section();
    for (;*readptrs;++readptrs)
    {
        if (**readptrs==RBF_outbuf_invalid)
            **readptrs=value;
    }
    RBF_leave_critical_section(state);
}

void output_buffer_available_impl(buffer_t const* buf)
{
    // no locking because a single writing thread/source is assumed
    if (buf->var->write_idx>=buf->max) return;
    buf->var->avail_idx=buf->var->write_idx;
    buf->var->write_idx= RBF_outbuf_invalid;
    fill_readers(buf->var->avail_idx, buf->readptrs);
    output_available_impl(buf->listeners);
}

// handle overflow by moving readers
static void move_readers(rbf_buffer_index_t value, rbf_buffer_index_t max, rbf_buffer_index_t volatile*const* readptrs)
{
    int state= RBF_enter_critical_section();
    for (;*readptrs;++readptrs)
    {
        if (**readptrs==value)
        {
            ++**readptrs;
            if (**readptrs==max)
                **readptrs=0;
        }
    }
    RBF_leave_critical_section(state);
}

rbf_buffer_index_t output_buffer_prepare_impl(buffer_t const* buf)
{
    // no locking, circumventing volatile because a single writing thread/source is assumed
    rbf_buffer_index_t write= buf->var->write_idx;
    if (write!=RBF_outbuf_invalid) 
        return write;
    write=buf->var->avail_idx+1;
    if (write>=buf->max)
        write=0;
    buf->var->write_idx= write;
    move_readers(write, buf->max, buf->readptrs);
    return write;
}
