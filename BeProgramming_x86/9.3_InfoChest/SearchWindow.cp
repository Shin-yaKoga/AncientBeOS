/************************************************************************
 * $Id$
 * Copyright (c) 1998 Fort Gunnison, Inc.
 *
 * SearchWindow.cp: SearchWindowクラスの実装(改訂版)
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 23, 1998
 * Last update:	May. 04, 1998
 *
 * 注意：この改訂版は、MonoWindowではなくRegularWindowクラスを継承する
 *		ように変更した以外は、"FileSearch"アプリケーションで使っている
 *		オリジナルと変わらない。
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "SearchWindow.h"
#include "RetrieverView.h"

#include <support/ClassInfo.h>
#include <support/Debug.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/* SearchWindowクラスの非公開メソッド */
/*
 * メニュー調節; SearchWindow
 */
void
SearchWindow::MenusBeginning (void)
{
	ASSERT(this->KeyMenuBar() != NULL && fSearchView != NULL);
	
	fSearchView->AdjustMenuItems(this->KeyMenuBar());
	return;
}

/*
 * メッセージ処理; SearchWindow
 */
void
SearchWindow::MessageReceived (BMessage* message)
{
	ASSERT(fSearchView != NULL);

	switch (message->what) {
	case B_NODE_MONITOR:
		fSearchView->MessageReceived(message);	break;
	default:
		RegularWindow::MessageReceived(message);
	}
	
	return;
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * SearchWindowクラスの公開メソッド
 */
SearchWindow::SearchWindow (BRect frame, const char* title,
		window_type type, uint32 flags, uint32 quitMsg)
	: RegularWindow(frame, title, type, flags, quitMsg)
{
	fSearchView = NULL;
}

SearchWindow::~SearchWindow (void)
{  /* do nothing */  }

void
SearchWindow::InitContent (BView* inContent)
{
	RegularWindow::InitContent(inContent);
	fSearchView = cast_as(inContent, RetrieverView);
	
	return;
}

void
SearchWindow::InitContent (BMenuBar* inMenuBar, BView* inContent)
{
	RegularWindow::InitContent(inMenuBar, inContent);
	fSearchView = cast_as(inContent, RetrieverView);
	
	return;
}


/*
 * End of File
 */
