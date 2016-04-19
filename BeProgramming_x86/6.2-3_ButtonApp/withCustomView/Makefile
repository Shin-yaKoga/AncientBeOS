#
# ButtonApp's Makefile
#
# Copyright (C) 1994 Be Inc.  All Rights Reserved
#
# Modified by koga@ftgun.co.jp ('98. 1/8)
# Last Update: '98. 1/8
#

CC		= mwcc
LD		= mwld
APP_NAME	= ButtonApp
TARGET		= $(APP_NAME)
RESOURCE	= $(APP_NAME).rsrc
PREFIX		= /boot/develop/headers/be/precompiled/BeHeadersx86
CFLAGS		= -w all -opt speed -prefix $(PREFIX) -c
LDFLAGS		= 
#PREFIX		= /boot/develop/headers/be/precompiled/BeDebugHeadersx86
#CFLAGS		= -g -w all -opt speed -prefix $(PREFIX) -c  # for debugging
#LDFLAGS	= -sym on  # for debugging

COM_DIR		= ../Common

OBJS		= \
	ButtonAppMain.o\
	ButtonApp.o\
	SingleWindow.o\
	ButtonView.o\
	KGCircle.o

$(TARGET): $(OBJS) $(COM_DIR)/$(RESOURCE)
	$(LD) -o $@ $(OBJS) -map $@.xMAP $(LDFLAGS)
	copyres $(COM_DIR)/$(RESOURCE) $@
	mimeset -f $@

ButtonAppMain.o:
	$(CC) -o $@ $(CFLAGS) $(COM_DIR)/ButtonAppMain.cp
ButtonApp.o:
	$(CC) -o $@ $(CFLAGS) -I../withCustomView $(COM_DIR)/ButtonApp.cp
SingleWindow.o:
	$(CC) -o $@ $(CFLAGS) $(COM_DIR)/SingleWindow.cp

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

