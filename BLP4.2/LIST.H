/*
 *	%W%	Series3 BLP
 *
 *	- Image/priority list definitions.
 */

REC_LEN		equ	10;	/* Priority/image list record size */

/* Generic record offsets */
GR_TYPE		equ	1;	/* Record type */
GR_SKIP		equ	2;	/* Line skip count */
GR_ADDR_HI	equ	3;	/* Data address (hi word) */
GR_ADDR_LO	equ	4;	/* Data address (lo word) */
GR_XPHASE	equ	8;	/* Horizontal pattern/tint phase */
GR_YPHASE	equ	9;	/* Vertical pattern/tint phase */

/* Priority record offsets */
PR_LAST		equ	1;	/* Pointer to last priority record */
PR_PRIORITY	equ	2;	/* Priority of this record */
PR_MODE		equ	3;	/* Combination mode and buffer selection */
PR_PAT_TYPE	equ	4;	/* Pattern type */
PR_DATA1	equ	5;	/* Pattern data word 1 */
PR_DATA2	equ	6;	/* Pattern data word 2 */
PR_PAT_A+B	equ	7;	/* a + b */
PR_IMAGE	equ	9;	/* Pointer to image list */

/* Character record offsets */
CR_DICT_INDEX	equ	5;	/* Dictionary index */
CR_REPEAT	equ	7;	/* Scan line repeat count */

/* Shape record offsets */
SR_DEPTH	equ	3;	/* Depth */
SR_LEFT_TYPE	equ	4;	/* Left edge type */
SR_LEFT_REC	equ	5;	/* Left edge record */
SR_RIGHT_TYPE	equ	6;	/* Right edge type */
SR_RIGHT_REC	equ	7;	/* Right edge record */

/* Diagonal edge record offsets */
DR_E		equ	1;	/* Bresenham error term */
DR_ENEG		equ	6;	/* Value to add to e when -ve */

/* Scan conversion record offsets */
SC_X		equ	6;	/* X-position */
SC_PARAM_REC	equ	7;	/* Parameter record address */

SC_SCANLINE	equ	1;	/* Current scan depth */
SC_MAX_Y	equ	3;	/* Next maximum-y */
SC_NEXT_EDGE	equ	5;	/* Next edge address */
SC_REPEAT	equ	7;	/* Blank line repeat */
SC_SENT_ADDR	equ	8;	/* Sentinel edge address */

/* Picture record offsets */
PI_PCB_PTR	equ	5;	/* Picture control block pointer */
PI_REP_FACTOR	equ	7;	/* Bitmap replication factor */
PI_CELL_SIZE	equ	7;	/* Halftone cell size (minus 1) */

/* Graduated tint rectangle record offsets */
GT_DEPTH	equ	3;	/* Rule depth */
GT_X		equ	4;	/* X-position */
GT_HORIZ_REC	equ	5;	/* Horizontal record */
GT_LEFT_REC	equ	6;	/* Left vertical record */
GT_RIGHT_REC	equ	7;	/* Right vertical record */

GT_BIT_CON	equ	1;	/* Bitmap control word */
GT_CELLS/LINE	equ	3;	/* Numbers of cells per line */

GT_ENEG		equ	4;	/* Vertical record eneg parameter */

/* Tiled rectangle record offsets */
TI_TILE_ID	equ	5;	/* Tile identifier */
TI_DEPTH	equ	7;	/* Tile rectangle depth */
TI_PARAM_REC	equ	9;	/* Parameter record */

/* Picture header */
PIC_HDR_LEN	equ	32;	/* Picture header length */
PIC_WIDTH_OFF	equ	3;	/* Picture width offset */

/* Miscellaneous */
STOP_BLK_OFF	equ	2;	/* Picure control block stop block offset */
