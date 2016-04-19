/************************************************************************
 * $Id$
 * Copyright (c) 1998 Fort Gunnison, Inc.
 *
 * DocumentsWin.cp: DocumentsWinクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 24, 1998
 * Last update:	May. 04, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "DocumentsWin.h"
#include "DocumentsView.h"  /* for sort menus */
#include "GUI/MenuUtil.h"

#include <support/Debug.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/* メニュー情報 */
static MItemInfo	sSortMenu[]	= {  /* Sortメニュー */
	{ "By Name",		SORT_BY_NAME,		NULL },
	{ "By Modified",	SORT_BY_MODIFIED,	NULL },
	{ "By Size",		SORT_BY_SIZE,		NULL },
	NULL
};


/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/* DocumentsWinクラスの非公開メソッド */
/*
 * メニュー調節; DocumentsWin
 */
void
DocumentsWin::MenusBeginning (void)
{
	ASSERT(this->KeyMenuBar() != NULL && fDocumentsView != NULL);
	
	fDocumentsView->AdjustMenuItems(this->KeyMenuBar());
	return;
}

/*
 * メッセージ処理; DocumentsWin
 */
void
DocumentsWin::MessageReceived (BMessage* message)
{
	ASSERT(fDocumentsView != NULL);
	
	switch (message->what) {
	case B_NODE_MONITOR:
		fDocumentsView->MessageReceived(message);	break;
	default:
		RegularWindow::MessageReceived(message);
	}
	
	return;
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/* DocumentsWinクラスの公開メソッド */
/*
 * 初期化と解放; DocumentsWin
 */
DocumentsWin::DocumentsWin (BRect frame, const char* title,
		window_type type, uint32 flags, uint32 quitMsg)
	: RegularWindow(frame, title, type, flags, quitMsg)
{
	fDocumentsView = NULL;
}

DocumentsWin::~DocumentsWin (void)
{  /* do nothing */  }

void
DocumentsWin::InitContent (BView* inContent)
{
	RegularWindow::InitContent(inContent);
	fDocumentsView = cast_as(inContent, RetrieverView);
	
	return;
}

void
DocumentsWin::InitContent (BMenuBar* inMenuBar, BView* inContent)
{
	BMenu*	theSortMenu;
	
	/* Sortメニューを生成してメニューバーに追加 */
	theSortMenu = MenuUtil::MakeMenu("Sort", sSortMenu);
	theSortMenu->SetRadioMode(true);
	inMenuBar->AddItem(theSortMenu);

	/* ウィンドウ内容をセットアップ */
	RegularWindow::InitContent(inMenuBar, inContent);
	fDocumentsView = cast_as(inContent, RetrieverView);
	(void)theSortMenu->SetTargetForItems(fDocumentsView);
	
	return;
}


/*
 * End of File
 */
