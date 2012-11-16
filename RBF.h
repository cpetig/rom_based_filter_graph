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
	task_ram_t *const var;
	const task_fun_ptr_t fun;	
} task_t;

typedef unsigned timer_counter_t;

typedef struct timer_s
{
	timer_counter_t *const counter;
	const timer_counter_t interval;
	task_t const*const*const listeners; // null terminated list
} RBF_timer_t;

/****************** Internal macros start here ********************************************/
#if defined (__GNUC__) /*|| __TI_GNU_ATTRIBUTE_SUPPORT__*/
# define RBF_SEC_PREFIX ".text$RBF"
# define _ROM_table_define_addr(CTYPE,SECTION) \
	CTYPE const*const RBF_##SECTION##_start[1] \
	 __attribute__((section(RBF_SEC_PREFIX"_"#SECTION"0"),used)) = { 0 }
# define _ROM_table_import_addr(CTYPE,SECTION) \
	extern CTYPE const*const RBF_##SECTION##_start[]
# define _ROM_table_addr(SECTION) \
	(RBF_##SECTION##_start+1)
# define _ROM_table_entry(CTYPE,ENTRYPREFIX,SECTION,SYMBOL) \
	static CTYPE const*const ENTRYPREFIX##entry \
	 __attribute__((section(RBF_SEC_PREFIX"_"#SECTION"1"),used)) = &SYMBOL
# define _ROM_table_end() \
        static void const*const RBF_end_entry \
	__attribute__((section(RBF_SEC_PREFIX"z"),used)) = 0
#elif defined(__TI_COMPILER_VERSION__)
# define RBF_SEC_PREFIX ".text:RBF"
// I found no other way to mark a symbol as used
# define _USE_SYMBOL(CTYPE,ENTRYPREFIX,SYMBOL)	\
	CTYPE const*const ENTRYPREFIX##entry = &SYMBOL
# ifdef __eabi__
#  define _SYMBOL_PREFIX
# else
#  define _SYMBOL_PREFIX "_"
# endif
# define _ROM_table_define_addr(CTYPE,SECTION) \
	asm(" .global "_SYMBOL_PREFIX "RBF_"#SECTION"_start\n"	\
		" .sect \""RBF_SEC_PREFIX"_"#SECTION"0\"\n"	\
		_SYMBOL_PREFIX "RBF_"#SECTION"_start:\n"	\
		" .word 0\n");	\
	extern CTYPE const*const RBF_##SECTION##_start[]
# define _ROM_table_import_addr(CTYPE,SECTION) \
	extern CTYPE const*const RBF_##SECTION##_start[]
# define _ROM_table_addr(SECTION) \
	(RBF_##SECTION##_start+1)
# define _ROM_table_entry(CTYPE,ENTRYPREFIX,SECTION,SYMBOL) \
	asm(" .sect \""RBF_SEC_PREFIX"_"#SECTION"1\"\n"	\
		" .word "_SYMBOL_PREFIX #SYMBOL"\n");	\
	_USE_SYMBOL(CTYPE,ENTRYPREFIX,SYMBOL)
# define _ROM_table_end() \
	asm(" .sect \""RBF_SEC_PREFIX"z\"\n"	\
		" .word 0\n")
#endif

#define _define_RBF_timer(NAME,MILLISECONDS,RELOAD) \
	_ROM_table_define_addr(task_t,NAME##_tasks); \
	static timer_counter_t NAME##_value= MILLISECONDS; \
	static const RBF_timer_t NAME##_timer = \
	{ &NAME##_value, RELOAD, \
	  _ROM_table_addr(NAME##_tasks) }; \
	_ROM_table_entry(RBF_timer_t,NAME##_timer,timers,NAME##_##timer)

#define _define_task4(STORAGE, NAME, FUNCTION, RAM) \
	STORAGE const task_t NAME##_task = \
	{ (task_ram_t*)&RAM, &FUNCTION }
#define _define_task_i3(STORAGE, NAME, FUNCTION) \
	static task_ram_t NAME##_value = TASK_RAM_T_INIT_VALUE; \
	_define_task4(STORAGE, NAME, FUNCTION, NAME##_value)

void output_available_impl(task_t const*const* listeners);
// timer tick (should be called with 1ms periodicy)
void tick_1ms();

/****************** Public interface starts here ********************************************/
#define define_event(NAME) /* void type output */ \
		_ROM_table_define_addr(task_t,NAME##_tasks)
#define define_output(TYPE,NAME) \
		define_event(NAME); \
		typedef TYPE NAME##_type; \
		NAME##_type NAME##_value
#define define_interval_timer(NAME,MILLISECONDS) \
		_define_RBF_timer(NAME,MILLISECONDS,MILLISECONDS)
#define define_timeout(NAME, MILLISECONDS) \
		_define_RBF_timer(NAME,MILLISECONDS,0)

#define connect(OUTPUT,TASK) \
		_ROM_table_entry(task_t,OUTPUT##_##TASK,OUTPUT##_tasks,TASK##_##task)

#define define_task3(NAME, FUNCTION, RAM) \
		_define_task4(static, NAME, FUNCTION, RAM)
#define define_task(NAME, FUNCTION) \
		_define_task_i3(static, NAME, FUNCTION)

#define output_prepare(NAME) NAME##_value
#define output_available(NAME) output_available_impl(_ROM_table_addr(NAME##_tasks))
#define output_get(NAME) NAME##_value

#define declare_event(NAME) \
		_ROM_table_import_addr(task_t,NAME##_tasks)
#define declare_source(TYPE,NAME) \
		declare_event(NAME); \
		typedef TYPE NAME##_type; \
		extern NAME##_type NAME##_value
//#define declare_sink(NAME) \
//		extern const task_t NAME##_task
#define stop_timer(NAME) \
		NAME##_value= 0
#define restart_timer(NAME, MILLISECONDS) \
		NAME##_value= MILLISECONDS

int RBF_enter_critical_section();
void RBF_leave_critical_section(int); // pass the value received by enter

declare_event(idle_source);
declare_event(program_start_src);

