/************************************************************************
 * $Id$
 * Copyright (c) 1997, Fort Gunnison, Inc.
 *
 * MonoWindow.cp: MonoWindowクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 24, 1997
 * Last update:	Dec. 24, 1997
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "MonoWindow.h"
#include "KGUtility/kgAlert.h"


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/* アラート用のメッセージ */
const char	kConfirmQuitMsg[]	= "If you close this window, "
	"the application will quit.\n"
	"Are you OK?";


/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/* MonoWindowクラスの公開メソッド */
/*
 * 初期化と解放; MonoWindow
 */
MonoWindow::MonoWindow (BRect frame, const char* title,
		window_type type, uint32 flags)
	: RegularWindow(frame, title, type, flags, B_QUIT_REQUESTED)
{  /* do nothing */  }

MonoWindow::~MonoWindow (void)
{  /* do nothing */  }

/*
 * 終了時処理; MonoWindow
 */
bool
MonoWindow::QuitRequested (void)
{
	bool		isOK;
	BMessage*	theMessage = this->CurrentMessage();
	
	/* アプリケーション全体を終了する場合は何もしない */
	if (theMessage == NULL || theMessage->what != B_QUIT_REQUESTED)
		return true;
	
	/* 終了してもよいかユーザに確認 */
	isOK = KGConfirm(kConfirmQuitMsg);
	
	return isOK;
}


/*
 * End of File
 */
