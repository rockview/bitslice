/*
 *	%W%	Series3 BLP
 *
 *	- Scan conversion definitions.
 */

/* Sentinel edge offset */
SE_PATH_CNT	equ	1;			/* Image path count */
SE_LEFT_X	equ	2;			/* Leftmost x value */
SE_FILL_RULES	equ	16;			/* Array of filling rules */

/* Edge offsets */
EG_X		equ	2;			/* Current x-intercept */
EG_E_POS_INCR	equ	6;			/* Beresenham e-positive term */
EG_X_NEG_INCR	equ	8;			/* Bresenham x-negative term */
EG_TYPE		equ	11;			/* Edge type */
EG_END_ADDR	equ	12;			/* End point address */
EG_NEXT_EDGE	equ	14;			/* Next edge address */
EG_FIRST_POINT	equ	16;			/* First point in edge */
EG_MAX_Y	equ	18;			/* Maximum y-value in an edge */

/* Constants */
RIGHT_EDGE	equ	h'4000';		/* Right edge type */
PATH_MASK	equ	h'3fff';		/* Path number mask */

/* Fixed point constants */
FP_PLACES	equ	6;			/* Fractional binary places */
FP_FRAC_MASK	equ	((1 << FP_PLACES) - 1);	/* Fraction mask */
FP_ONE		equ	(1 << FP_PLACES);	/* Fixed point rep. of 1.0 */
FP_HALF		equ	(FP_ONE >> 1);		/* Fixed point rep. of 0.5 */
FP_MAX_HI	equ	h'7fff';		/* Max fixed point value (hi) */

HI_ADDR_MASK	equ	h'00ff';		/* Mask GPP VME address */
GPP_VME_SEL	equ	h'0400';		/* Selects GPP VME addr space */
