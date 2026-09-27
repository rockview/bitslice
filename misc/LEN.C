/*
 *	Series 3 Output Bitslice instruction length analyser.
 *
 *	- This program reads a METASTEP microcode object file in Microword TM
 *	format. Each instruction is read from the file and its fields inspected
 *	to determine what its correct length should be. If the instruction has
 *	been programmed with the default length of 3 it is modified to reflect
 *	the field analysis. However, if the length has been explicitly
 *	programmed, i.e. it isn't length 3, it is left as it is. If the computed
 *	length is longer than the explicitly programmed length a warning message
 *	is printed.
 *
 *	Author:	J. A. Hall
 *	Date:	8th May 88
 */

#ifdef	NOTDEF
#include	<stdio.h>
#else
#include	"\zinc\stdio.h"
#endif

/* Field constants */
#define	READ	0x00
#define	WRITE	0x10
#define	RAMQD	0x4
#define	RAMD	0x5
#define	RAMQU	0x6
#define	RAMU	0x7
#define	CxtoC	0x2

#define	MAXLINE	256		/* Maximum input line length */

int		len;		/* Programmed instruction length, b63-61 */
int		main_ldh;	/* Main RAM hi latch load, b60 */
int		main_ldl;	/* Main RAM lo latch load, b59 */
int		main_cycle;	/* Main RAM read/write cycle, b58 */
int		local_cycle;	/* Local RAM read/write cycle, b57 */
int		local_ld;	/* Local RAM latch load, b56 */
int		io;		/* I/O cycle, b53 */
int		rw;		/* Read/write cycle, b52 */
int		cin;		/* Am2904 condition code input, b51-50 */
int		dst;		/* ALU destination, b37-35 */
int		cc_en;		/* Condition code enable, b16 */
int		msr_en;		/* 2904 machine status register enable */

int		cycles;		/* Computed instruction length */
char		in_name[12];	/* Input filename */
FILE		*in_fp;		/* Input file pointer */
char		out_name[12];	/* Output filename */
FILE		*out_fp;	/* Output file pointer */
short		addr;		/* Current microinstruction address */
short		data[8];	/* Current microinstruction data */
int		inst_cnt;	/* Instruction count */
int		total_len;	/* Total of all the instruction lengths */
int		ascii;		/* Flags ASCII output */
int		force10;	/* Flags force all instructions to length 10 */

/*	MAIN
 *
 *	- Main program segment.
 */
main(argc, argv)
int	argc;
char	*argv[];
{
	int	arg;	/* Argument counter */
	char	*p;
	char	c;

	if (argc < 2 || argc > 4)
		usage();

	force10 = ascii = 0;
	for (arg = 1; arg < argc - 1; arg++) {
		p = argv[arg];
		c = *p++;
		if (c == '-')
			while (c = *p++)
				if (c == 'f' || c == 'F')
					force10 = 1;
				else if (c == 'a' || c == 'A')
					ascii = 1;
				else
					usage();
		else
			usage();
	}

	sprintf(in_name, "%s.obj", argv[argc - 1]);
	in_fp = fopen(in_name, "r");
	if (in_fp == NULL) {
		printf("len: can't open input file: %s\n", in_name);
		exit(1);
	}

	sprintf(out_name, "%s.%s%c", argv[argc - 1],
		(force10) ? "10" : "xx",
		(ascii) ? 'a' : 'b');
	out_fp = fopen(out_name, "w");
	if (in_fp == NULL) {
		printf("len: can't open output file: %s\n", out_name);
		exit(1);
	}

	inst_cnt = 0;
	total_len = 0;
	while (1) {
		read_inst();
		extract_fields();
		analyse();
		write_inst();
	}

} /* End of main */

/*	USAGE
 *
 *	- Prints usage instructions and exits.
 */
usage()
{
	puts("usage:	len [-ft] obj");
	puts("where:");
	puts("	-f	forces all instructions to length 10");
	puts("	-a	output in ASCII format (default is binary)");
	puts("	obj	object filename without the .obj extension\n");
	puts("N.B. generates a file called:\n");
	puts("	obj.xxb		variable length binary (default)");
	puts("	obj.xxa		variable length ASCII (-a)");
	puts("	obj.10b		length 10 binary (-f)");
	puts("	obj.10a		length 10 ASCII (-fa)");
	exit(1);

} /* End of usage */

/*	READ_INST
 *
 *	- Reads a microinstruction from the input file.
 */
read_inst()
{
	char		buf[MAXLINE];	/* Input buffer */
	int		datalen;	/* Instruction data bytes */
	unsigned char	sum;		/* Checksum accumulator */
	unsigned char	chksum;		/* Actual checksum */
	int		rectype;	/* Record type */

	if (fgets(buf, MAXLINE, in_fp) == NULL) {
		printf("len: read error on input file: %s\n", in_name);
		exit(1);
	}

	sscanf(buf, ":%2p%4p%2p%2p%2p%2p%2p%2p%2p%2p%2p%2p\n",
		&datalen, &addr, &rectype,
		&data[0], &data[1], &data[2], &data[3],
		&data[4], &data[5], &data[6], &data[7],
		&chksum);
	inst_cnt++;

#ifdef	DEBUG
printf("READ_INST: line %d, :08,%04x,%02x,%02x%02x%02x%02x%02x%02x%02x%02x,%02x\n", inst_cnt, addr, rectype, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], chksum);
#endif

	if (datalen != 0x08 && rectype == 0x01) {
		/* End of file */

		inst_cnt--;

		if (force10)
			printf("-- %d instrs\n", inst_cnt);
		else
			printf("-- %d instrs, av. length %1.1f\n", inst_cnt,
				total_len / (float)inst_cnt);
		if (ascii)
			fputs(":00000001ff\n", out_fp);
		exit(0);
	}

	if (rectype != 0x00) {
		printf("len: format error on line %d\n", inst_cnt);
		exit(1);
	}
		
	/* Compute checksum */
	sum = 0x08;
	sum += (addr >> 8) & 0xff;
	sum += addr & 0xff;
	sum += 0x00;
	sum += data[0];
	sum += data[1];
	sum += data[2];
	sum += data[3];
	sum += data[4];
	sum += data[5];
	sum += data[6];
	sum += data[7];
	sum += chksum;
	if (sum != 0) {
		printf("checksum error on line: %d\n", inst_cnt);
		exit(1);
	}

} /* End of read_inst */

/*	EXTRACT_FIELDS
 *
 *	- Extracts the field values from the instruction.
 */
extract_fields()
{
	if (force10)
		return;

	/* Convert length from Gray code */
	switch ((data[0] & 0xe0) >> 5) {
	case 0:	len = 3;
		break;
	case 1:	len = 4;
		break;
	case 2:	len = 8;
		break;
	case 3:	len = 7;
		break;
	case 4:	len = 10;
		break;
	case 5:	len = 5;
		break;
	case 6:	len = 9;
		break;
	case 7:	len = 6;
		break;
	}
	main_ldh = ! (data[0] & 0x10);
	main_ldl = ! (data[0] & 0x08);
	main_cycle = ! (data[0] & 0x04);
	local_cycle = ! (data[0] & 0x02);
	local_ld = ! (data[0] & 0x01);
	io = ! (data[1] & 0x20);
	rw = data[1] & 0x10;
	cin = (data[1] & 0x0c) >> 2;
	msr_en = ! (data[3] & 0x80);
	dst = (data[3] & 0x38) >> 3;
	cc_en = ! (data[5] & 0x01);

#ifdef	DEBUG
printf("EXTRACT_FIELDS: len = %d, main_ldh = %d, main_ldl = %d, main_cycle = %d\n", len, main_ldh, main_ldl, main_cycle);
printf("EXTRACT_FIELDS: local_cycle = %d, local_load = %d, io = %d, rw = %s\n", local_cycle, local_ld, io, (rw) ? "WRITE" : "READ");
printf("EXTRACT_FIELDS: cin = %d, dst = %d, cc_en = %d, msr_en = %d\n", cin, dst, cc_en, msr_en);
#endif

} /* End of extract_fields */
	
/*	ANALYSE
 *
 *	- Analyse instruction and computes length.
 */
analyse()
{
	int	shift;	/* Flags shift instruction */
	int	addIc;	/* Flags addIc instruction */
	int	latch;	/* Flags ALU output to address latch */

	if (force10) {
		len = 10;
		return;
	}

	if (cc_en)
		/* Conditional jump type */

		cycles = 6;
	else {
		/* Determine ALU operations */

		shift = dst == RAMU || dst == RAMD ||
			dst == RAMQU || dst == RAMQD;
		addIc = cin == CxtoC;
		latch = local_ld || main_ldh || main_ldl;

		if (! shift && ! addIc)
			cycles = 3;
		else if (! shift & addIc)
			cycles = (latch) ? 4 : 3;
		else if (shift & ! addIc)
			cycles = (latch) ? 4 : 3;
		else
			cycles = (latch) ? 5 : 4;
	}

	if (local_cycle || main_cycle || io) {
		/* Input/output type */

		if (rw == READ)
			cycles += (msr_en) ? 4 : 3;	/* Input type */

		if (rw == WRITE || (local_cycle && main_cycle)) {
			/* Output type */

			if (local_cycle) {
				if (! cc_en && ! addIc)
					cycles += 2;
			}
			else if (main_cycle)
				cycles += 3;
			else
				cycles += 4;
		}
	}

	if (len != 3) {
		/* Explicitly programmed instruction length */

		if (cycles > len)
			printf("WARNING: addr %03x, programmed length too small, is %d, should be %d\n", addr, len, cycles);
	}
	else
		len = cycles;

	total_len += len;

#ifdef	DEBUG
printf("ANALYSE: new length is %d\n", len);
#endif

} /* End of analyse */

/*	WRITE_INST
 *
 *	- Writes a microinstruction to the output file.
 */
write_inst()
{
	unsigned char	sum;		/* Checksum accumulator */
	int		nib;		/* Nibble count */
	unsigned char	nib_val;	/* Nibble value */

	/* Convert length to Gray code */
	switch (len) {
	case 3:
		len = 0;
		break;
	case 4:
		len = 1;
		break;
	case 5:
		len = 5;
		break;
	case 6:
		len = 7;
		break;
	case 7:
		len = 3;
		break;
	case 8:
		len = 2;
		break;
	case 9:
		len = 6;
		break;
	case 10:
		len = 4;
		break;
	}
	/* Put in new length */
	data[0] = (len << 5) | (data[0] & 0x1f);

	if (ascii) {

		/* Compute checksum */
		sum = 0x08;
		sum += (addr >> 8) & 0xff;
		sum += addr & 0xff;
		sum += 0x00;
		sum += data[0];
		sum += data[1];
		sum += data[2];
		sum += data[3];
		sum += data[4];
		sum += data[5];
		sum += data[6];
		sum += data[7];

		fprintf(out_fp,
			":08%04x00%02x%02x%02x%02x%02x%02x%02x%02x%02x\n", addr,
			data[0], data[1], data[2], data[3],
			data[4], data[5], data[6], data[7],
			(0x100 - sum) & 0xff);
	}
	else {
		/* Write the address */
		nib_val = (addr >> 8) & 0x00ff;
		fwrite(&nib_val, 1, 1, out_fp);
		nib_val = addr & 0x00ff;
		fwrite(&nib_val, 1, 1, out_fp);

		/* Write the data */
		for (nib = 0; nib < 8; nib++) {
			nib_val = data[nib];
			fwrite(&nib_val, 1, 1, out_fp);
		}
	}

} /* End of write_inst */
