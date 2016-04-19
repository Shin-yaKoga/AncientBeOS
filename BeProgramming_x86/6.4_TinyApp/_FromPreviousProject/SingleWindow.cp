/************************************************************************
 * $Id$
 * Copyright (c) 1997, Fort Gunnison, Inc.
 *
 * SingleWindow.cp: SingleWindowクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 12, 1997
 * Last update:	Dec. 12, 1997
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "SingleWindow.h"
#include <app/Application.h>
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

/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/* SingleWindowクラスの公開メソッド */
/*
 * 初期化と解放; SingleWindow
 */
SingleWindow::SingleWindow (BRect frame, const char* title,
		window_type type, uint32 flags, uint32 workspace)
	: BWindow(frame, title, type, flags, workspace)
{  /* do nothing */  }

SingleWindow::~SingleWindow (void)
{  /* do nothing */  }

void
SingleWindow::InitContent (BView* inContent)
{
#if DEBUG
	this->Lock();
	ASSERT(this->IsHidden());
	ASSERT(this->CountChildren() == 0);
	this->Unlock();
#endif
	
	BRect	myBounds = this->Bounds();

	/* ビューのサイズを調節 */
	inContent->ResizeTo(myBounds.Width(), myBounds.Height());
	
	/* ビューを貼り付ける */
	this->AddChild(inContent);
	
	return;
}

/*
 * 終了時処理; SingleWindow
 */
void
SingleWindow::Quit (void)
{
	/* アプリケーション本体に終了通知 */
	be_app->PostMessage(B_QUIT_REQUESTED, be_app);
	
	/* 親クラスのメソッドを実行 */
	BWindow::Quit();
	
	return;
}


/*
 * End of File
 */
