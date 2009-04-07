For VEXPro do the following:

1) Turn off VEXPro
2) Install jumper J5
3) Hook up console to USB port
4) Run from this directory:
./download -t 9302 -p1 ../../redboot/redboot.bin
5) Turn on VEXPro
6) Wait-- it should automatically upload and indicate success 
7) Don't forget to remove jumper J5

*******************************************************************************


                                   Download

                     Copyright (c) 2005 Cirrus Logic, Inc.


*******************************************************************************

OVERVIEW

The download application is used to program either the NOR FLASH or the SPI
EEPROM connected to the Cirrus Logic ARM processor.  For boards with an EP73xx,
it can program the NOR FLASH connected to nCS0.  For boards with an EP93xx, it
can program the NOR FLASH connected to nCS6, as well as the SPI EEPROM
connected to the SPI port.  It supports all sizes of Intel B3, C3, and J3
FLASH, in either 16-bit or 32-bit wide configuraitons (where the 32-bit wide
configurations are simply two 16-bit FLASHes connected in parallel on the
32-bit wide bus).  For SPI EEPROM, it supports only the Atmel 25F1024 EEPROM.

-------------------------------------------------------------------------------

ARGUMENTS

The arguments for the download program are as follows:

    -b <baud>    Specifies the baud rate at which the file data is downloaded.
                 The initial boot code is downloaded at 9600 (which is hard
                 coded into the boot ROM of the part); the baud rate is changed
                 to this value after the initial boot code download but before
                 sending the file data.  The valid values are 9600, 19200,
                 38400, 57600, and 115200.  The default value is 115200.

    -h           Displays the usage message for the download application.

    -n <number>  Specifies the board number, which, along with the board type,
                 implies the Ethernet MAC address.  This implies the -s
                 argument.  This will place a 32 byte Ethernet MAC block at
                 offset 4096 in the SPI EEPROM; if an image is specified these
                 bytes will be replaced with the Ethernet MAC block.  When the
                 board number is specified, it is not required to specify an
                 image to download.

    -o <offset>  The offset into the memory where the image should be
                 programmed.  This value must correspond to a block address
                 boundary; the download will fail without modifying the FLASH
                 if it is not.  The default value is 0.

    -p <port>    The serial port that is used to communicate with the board.
                 The valid values are 1, 2, 3, and 4 (for /dev/ttyS0 thorough
                 /dev/ttyS3, respectively).  The default value is 1.

    -s           Causes download to program into the SPI EEPROM instead of the
                 NOR FLASH.  This will fail for EP73xx boards.  A portion of
                 the image being downloaded can be overwritten by the Ethernet
                 MAC block if the board number is specified.

    -t <type>    Specifies the board type, which, along with the board number,
                 implies the Ethernet MAC address.  This is used in conjunction
                 with -n; if -n is not specified, then this value is ignored.
                 Valid values are 9301, 9302, 9307, 9312, and 9315. The value is
                 9315 for EDB9315A board. The default is 9312.

    -v           Displays the version number of the download application.

    <filename>   The name of the file to be programmed into the NOR FLASH or
                 SPI EEPROM.  This argument is optional if -n is specified.

-------------------------------------------------------------------------------

USAGE

To program the file "foobar.bin" into the NOR FLASH at offset 0, using serial
port 1, and data speed of 115200, simply type the following:
    download foobar.bin

To program it into the SPI EEPROM of an EDB9312, board number 15, type the
following:

    download -t 9312 -n 15 -s foobar.bin

Once download is ready to talk to the board, it will display:

    Waiting for the board to wakeup...

At this point, download is waiting for the board to initiate the serial boot
sequence.  This sequence varies for each board:

For the EDB7312:

    1) Connect the "serial 1" port of the board to the PC with a NULL modem
       cable.  For non-EDB7312 boards that use an EP73xx, use the serial port
       that is connected to UART1 of the EP73xx.
    2) While holding the "CPU BROM" button on the EDB7312 board, press and
       release the "CPU POR" button.
    3) Release the "CPU BROM" button.

For the EDB9301, EDB9302, EDB9312, and EDB9315:

    1) Connect the "UART1" port of the board (i.e. the bottom serial connector)
       to the PC with a NULL modem cable.
    2) Move the TEST0 jumper (JP14 on the EDB9301/EDB9302 or JP92 on the
       EDB9312/EDB9315) from 2-3 to 1-2.
    3) Press and release the RESET button.
    4) In order to boot properly, the TEST0 jumper must be moved back to 2-3
       once the download process is complete.

For the EDB9307:

   1) Connect the "UART1" port of the board (i.e. the bottom serial connector)
      to the PC with a NULL modem cable.
   2) PRESS/HOLD S3 (BOOT 0).
   3) PRESS/RELEASE SW2 (RESET).
   4) RELEASE S3 (BOOT 0).

For the EDB9315A:

   1) Connect the "UART" port of the board to the PC with a NULL modem cable.
   2) PRESS/HOLD S1 (SERIAL BOOT).
   3) PRESS/RELEASE S3 (POR).
   4) RELEASE S1 (SERIAL BOOT).

At this point, download should start sending the initial boot code.  As it does
so, it will display its status as follows:

    Downloading boot code...(67%)

After the initial boot code is downloaded, it will check the starting offset
and image length to make sure they fit into the memory being programmed.  For
all of the following messages, programming the NOR FLASH is assumed.  If the
SPI EEPROM is being programmed, all occurrances of FLASH would appear as
EEPROM.  If the image is too large to fit into the memory that was detected on
the board, it will display the following message and exit:

    The image is too large for the FLASH.

If the starting offset does not correspond to a block boundary, it will display
the following message and exit:

    The offset '543' is not valid for the FLASH.

If the starting address and image size are valid, it will erase the portion of
memory to be programmed.  It could take several seconds to erase the memory so
be patient.  It will display its status as follows:

    Erasing the FLASH...

Once the memory is erased, it will program the contents of the memory.  It will
display its status as follows:

    Programming the FLASH...(17%)

After the memory has been programmed, download will display the final results
of the download.  The data is checksummed as it is transferred over the serial
link, and read back from memory after it has been programmed to compare it to
the data received from the PC.  If there is an error in any of these
verification steps, download will display the following message when complete:

    Failed to program 'filename'.

Or it will display the following message if a problem occurred while
programmed just the Ethernet MAC address:

    Failed to program the Ethernet MAC address.

If everything was successful, it will display the following message:

    Successfully programmed 'filename'.

Or it will display the following message if just the Ethernet MAC address was
being programmed:

    Successfully programmed the Ethernet MAC address.

It is possible that the PC and the target get a bit out of sync during the
download process.  If this happens, the download program attempts to recover
the communication link gracefully.  If it is unable to, it will eventually
time out.  If the progress seems to be stuck, wait at least two minutes before
killing the download program.

For memories that support block locking, download will attempt to unlock the
blocks before erasing them.  For blocks that are hardware locked (either via
permanent lock mechanism in the programming interface itself or a pin on the
chip itself), there is nothing that download can do; in this case, the download
will fail.

-------------------------------------------------------------------------------

FILES

The following files are in the package:

    asmdefs.h    - A set of #defines used to make the assembly code independent
                   of the assembler being used.  Assembler directives and
                   certain other operators are different between various
                   assemblers; this file abstracts those differences so that a
                   single assembler file can be assembled by a variety of tool
                   chains.  The ARM SDT 2.5x, ARM ADS 1.x, and gcc tool chains
                   are currently supported.

    assemble.sh  - A shell script used to assmeble .S files under Windows.
                   Since the ARM SDT and ADS tool chains do not support passing
                   an assembler file through the C preprocessor and then to the
                   assembler, this script does this manually.  It depends upon
                   a Un*x-like shell and some other Un*x commands.

    bin          - Contains Windows versions of the various Un*x commands
                   required to build the download utility.  These executables
                   were taken from the UnxUtils package on
                   unxutils.sourceforge.net.

    download.c   - The source for the helper application that runs on the PC to
                   download images to the board.  It uses the code image
                   generated from flasher.S as the initial boot code.

    download.exe - A pre-built version of the download utility for Windows.

    flasher.lds  - A GNU ld script for generating the programmer code that runs
                   on the ARM processor.

    flasher.S    - The source for the programmer code that runs on the ARM
                   processor.  This is highly optimized thumb code that should
                   only be modified with great care to avoid breaking the
                   existing functionality.  The code image produced from this
                   must be <= 2048 bytes, can only perform word writes to the
                   data area, and must be fully position independent.

    getopt.c     - A getopt() implementation taken from glibc 2.2.5.

    getopt.h     - The header file for the getopt() implementation.  This was
                   also taken from glibc 2.2.5.

    LICENSE      - The license that governs this software release.

    makedefs     - Rules for building libraries and applications for the ARM
                   processor.  These rules are good for building standalone
                   libraries and applications, not ones that run on an OS such
                   as Linux.

    Makefile     - The rules for building download.  If building under Windows,
                   GNU make version 3.78 or greater must be used, and a bourne-
                   compatible shell called sh.exe must exist in the search
                   path; the contents of the bin directory of this package
                   provide the applications necessary to build the source code
                   (outside the actual compilers).

    mkarray.awk  - An awk script that turns the binary data of the ARM code
                   generated from flasher.S and turns it into a C array in a
                   header file for use by download.c.

-------------------------------------------------------------------------------

BUILDING THE SOURCE

To build download, simply type "make".  The appropriate cross compiler and
native compiler must be in the search path.  By default, gcc is used as the
cross compiler and native compiler when building on Linux; ARM ADS 1.x and MSVC
are used as the cross compiler and native compiler when building in Windows.

If building on Windows, the appropriate Un*x-like utility must also be in the
search path; use the bin directory of this package if necessary.  Many Windows
tool chains have their own version of make; be sure that GNU make is the one
executed when you type "make" or the build will surely fail.

*******************************************************************************
