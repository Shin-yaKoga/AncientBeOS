#
# MemoApps's Makefile
#
# Copyright (C) 1994 Be Inc.  All Rights Reserved
#
# Modified by koga@ftgun.co.jp ('98. 1/8)
# Last Update: '98. 5/5
#

CC		= mwcc
LD		= mwld
APP_NAME	= MemoApp
TARGET		= $(APP_NAME)
RESOURCE	= $(APP_NAME).rsrc
PREFIX		= /boot/develop/headers/be/precompiled/BeHeadersx86
CFLAGS		= -w all -opt speed -prefix $(PREFIX) -c
LDFLAGS		=
#PREFIX		= /boot/develop/headers/be/precompiled/BeDebugHeadersx86 # for debugging
#CFLAGS		= -g -w all -opt spped -prefix $(PREFIX) -c  # for debugging
#LDFLAGS	= -sym on  # for debugging

MYLIB_DIR	= ../MyLib
KGUTIL_DIR	= $(MYLIB_DIR)/KGUtility
GUI_DIR		= $(MYLIB_DIR)/GUI
INCLUDE		= -I$(MYLIB_DIR) -I$(GUI_DIR)

OBJS		= \
	MemoAppMain.o\
	MemoAppMenu.o\
	MemoApp.o\
	MemoWindow.o\
	MyTextView.o\
	MenuUtil.o\
	RegularWindow.o\
	kgDebug.o\
	kgAlert.o\

$(TARGET): $(OBJS) $(RESOURCE)
	$(LD) -o $@ $(OBJS) -map $@.xMAP $(LDFLAGS)
	copyres $(RESOURCE) $@
	mimeset -f $@

MenuUtil.o:
	$(CC) -o $@ $(INCLUDE) $(CFLAGS) $(GUI_DIR)/MenuUtil.cp
RegularWindow.o:
	$(CC) -o $@ $(INCLUDE) $(CFLAGS) $(GUI_DIR)/RegularWindow.cp
SimpleDialog.o:
	$(CC) -o $@ $(INCLUDE) $(CFLAGS) $(GUI_DIR)/SimpleDialog.cp
kgDebug.o:
	$(CC) -o $@ $(CFLAGS) $(KGUTIL_DIR)/kgDebug.cp
kgAlert.o:
	$(CC) -o $@ $(CFLAGS) $(KGUTIL_DIR)/kgAlert.cp

.SUFFIXES: .o .cpp .cp .c

.cpp.o:
	$(CC) -o $@ $(INCLUDE) $(CFLAGS) $<

.cp.o:
	$(CC) -o $@ $(INCLUDE) $(CFLAGS) $<

.c.o:
	$(CC) -o $@ $(INCLUDE) $(CFLAGS) $<

all:
	$(TARGET)

clean:
	rm -f $(APP_NAME) *.o *.xSYM *.xMAP

wc:
	wc *.h *.cpp *.cp *.c


#
# End of File
#

