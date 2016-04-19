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
APP_NAME	= MemoMailer
TARGET		= $(APP_NAME)
RESOURCE	= $(APP_NAME).rsrc
PREFIX		= /boot/develop/headers/be/precompiled/BeHeadersx86
CFLAGS		= -w all -opt speed -prefix $(PREFIX) -c
LDFLAGS		=
#PREFIX		= /boot/develop/headers/be/precompiled/BeDebugHeadersx86  # for debugging
#CFLAGS		= -g -w all -opt spped -prefix $(PREFIX) -c  # for debugging
#LDFLAGS	= -sym on  # for debugging

COM_DIR		= _FromMemoApp
COM_DIR2	= _FromMemoMailer
MYLIB_DIR	= ../MyLib
KGUTIL_DIR	= $(MYLIB_DIR)/KGUtility
GUI_DIR		= $(MYLIB_DIR)/GUI
INCLUDE		= -I$(COM_DIR2) -I$(COM_DIR) -I$(MYLIB_DIR) -I$(GUI_DIR)

OBJS		= \
	MemoMailerMain.o\
	MemoMailerApp.o\
	MailWindow.o\
	MailSender.o\
	MyMemoWindow.o MemoMailerMenu.o\
	MemoWindow.o\
	MyTextView.o\
	EntryListView.o EntryListItem.o\
	MenuUtil.o\
	RegularWindow.o\
	kgDebug.o\
	kgAlert.o\

$(TARGET): $(OBJS) $(RESOURCE)
	$(LD) -o $@ $(OBJS) -map $@.xMAP $(LDFLAGS)
	copyres $(RESOURCE) $@
	mimeset -f $@

MyMemoWindow.o:
	$(CC) -o $@ -I./ $(INCLUDE) $(CFLAGS) $(COM_DIR2)/MyMemoWindow.cp
MemoMailerMenu.o:
	$(CC) -o $@ -I./ $(INCLUDE) $(CFLAGS) $(COM_DIR2)/MemoMailerMenu.cp
MemoWindow.o:
	$(CC) -o $@ -I./ $(INCLUDE) $(CFLAGS) $(COM_DIR)/MemoWindow.cp
MyTextView.o:
	$(CC) -o $@ -I./ $(INCLUDE) $(CFLAGS) $(COM_DIR)/MyTextView.cp
EntryListView.o:
	$(CC) -o $@ $(INCLUDE) $(CFLAGS) $(GUI_DIR)/EntryListView.cp
EntryListItem.o:
	$(CC) -o $@ $(INCLUDE) $(CFLAGS) $(GUI_DIR)/EntryListItem.cp
MenuUtil.o:
	$(CC) -o $@ $(INCLUDE) $(CFLAGS) $(GUI_DIR)/MenuUtil.cp
RegularWindow.o:
	$(CC) -o $@ $(INCLUDE) $(CFLAGS) $(GUI_DIR)/RegularWindow.cp
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

