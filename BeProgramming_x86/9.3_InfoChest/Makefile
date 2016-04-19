#
# FileSearch's Makefile
#
# Copyright (C) 1994 Be Inc.  All Rights Reserved
#
# Modified by koga@ftgun.co.jp ('98. 1/8)
# Last Update: '98. 4/29
#

CC		= mwcc
LD		= mwld
APP_NAME	= InfoChest
TARGET		= $(APP_NAME)
RESOURCE	= $(APP_NAME).rsrc
PREFIX		= /boot/develop/headers/be/precompiled/BeHeadersx86
CFLAGS		= -w all -opt speed -prefix $(PREFIX) -c
#PREFIX		= /boot/develop/headers/be/precompiled/BeDebugHeadersx86  # for debugging
#CFLAGS		= -g -w all -opt spped -prefix $(PREFIX) -c  # for debugging
#LDFLAGS	= -sym on  # for debugging

COM_DIR		= _FromFileSearch
MYLIB_DIR	= ../MyLib
KGUTIL_DIR	= $(MYLIB_DIR)/KGUtility
GUI_DIR		= $(MYLIB_DIR)/GUI
INCLUDE		= -I./$(COM_DIR) -I$(MYLIB_DIR) -I$(GUI_DIR)

OBJS		= \
	InfoChestMain.o\
	InfoChestApp.o\
	InfoChestWin.o SearchWindow.o DocumentsWin.o\
	InfoChestView.o DocumentsView.o\
	Categorizer.o\
	SearchView.o RetrieverView.o\
	SearchEngine.o\
	EntryListView.o EntryListItem.o\
	MenuUtil.o\
	RegularWindow.o\
	SimpleDialog.o\
	kgDebug.o\
	kgAlert.o\

$(TARGET): $(OBJS) $(RESOURCE)
	$(LD) -o $@ $(OBJS) -map $@.xMAP $(LDFLAGS)
	copyres $(RESOURCE) $@
	mimeset -f $@

SearchView.o:
	$(CC) -o $@ -I./ $(INCLUDE) $(CFLAGS) $(COM_DIR)/SearchView.cp
RetrieverView.o:
	$(CC) -o $@ -I./ $(INCLUDE) $(CFLAGS) $(COM_DIR)/RetrieverView.cp
SearchEngine.o:
	$(CC) -o $@ $(INCLUDE) $(CFLAGS) $(COM_DIR)/SearchEngine.cp
EntryListView.o:
	$(CC) -o $@ $(INCLUDE) $(CFLAGS) $(GUI_DIR)/EntryListView.cp
EntryListItem.o:
	$(CC) -o $@ $(INCLUDE) $(CFLAGS) $(GUI_DIR)/EntryListItem.cp
MenuUtil.o:
	$(CC) -o $@ $(INCLUDE) $(CFLAGS) $(GUI_DIR)/MenuUtil.cp
MonoWindow.o:
	$(CC) -o $@ $(INCLUDE) $(CFLAGS) $(GUI_DIR)/MonoWindow.cp
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

