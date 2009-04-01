#!/bin/sh
#*****************************************************************************
#
# assemble.sh - Script to build a .S file with the ARM tools, manually passing
#               it through the C preprocessor and then sending it to the
#               assembler (since the compiler driver won't do it itself).
#
# Copyright (c) 2004 Cirrus Logic, Inc.
#
#******************************************************************************

#
# Echo the command line.
#
echo ${0} ${*}

#
# Check the arguments.
#
if [ $# != 1 ]
then
    echo Usage: assemble \<filename\>
    exit 1
fi

#
# Compute the various file names that will be used during the process.
#
C_NAME=`echo ${1} | sed 's/\.[Ss]/_.c/g'`
S_NAME=`echo ${1} | sed 's/\.[Ss]/_.s/g'`
D_NAME=`echo ${1} | sed 's/\.[Ss]/_.d/g'`
O_NAME=`echo ${1} | sed 's/\.[Ss]/_.o/g'`
DNAME=`echo ${1} | sed 's/\.[Ss]/.d/g'`
ONAME=`echo ${1} | sed 's/\.[Ss]/.o/g'`

#
# Copy the assembly file to a temporary .c file.
#
cp ${1} ${C_NAME}
if [ $? != 0 ]
then
    exit 1
fi

#
# Make sure the temporary .c file is writable (it might not be).
#
chmod +w ${C_NAME}
if [ $? != 0 ]
then
    rm -f ${C_NAME}
    exit 1
fi

#
# Pass the temporary .c file through the C preprocessor.
#
${CC} `echo ${CFLAGS}` -E ${C_NAME} > __tmp.c
if [ $? != 0 ]
then
    rm -f ${C_NAME} __tmp.c
    exit 1
fi

#
# Remove the #line directives from the preprocessed output.
#
grep -v ^\#line __tmp.c > ${S_NAME}
if [ $? != 0 ]
then
    rm -f ${C_NAME} ${D_NAME} __tmp.c
    exit 1
fi

#
# Assemble the preprocessed output.
#
${AS} `echo ${AFLAGS}` -o ${ONAME} ${S_NAME}
if [ $? != 0 ]
then
    rm -f ${C_NAME} ${D_NAME} ${S_NAME} ${ONAME} __tmp.c
    exit 1
fi

#
# Convert the references to the _.c file name to the .s file name in the
# dependencies (.d) file.
#
sed s/${C_NAME}/${1}/g ${D_NAME} > __tmp.d
if [ $? != 0 ]
then
    rm -f ${C_NAME} ${D_NAME} ${S_NAME} ${ONAME} __tmp.c __tmp.d
    exit 1
fi

#
# Convert the references to the _.o file name to the .o file name in the
# dependencies (.d) file.
#
sed s/${O_NAME}/${ONAME}/g __tmp.d > ${DNAME}
if [ $? != 0 ]
then
    rm -rf ${C_NAME} ${D_NAME} ${S_NAME} ${ONAME} __tmp.c __tmp.d
    exit 1
fi

#
# Remove all temporary files.
#
rm -f ${C_NAME} ${S_NAME} ${D_NAME} __tmp.c __tmp.d

#
# Success.
#
exit 0
