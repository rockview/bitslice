/*
 *	%W%	Series3 BLP
 *
 *	- Linker directives for the construction of the BLP.
 *
 *	Author:	J A Hall
 *	Date:	20 May 87
 */

name	blp;

/* Modules are loaded in the reverse order! */
load	loc;
load	main;
load	boot;			/* Must be first in blp space */
load	job;
load	comms;
load	misc;
load	math;
load	in_list;
load	addr;
load	phase;
load	pattern;
load	out_band;
load	death;
load	char;
load	rule;
load	shape;
load	scan;
load	line_art;
load	picture;
load	graduate;
load	tile;

/* Dynamic ram */
space	dram;
place	main_abs;

/* Static ram */
space	sram;
place	loc_abs;

/* Instruction control store */
space	blp;
place	boot_rel;			/* Must be first */
place	job_rel;
place	comms_rel;
place	misc_rel;
place	math_rel;
place	in_list_rel;
place	addr_rel;
place	phase_rel;
place	pattern_rel;
place	out_band_rel;
place	death_rel;
place	char_rel;
place	rule_rel;
place	shape_rel;
place	scan_rel;
place	line_art_rel;
place	picture_rel;
place	graduate_rel;
place	tile_rel;
place	boot_abs;

	end;
