/*
 *	%W%	Series3 BLP
 *
 *	- Main memory variables and constants.
 */

external	poll_job, poll_list, poll_death;
external	intr_sema, depth_sema;

/* Interrupt types */
I_NEW_LIST	equ	0;	/* Request new addition list */
I_DEATH_LIST_1	equ	1;	/* Death list in death buffer 1 */
I_DEATH_LIST_2	equ	2;	/* Death list in death buffer 2 */
I_END_OF_JOB	equ	3;	/* End-of-job */
I_WARNING	equ	4;	/* Warning error condition */
I_FATAL		equ	5;	/* Fatal error condition */
I_PICTURE_UPDATE equ	6;	/* Request more picture data */
I_PICTURE_DEATH	equ	7;	/* Inform of picture death */
I_TILE_DEATH	equ	8;	/* Inform of tile death */

/* Warning errors */
W_BAD_ADDN_TYPE	equ	1;	/* Bad addition record type */
W_BAD_EDGE_TYPE	equ	2;	/* Bad edge type */
W_CANT_PAT	equ	3;	/* Can't pattern bitmap or halftone */
W_CANT_TINT	equ	4;	/* Can't tint image */

/* Fatal errors */
F_LIST_OVFL	equ	1;	/* Image list overflow */
F_NOT_IMPL	equ	2;	/* Function not implemented */
F_LOU_INIT	equ	3;	/* Can't find busy band when initialising LOU */

POLL_DELAY	equ	30;	/* Polling delay.  No. of length 10 instrs */
