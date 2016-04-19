/************************************************************************
 * $Id$
 * Copyright (c) 1997-1998 Fort Gunnison, Inc.
 *
 * RegularWindow.cp: RegularWindowクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 21, 1997
 * Last update:	Jan. 03, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "RegularWindow.h"
#include <app/Application.h>
#include <interface/MenuBar.h>
#include <support/Debug.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/* 文字列定数 */
const char	kSender[]	= "sender";


/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################		*/

/*
 * RegularWindowクラスの公開メソッド
 */
RegularWindow::RegularWindow (BRect frame, const char* title,
		window_type type, uint32 flags, uint32 quitMsg)
	: BWindow(frame, title, type, flags, B_CURRENT_WORKSPACE)
{
	fQuitMsg = quitMsg;
}

RegularWindow::~RegularWindow (void)
{  /* do nothing */  }

void
RegularWindow::InitContent (BView* inContent)
{
#if DEBUG
	this->Lock();
	ASSERT(this->IsHidden());
	ASSERT(this->CountChildren() == 0);
	this->Unlock();
#endif

	BRect	myBounds = this->Bounds();
	
	inContent->ResizeTo(myBounds.Width(), myBounds.Height());
	this->AddChild(inContent);
	this->Show();
	
	return;
}

void
RegularWindow::InitContent (BMenuBar* inMenuBar, BView* inContent)
{
#if DEBUG
	this->Lock();
	ASSERT(this->IsHidden());
	ASSERT(this->CountChildren() == 0);
	this->Unlock();
#endif

	BRect	myBounds = this->Bounds();
	BRect	mbarFrame;
	
	this->AddChild(inMenuBar);
	mbarFrame = inMenuBar->Frame();
	inContent->ResizeTo(
		myBounds.Width(), myBounds.Height() - mbarFrame.Height() - 1);
	inContent->MoveBy(0, mbarFrame.Height() + 1);
	this->AddChild(inContent);
	this->Show();
	
	return;
}

/*
 * 終了時処理; RegularWindow
 */
void
RegularWindow::Quit (void)
{
	BMessage	quitMessage(fQuitMsg);
	
	/* アプリケーションに終了を通知 */
	quitMessage.AddPointer(kSender, this);
	(void)be_app->PostMessage(&quitMessage);
	
	/* 親クラスのメソッドを実行 */
	BWindow::Quit();
	
	return;
}


/*
 * End of File
 */
