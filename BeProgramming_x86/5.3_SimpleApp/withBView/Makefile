#
# SimpleApp's Makefile
#
# Copyright (C) 1994 Be Inc.  All Rights Reserved
#
# Modified by koga@ftgun.co.jp ('98. 1/8)
# Last Update: '98. 1/8
#

CC		= mwcc
LD		= mwld
APP_NAME	= SimpleApp
TARGET		= $(APP_NAME)
RESOURCE	= $(APP_NAME).rsrc
PREFIX		= /boot/develop/headers/be/precompiled/BeHeadersx86
CFLAGS		= -w all -opt speed -prefix $(PREFIX) -c
LDFLAGS		= 
#PREFIX		= /boot/develop/headers/be/precompiled/BeDebugHeadersx86  # for debugging
#CFLAGS		= -g -w all -opt speed -prefix $(PREFIX) -c  # for debugging
#LDFLAGS	= -sym on  # for debugging

OBJS		= \
	SimpleAppMain.o\
	SimpleApp.o\
	SimpleWin.o

$(TARGET): $(OBJS) $(RESOURCE)
	$(LD) -o $@ $(OBJS) -map $@.xMAP $(LDFLAGS)
	copyres $(RESOURCE) $@
	mimeset -f $@

.SUFFIXES: .o .cpp .cp .c

.cpp.o:
	$(CC) -o $@ $(CFLAGS) $<

.cp.o:
	$(CC) -o $@ $(CFLAGS) $<

.c.o:
	$(CC) -o $@ $(CFLAGS) $<

all:
	$(TARGET)

clean:
	rm -f $(APP_NAME) *.o *.xSYM *.xMAP

wc:
	wc *.h *.cpp *.cp *.c


#
# End of File
#

