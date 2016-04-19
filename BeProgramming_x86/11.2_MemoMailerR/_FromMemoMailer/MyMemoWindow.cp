/************************************************************************
 * $Id$
 * Copyright (c) 1998 Fort Gunnison, Inc.
 *
 * MyMemoWindow.cp: MyMemoWindowクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		May. 16, 1998
 * Last update:	May. 16, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "MyMemoWindow.h"
#include "MailWindow.h"
#include "MemoMailerMenu.h"
#include "MyTextView.h"


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/*
 * メッセージ応答; MyMemoWindow
 */
void
MyMemoWindow::MessageReceived (BMessage* message)
{
	switch (message->what) {
	case SEND_DOC:
		this->SendDocument();			break;
	default:
		MemoWindow::MessageReceived(message);
	}
	
	return;
}

void
MyMemoWindow::SendDocument (void)
{
	MailWindow*	mailWindow = new MailWindow(this);
	
	mailWindow->Show();
	
	return;
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/* MemoWindowクラスの公開メソッド */
/*
 * 初期化と解放; MemoWindow
 */
MyMemoWindow::MyMemoWindow (
		BRect frame, const char* inTitle, uint32 quitMsg)
	: MemoWindow(frame, inTitle, quitMsg)
{  /* do nothing */  }

MyMemoWindow::~MyMemoWindow (void)
{  /* do nothing */  }

/*
 * 表示テキストの取得
 */
const char*
MyMemoWindow::Text (void)
{
	return (this->GetTextView()->Text());
}


/*
 * End of File
 */
