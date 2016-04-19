/************************************************************************
 * $Id$
 * Copyright (c) 1997-1998 Fort Gunnison, Inc.
 *
 * SmartWindow.cp: SmartWindowクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 14, 1997
 * Last update:	Feb. 12, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "SmartWindow.h"
#include <interface/Alert.h>
#include <interface/MenuBar.h>
#include <support/Debug.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/* アラート用のメッセージ */
const char	kConfirmQuitMsg[] = "If close this window, "
	"the application will quit.\n"
	"Are you OK?";



/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/*
 * SmartWindowクラスの非公開メソッド
 */
bool
SmartWindow::QuitRequested (void)
{
	bool		isOK;
	BAlert*		alertPanel;
	BMessage*	theMessage = this->CurrentMessage();
	
	/* アプリケーション全体を終了する場合は何もしない */
	if (theMessage == NULL || theMessage->what != B_QUIT_REQUESTED)
		return true;

	/* 終了してもよいかユーザに確認 */
	alertPanel = new BAlert(
		B_EMPTY_STRING, kConfirmQuitMsg,
		"Cancel", "OK", NULL,
		B_WIDTH_FROM_WIDEST, B_WARNING_ALERT
	);
	alertPanel->SetShortcut(0, B_ESCAPE);
	isOK = (alertPanel->Go() == 1);
	
	return isOK;
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * SmartWindowクラスの公開メソッド
 */
SmartWindow::SmartWindow (BRect frame, const char* title,
		window_type type, uint32 flags, uint32 workspace)
	: SingleWindow(frame, title, type, flags, workspace)
{  /* do nothing */  }

SmartWindow::~SmartWindow (void)
{  /* do nothing */  }

void
SmartWindow::InitContent (BMenuBar* inMenuBar, BView* inContent)
{
#if DEBUG
	this->Lock();
	ASSERT(this->IsHidden());
	ASSERT(this->CountChildren() == 0);
	this->Unlock();
#endif

	BRect		myBounds = this->Bounds();
	BRect		mbarFrame;

	/* メニューバーを貼り付ける */
	this->AddChild(inMenuBar);
	mbarFrame = inMenuBar->Frame();
	
	/* ビューのサイズを調節 */
	inContent->ResizeTo(
		myBounds.Width(), myBounds.Height() - mbarFrame.Height() - 1
	);
	inContent->MoveBy(0, mbarFrame.Height() + 1);
	
	/* ビューを貼り付ける */
	this->AddChild(inContent);
	
	/* スクリーンに表示 */
	this->Show();
	
	return;
}


/*
 * End of File
 */
