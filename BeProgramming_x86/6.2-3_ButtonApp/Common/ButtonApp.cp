/************************************************************************
 * $Id$
 * Copyright (c) 1997, Fort Gunnison, Inc.
 *
 * ButtonApp.cp: ButtonAppクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 12, 1997
 * Last update:	Dec. 12, 1997
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "ButtonApp.h"
#include "ButtonView.h"
#include "SingleWindow.h"


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
 * ButtonAppクラスの非公開メソッド
 */
void
ButtonApp::ReadyToRun (void)
{
	SingleWindow*	windowObj;
	
	/* SingleWindowオブジェクトを生成 */
	windowObj = new SingleWindow(
		BRect(50, 50, 200, 120),
		"My Window",
		B_TITLED_WINDOW,
		0
	);
	windowObj->InitContent(
		new ButtonView(
			BRect(0, 0, 0, 0),
			"content",
			B_FOLLOW_ALL_SIDES,
			B_WILL_DRAW
		)
	);
	
	/* 画面に表示 */
	windowObj->Show();
	
	return;
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################		*/

/*
 * ButtonAppクラスの公開メソッド
 */
ButtonApp::ButtonApp (void)
	: BApplication("application/x-vnd.FtGUN-ButtonApp")
{  /* do nothing */  }

ButtonApp::~ButtonApp (void)
{  /* do nothing */  }


/*
 * End of File
 */
