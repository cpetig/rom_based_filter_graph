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

int RBF_enter_critical_section()
{
#ifdef __16bis__
	/* TODO: some assembly tricks */
	return 0;
#else
	return _disable_interrupts();
#endif
}

void RBF_leave_critical_section(int val)
{
#ifdef __16bis__
	/* TODO: some assembly tricks */
#else
	_set_CPSR(val);
//    _restore_interupts(val);
#endif
}
