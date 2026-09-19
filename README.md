## Microcode For The Series 3 Typesetter

The files in this directory were recently recovered from a set of 5-1/4 inch DOS
floppy disks that I wrote nearly 40 years ago. They constitute a microprogram I
developed for a raster image processor (RIP) back in the late 1980s that ran on
a bitslice processor.

The code was written between 1986-1989. I had a full-time job at the time (same
company) so this code was written in my spare time.

### Image Types

The RIP was capable of typesetting various kinds of objects:

* char (a type of raster image)
* rule (rectangle, i.e. two vertical edges)
* shape (like rule but with at least one edge that is diagonal or elliptical)
* scan (vector outlines; closed paths filled according to winding rules, like a
  PostScript path)
* line art (a type of raster image)
* bitmap (a type of raster image)
* halftone picture
* graduated tint region (interpolated grayscale image)
* tile (a repeating raster image)

Several different decorations could be applied to some of these objects:

* A/B pattern
* flat tints (halftone using a single dot pattern)
* tiles

The A/B pattern type produces repeating runs of black and white pixels in six
different configurations that could produce effects like horizontal, vertical,
and diagonal hatched patterns, as well as dashed lines and checkerboards.

Image objects had priorities associated with them that could be composited to
produce interesting graphic effects.

The scan object was added late in the development cycle and was hugely
challenging to implement even though it was largely copied from working C code.
Microcode is just more difficult to write and 16 registers is not enough!

### RIP Hardware

The important hardware components of the RIP, as far as the bitslice processor
was concerned, were:

* GPP (General Purpose Processor); Motorola 68000-based, probably a -20 or -30
  running VRTX RTOS
* OBP (Output Bitslice Processor); the bitslice (the focus of this archive)
* OBB (Output buffer board); hardware that buffered scanlines for laser output

These components were all VMEbus cards that plugged into the same backplane. The
GPP fed typesetting commands and data to the OBP that in turn processed them
into black/white runs of pixel data that were written to the OBB. The OBB drove a
LOU (Laser Output Unit) that modulated a scanning laser beam exposing a moving
photographic film. The output unit had resolution of 1000 dpi horizontally and
vertically, spanning a 12 or 17 inch width. During development, we also had
access to a custom laser printer that was connected to RIP and avoided having to
develop photographic film for testing.

### OBP Hardware

The bitslice architecture was designed around an Advanced Micro Devices chipset:

* AM29C101 16-bit CMOS processor
* AM2904 status and shift control
* AM2910 microprogram sequencer
* AM2925 micro cycle length controller

It contained three different onboard memories:

* a 64-bit by 4096 word instruction memory used to store the microprogram. This
was accessible to the VMEbus and could be loaded by the GPP enabling very quick
turn around of microcode changes without the need to burn a new program into
PROMs.

* a 16-bit wide by 64KiW static RAM was used to back up registers, store variables,
and maintain an list of active images called the image list. This is referred to
*local* (L) memory by the microcode. 

* a 16-bt wide by 1-4MiW dynamic RAM was used to communicate with the GPP and
store addition lists and image data. This is referred to as *main* (M) memory by
the microcode and was accessible to the VMEbus.

The bitslice was clocked at 30MHz and a single microinstruction typically
executed in 3 cycles.

No dedicated multiply or divide hardware was available so these operations were
performed in software. A 16 x 16 bit multiply took 1.7us and a 32 / 16 bit
divide took 3.5us. 

### OBB Hardware

The OBB was quite complex and provided memory to store 256 scanlines of 64KiBits
each, organised into 16 bands of 16 scanlines. Two bands provided temporary
scanline storage that could be combined with other bands to produce special
effects. The remaining 14 bands fed scanlines to the LOU for output. 

The board also contained memory for storing halftone dot bitmaps that was mapped
to the address space of the VMEbus and could therefore be initialized by the
GPP.

Instead of handling pixel data directly, the OBB contained several decoders that
supported different types of image data:

* character (text)
* line art (graphic image)
* pattern (repeating on/off pattern)
* bitmap
* halftone

Once the various decoder registers were set up for a scanline, image data could
simply be copied by the OBP from main memory into the OBB until the data for
that scanline ended. The OBP then advanced to the next scanline and repeated the
process. This made it very efficient to process image data since all run
computation was handled in hardware.

### GPP/OBP Communication

Communication between the GPP and OBP was synchronized by semaphores in main
memory. The OBP could send interrupts to the GPP when notable events occurred but
the OBP polled for events from the GPP periodically. (We found this design to be
much easier to implement than one in which the microcode is interruptible. This
was an important lesson learned from the previous typesetter.)

### Image Data

Image data (raster images) was copied into main memory by the GPP so that it
could, in turn, be copied by the OBP to one of the OBB decoders for rendering.
This data was divided into 128 word blocks that were chained together by
reserving the last word in each block for a the block number of the next block
in the image. This avoided fragmentation of main memory. The OBP had special
purpose hardware that automatically detected when a block number was being read
and allowed seamless and efficient chaining of data blocks.

Scan object data (edge coordinate data) was similarly handled but required scan
conversion before the run data was sent to the OBB.

### Addition Lists

The GPP sent information to be typeset in the form of addition lists of objects
that are to be added to the active image list. The addition list begins with a
count of records in the list, followed by the scanline number at which to
request another addition list from the GPP. 

Records that share the same priority are grouped together in the list. The group
begins with a priority record followed by a list of image records to be typeset
with that priority. A priority record specifies a numerical priority, OBB mode
(how data is combined with an output buffer), and pattern. The pattern is
optional but if present it is an A/B pattern or tint that is applied to all the
images in the group. 

### Image Lists

The OBP reads addition lists and merges their records with its active image
list. A double-linked priority list is maintained whose records point to a
double-linked list of images. These list were traversed for each band of
scanlines that is processed.

### Image Lifecycle

The OBP maintains a scanline counter that begins at zero and increases as a
typesetting job progresses. At the beginning of the job, or when the scanline
matches the next addition list request number, a new addition list is read and
incorporated into the active image list. The images in this list are
scan-converted and sent to the OBB for rendering, a band of scanlines at a time.
Eventually, the scanline number increases beyond the last scanline of an image,
at which point the images "dies" and is removed from the image list. If it is an
image with associated image data, e.g a char, its removal is recorded using a
special identifier in a death list. When this list becomes full or when the GPP
requests it, it is communicated to the GPP. The GPP can then manage the
resources of these dead images, e.g. by making the image blocks that were
allocated to the image available for reuse.

### OBP Software

The microprogram that ran on the OBP was called the Buffer Load Process (BLP).

The OBP was programmed using a set of METASTEP development tools from the STEP
engineering company of Sunnyvale, California. These ran on the MS-DOS operating
system and proved to be very flexible and reliable.

In addition to the METASTEP tools I was also using an MKS Toolkit that provided
a number of UNIX-like utilities. All source code editing was performed with the
*vi* editor that came with the toolkit.

I ran these tools on an 8086-based Amstrad PC1512 that had 5-1/4 inch floppies,
but I cannot remember if it had a hard disk.

The target bitslice architecture was specified using the METASTEP Definition
Language. This was complied into a into a definition file that subsequently
guided the execution of the METASTEP Assembler. The resulting relocatable object
files could then be linked into a binary that was loaded into microprogram
memory for execution. The microword field definitions are contained in file
OBP.MDL.

The METASTEP software was very permissive about the allowable characters for
names of fields and macros (but not square brackets [], for some reason). For
example: 

    r/w ; read/write cycle
    D->Y ; move data from the D bus to the Y bus
    Ic/ ; immediate carry negated
    Y-L<> ; Y-bus to local memory cell

I got fairly creative with macro naming.

### Mircroinstruction Timing

All microcode was assembled so that each instruction would take 10 cycles to
execute. This exceeded the number of cycles the most complex instruction would
need to complete execution but provided a safe margin during development that
avoided hardware race conditions. Later in the project I wrote a C program that
read and analyzed each microinstruction and reprogrammed the cycle length field
in the microinstruction to the minimum safe number of clock cycles for that
instruction. This sped up the code by more than a factor of three times.

The OBP had an array of five LEDs that I used to indicate when the code was
stuck in a loop waiting for an event:

* BAND_WAIT     Waiting for OBB band to become available
* LIST_LIST     Waiting for addition list
* DEATH_WAIT    Waiting for a death buffer to become available
* CMD_WAIT      Waiting to send command to GPP
* PIC_WAIT      Waiting for picture data

These could be monitored by a logic analyzer to better understand the dynamics
of program execution and adjustments made accordingly.

Ideally, the OBP would fill all 14 output buffers (14 x 16 = 224 scanlines) it
had available quicker than the laser could output them. In this case, the OBP
would busy-wait for a band of scanlines to become available. Thus, it was easy
to monitor the RIPs performance by observing the state of the BAND_WAIT LED.

A technician in the lab built a simple circuit that would monitor the
microprogram address and split and converted it into two voltage components that
could drive an oscilloscope in XY mode and thus reveal any unexpected hotspots
in the microprogram.

### Diagnostics

A set of diagnostic tests were run when the typesetter booted up and proved
useful in detecting faults in the OBB and OBP. One of these tests exercised the
dual-ported main RAM on the OBP by writing to and checking odd addresses while
the GPP was writing to and checking even addresses in the same memory. This was
a basic pattern and address-for-data tests, but I'm sure we'd have implemented a
march test if we had known about it.

The onboard scanline buffers of the OBB were mapped from the VMEbus. I wrote a
diagnostic tests that exercised much of the OBB's functionality. Executing these
tests modified the contents of the scanline buffers. These were subsequently
read by the GPP and compared to previously saved and verified scanline data.

### Miscellaneous

The OBP was debugged using visual output, an oscilloscope, and a logic analyzer.

Two excellent software engineers wrote the code for the GPP and one exceptional
hardware engineer designed the hardware. This is one of the most enjoyable
projects in my 37-year professional software engineering career.

I hope this might be of interest to see how complex embedded systems were
developed and programmed in the 1980s.

### Addendum

I noticed that the OBB diagnostic OBB.MAL apparently has old definition names
from OBB.H. They seem to map to the new names as follows:

| **Old** | **New** |
| :--- | :--- |
| RUNLAD | OBB_LINE_ADDR |
| RUNADD | OBB_RUN_ADDR |
| RUNMOD | OBB_RUN_MODE |
| RUNDAT | OBB_RUN_DATA |
| INC_BAND | OBB_BAND |
| OUTMOD | OBB_OUT_MODE |
| RES_BAND | OBB_RESET |
| INTRUN | OBB_INTL_RUN |
| PATRUN | OBB_PAT_RUN |
| BITCON | OBB_BIT_CON |
| BITDAT | OBB_BIT_DATA |
| STATUS | OBB_STATUS |
| READY | OBB_PIC_RDY |
| NODATA | OBB_RUN_BSY? |
| NOBAND | OBB_BAND_BSY? |
| BAND_CNT | OBB_BAND_CNT |
| LINE_CNT | OBB_LINE_CNT |

I also noticed there doesn't seem to be a test for the halftone decoder. I have
no idea why this would be omitted.

I can't remember what the Cx input to the AM2904 is connected to, but it is used
by the addIc instruction which implies that it is connected to Ic (immediate
carry). The only use I can find in the microcode is to add a register to itself
and feed the carry out back into the carry in. Used inconjunction with a rotate
left results in rotating the register left 2 places. I guess this works because
there can be no further bit propagation since bit 0 will always have the value 0
after the ADD.