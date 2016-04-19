/************************************************************************
 * $Id$
 * Copyright (c) 1998 Fort Gunnison, Inc.
 *
 * StickyMemoMenu.cp: MemoAppMenuクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		May. 04, 1998
 * Last update:	May. 04, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "StickyMemoMenu.h"
#include "GUI/MenuUtil.h"


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

const char	kNewLabel[]		= "New";
const char	kOpenLabel[]	= "Open" B_UTF8_ELLIPSIS;
const char	kCloseLabel[]	= "Close";
const char	kSaveLabel[]	= "Save";
const char	kSaveAsLabel[]	= "Save As" B_UTF8_ELLIPSIS;
const char	kAboutLabel[]	= "About StickyMemo" B_UTF8_ELLIPSIS;
const char	kQuitLabel[]	= "Quit";

/* ドキュメントウィンドウ用のメニュー情報 */
static MItemInfo	sDocFileMenu[]	= {
	{ kNewLabel,		NEW_DOC, 			'N' },
	{ kOpenLabel,		OPEN_DOC,			'O' },
	{ kCloseLabel,		B_QUIT_REQUESTED,	'W' },
	{ B_EMPTY_STRING,	0, NULL },
	{ kSaveLabel,		SAVE_DOC,			'S' },
	{ kSaveAsLabel,		SAVE_DOC_AS,		NULL },
	{ B_EMPTY_STRING,	0, NULL },
	{ kAboutLabel,		B_ABOUT_REQUESTED,	NULL },
	{ B_EMPTY_STRING,	0, NULL },
	{ kQuitLabel,		B_QUIT_REQUESTED,	'Q' },
	NULL
};
static MItemInfo	sDocEditMenu[]	= {
	{ "Cut",			B_CUT,				'X' },
	{ "Copy",			B_COPY,				'C' },
	{ "Past",			B_PASTE,			'V' },
	{ "Clear",			EDIT_CLEAR,			NULL },
	{ "Select All",		B_SELECT_ALL,		'A' },
	NULL
};
static MItemInfo	sDocDocumentMenu[]	= {
	{ "Adjust Line Width",	ADJUST_WIDTH,	NULL },
	NULL
};
static MInfo	sDocMenuBar[]	= {
	{ "File",	sDocFileMenu },
	{ "Edit",	sDocEditMenu },
	{ "Document",	sDocDocumentMenu },
	NULL
};


/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * MemoAppMenuクラスの公開メソッド
 */
BMenuBar*
MemoAppMenu::MakeDocMenuBar (void)
{
	return MenuUtil::MakeMenuBar(sDocMenuBar);
}


/*
 * End of File
 */
