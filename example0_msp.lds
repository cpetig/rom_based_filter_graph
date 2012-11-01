SECTIONS
{
    .rbf_structs : {
	. = NEXT(2);
	__timers_start = .;
	_timers_start = .;
	*(.timers)
	SHORT(0)

	_program_start_src_tasks_start = .;
	program_start_src_tasks_start = .;
	*(program_start_src_tasks)
	SHORT(0)
	_idle_source_tasks_start = .;
	idle_source_tasks_start = .;
	*(idle_source_tasks)
	SHORT(0)

	_outp1_tasks_start = .;
	outp1_tasks_start = .;
	*(outp1_tasks)
	SHORT(0)
	_timer1_tasks_start = .;
	timer1_tasks_start = .;
	*(timer1_tasks)
	SHORT(0)
    } >rom
}
