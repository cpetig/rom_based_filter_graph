SECTIONS
{
    .timers : {
	__timers_start = .;
	*(.timers)
	LONG(0)
    }
    .rbf_std : {
	_program_start_src_tasks_start = .;
	*(program_start_src_tasks)
	LONG(0)
	_idle_source_tasks_start = .;
	*(idle_source_tasks)
	LONG(0)
    }

    .outp1_tasks : {
	_outp1_tasks_start = .;
	*(outp1_tasks)
	LONG(0)
    }
    .timer1_tasks : {
	_timer1_tasks_start = .;
	*(timer1_tasks)
	LONG(0)
    }

}
