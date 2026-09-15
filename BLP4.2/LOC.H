/*
 *	%W%	Series3 BLP
 *
 *	- Local memory variables and LED indication.
 */

external	offset, image_base, death_len, death_1;
external	death_2, death_cnt, death_next, death_curr, free_rec;
external	heap_base, pl_head, pl_curr, line_no, list_addr;
external	request_no, pat_type, curr_pri, reg_save, pat_scans;
external	pat_skip, off_by_one, above_point, below_addr, unsorted_edges;
external	unsorted_ints, next_int, winding, intercepts;

/* LED indication */
BAND_WAIT	equ	b'100001';	/* Waiting for band */
LIST_WAIT	equ	b'100010';	/* Waiting for addition list */
DEATH_WAIT	equ	b'100100';	/* Waiting for death buffer */
CMD_WAIT	equ	b'101000';	/* Waiting for command block */
PIC_WAIT	equ	b'110000';	/* Waiting for picture data */
