SECTIONS
{
    .rbf_structs : {
	*(SORT(.text$RBF*))
    } >rom
}
