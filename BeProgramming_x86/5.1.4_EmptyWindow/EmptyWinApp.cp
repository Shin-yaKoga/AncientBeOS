/************************************************************************
 * $Id$
 * Copyright (c) 1997, Fort Gunnison, Inc.
 *
 * EmptyWinApp.cp: EmptyWinAppクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 10, 1997
 * Last update:	Dec. 10, 1997
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "EmptyWinApp.h"
#include <interface/Window.h>


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
 * EmptyWinAppクラスの非公開メソッド
 */
void
EmptyWinApp::ReadyToRun (void)
{
	BWindow*	windowObj;
	
	/* BWindowオブジェクトを生成 */
	windowObj = new BWindow(
		BRect(50, 50, 200, 120),
		"My Window",
		B_TITLED_WINDOW,
		0
	);

	/* 画面に表示 */	
	windowObj->Show();
	
	return;
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * EmptyWinAppクラスの公開メソッド
 */
EmptyWinApp::EmptyWinApp (void)
	: BApplication("application/x-vnd.FtGUN-EmptyWindow")
{  /* do nothing */  }

EmptyWinApp::~EmptyWinApp (void)
{  /* do nothing */  }


/*
 * End of File
 */
