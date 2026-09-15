/*
 *	%W%	Series3 BLP
 *
 *	- OBB register definitions.
 */

/* Run decoder I/O addresses and bit masks */
OBB_LINE_ADDR	equ	h'80';		/* Line address within band */
OBB_RUN_ADDR	equ	h'24';		/* Start address */
OBB_RUN_MODE	equ	h'28';		/* Combination mode/buffer selection */
OBB_RUN_DATA	equ	h'2c';		/* Data */

OBB_LINE	equ	h'0100';	/* Line address load/increment */
OBB_BAND	equ	h'0200';	/* Band increment */
OBB_CLEAR_MODE	equ	h'0024';	/* Clear buffer mode */

/* Output mode I/O address and bit masks */
OBB_OUT_MODE	equ	h'38';

OBB_RESET	equ	h'0001';	/* Band address reset */
OBB_NORMAL	equ	h'0002';	/* Normal mode (positive/forward) */

/* Pattern decoder I/O addresses and bit mask */
OBB_INTL_RUN	equ	h'40';		/* Initial run */
OBB_PAT_RUN	equ	h'44';		/* Run lengths */

OBB_COLOUR	equ	h'0200';	/* Initial run colour */

/* Bitmap decoder I/O addresses */
OBB_BIT_CON	equ	h'48';		/* Control */
OBB_BIT_DATA	equ	h'4c';		/* Data */

/* Halftone decoder I/O addresses and bit masks */
OBB_TONE_CON	equ	h'50';		/* Control */
OBB_TONE_DATA	equ	h'54';		/* Data */

OBB_ODD_CELL	equ	h'0010';	/* Selects odd cell */
OBB_TINT	equ	h'0020';	/* Selects tint cell table */
OBB_TONE_MAGIC	equ	h'0200';	/* Delay count */
OBB_PHASE_MASK	equ	h'000f';	/* Phase mask */

/* Status I/O address and bit masks */
OBB_STATUS	equ	h'58';		/* Status */

OBB_RUN_BSY	equ	h'0001';	/* Flags run busy (active low) */
OBB_PIC_RDY	equ	h'0002';	/* Flags picture ready (active high) */
OBB_BAND_BSY	equ	h'0004';	/* Flags band busy (active high) */
OBB_LOU_WAIT	equ	h'0008';	/* Flags LOU waiting (active high) */

/* Miscellaneous */
OBB_BAND_CNT	equ	14;		/* Number of bands */
OBB_LINE_CNT	equ	16;		/* Number of lines per band */
