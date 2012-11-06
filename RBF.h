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
	struct task_s const* next;
} task_ram_t;

// initialize next to this value (0 means last of the ready list)
#define ADDR_PROCESSED ((task_t*)1)
#define TASK_RAM_T_INIT_VALUE { ADDR_PROCESSED }

typedef void (*task_fun_ptr_t)(struct task_s const*);

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
} RBF_timer_t;

// internal macros
#if 0 // old style: manual linker script
#define _ROM_table_import_addr(CTYPE,SECTION) \
	extern CTYPE const*const SECTION##_start[]
#define _ROM_table_addr(SECTION) \
	SECTION##_start
#define _ROM_table_entry(NAME,CATEGORY,CTYPE,ENTRYNAME,SECTION) \
	static CTYPE const*const ENTRYNAME##entry \
	 __attribute__((section(SECTION),used)) = &NAME##_##CATEGORY
#else
#define _ROM_table_define_addr(CTYPE,SECTION) \
	CTYPE const*const RBF_##SECTION##_start[1] \
	 __attribute__((section(".text$RBF_"#SECTION"0"),used)) = { 0 }
#define _ROM_table_import_addr(CTYPE,SECTION) \
	extern CTYPE const*const RBF_##SECTION##_start[]
#define _ROM_table_addr(SECTION) \
	(RBF_##SECTION##_start+1)
#define _ROM_table_entry(NAME,CATEGORY,CTYPE,ENTRYNAME,SECTION) \
	static CTYPE const*const ENTRYNAME##entry \
	 __attribute__((section(".text$RBF_"#SECTION"1"),used)) = &NAME##_##CATEGORY
#endif

#define _define_RBF_timer(NAME,MILLISECONDS,RELOAD) \
	_ROM_table_define_addr(task_t,NAME##_tasks); \
	static timer_counter_t NAME##_value= MILLISECONDS; \
	static const RBF_timer_t NAME##_timer = \
	{ &NAME##_value, RELOAD, \
	  _ROM_table_addr(NAME##_tasks) }; \
	_ROM_table_entry(NAME,timer,RBF_timer_t,NAME##_timer,timers)

#define _define_task4(STORAGE, NAME, FUNCTION, RAM) \
	STORAGE const task_t NAME##_task = \
	{ (task_ram_t*)&RAM, &FUNCTION }
#define _define_task_i3(STORAGE, NAME, FUNCTION) \
	static task_ram_t NAME##_value = TASK_RAM_T_INIT_VALUE; \
	_define_task4(STORAGE, NAME, FUNCTION, NAME##_value)

// public macros
#define define_output(TYPE,NAME) \
		_ROM_table_define_addr(task_t,NAME##_tasks); \
		typedef TYPE NAME##_type; \
		NAME##_type NAME##_value
#define define_event(NAME) /* void type output */ \
		_ROM_table_define_addr(task_t,NAME##_tasks)
#define define_interval_timer(NAME,MILLISECONDS) \
		_define_RBF_timer(NAME,MILLISECONDS,MILLISECONDS)
#define define_timeout(NAME, MILLISECONDS) \
		_define_RBF_timer(NAME,MILLISECONDS,0)

#define connect(OUTPUT,TASK) \
		_ROM_table_entry(TASK,task,task_t,OUTPUT##_##TASK,OUTPUT##_tasks)

#define define_task3(NAME, FUNCTION, RAM) \
		_define_task4(static, NAME, FUNCTION, RAM)
#define define_task(NAME, FUNCTION) \
		_define_task_i3(static, NAME, FUNCTION)
// a sink is a globally accessible task
#define define_sink(NAME, FUNCTION) \
		_define_task_i3(, NAME, FUNCTION)

#define output_available(NAME) output_available_impl(_ROM_table_addr(NAME##_tasks))
#define output_prepare(NAME) NAME##_value
#define output_get(NAME) NAME##_value
#define output_ref(NAME) &NAME##_value
#define output_unref(NAME,PTR) ((void)0)

#define declare_source(TYPE,NAME) \
		_ROM_table_import_addr(task_t,NAME##_tasks); \
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
// timer tick
void tick_1ms();

declare_source(void, idle_source);
declare_source(void, program_start_src);

