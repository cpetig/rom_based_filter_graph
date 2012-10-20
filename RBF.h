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

struct task_s;

// variable part of the task structure
typedef struct task_ram_s
{
	struct task_s* next;
} task_ram_t;

typedef void (*task_fun_ptr_t)(struct task_s*);

typedef struct task_s
{
	task_ram_t *var;
	task_fun_ptr_t fun;	
} task_t;

typedef unsigned timer_counter_t;

typedef struct timer_s
{
	timer_counter_t *counter;
	timer_counter_t interval;
	task_t const*const* listeners; // null terminated list
} timer_t;

#define define_output(TYPE,NAME) \
	typedef TYPE NAME##_type; \
	NAME##_type NAME##_value
#define define_interval_timer(NAME,MILLISECONDS) \
	static unsigned NAME##_value= MILLISECONDS; \
	static const timer_t NAME##_timer = \
	{ &NAME##_value, MILLISECONDS, \
	  NAME##_tasks_start }; \
	static timer_t const*const NAME##_timerentry \
	 __attribute__((section(#NAME "_tasks"))) = NAME##_timer
#define define_timeout(NAME, MILLISECONDS) \
	static unsigned NAME##_value= MILLISECONDS; \
	static const timer_t NAME##_timer = \
	{ &NAME##_value, 0, \
	  NAME##_tasks_start, NAME##_tasks_end-NAME##_tasks_start }; \
	static timer_t const*const NAME##_timerentry \
	 __attribute__((section(#NAME "_tasks"))) = NAME##_timer
#define connect(OUTPUT,TASK) \
	static task_t const*const OUTPUT##_##TASK##_entry \
	 __attribute__((section(#OUTPUT "_tasks"))) = &TASK##_task
#define define_task(NAME, FUNCTION) \
	static task_ram_t NAME##_value; \
	static const task_t NAME##_task = \
	{ &NAME##_value, &FUNCTION }
#define define_sink(NAME, FUNCTION) \
	static task_ram_t NAME##_value; \
	const task_t NAME##_task = \
	{ &NAME##_value, &FUNCTION }

#define output_available(NAME) output_available_impl(NAME##_tasks_start)
#define output_prepare(NAME) NAME##_value
#define output_get(NAME) NAME##_value
#define output_ref(NAME) &NAME##_value
#define output_unref(NAME,PTR) ((void)0)
#define declare_source(TYPE,NAME) \
	typedef TYPE NAME##_type; \
	extern NAME##_type NAME##_value
#define declare_sink(NAME) \
	extern const task_t NAME##_task
#define stop_timer(NAME) \
	NAME##_value= 0
#define restart_timer(NAME, MILLISECONDS) \
	NAME##_value= MILLISECONDS

// TODO: add more arguments for buffered output support
void output_available_impl(task_t const*const* listeners);
// main loop
void schedule();

declare_source(void, idle_source);
declare_source(void, program_start_src);

