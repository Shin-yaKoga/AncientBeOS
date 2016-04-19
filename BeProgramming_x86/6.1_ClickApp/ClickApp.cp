/************************************************************************
 * $Id$
 * Copyright (c) 1997, Fort Gunnison, Inc.
 *
 * ClickApp.cp: ClickAppクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 12, 1997
 * Last update:	Dec. 12, 1997
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "ClickApp.h"
#include "ClickView.h"
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
 * ClickAppクラスの非公開メソッド
 */
void
ClickApp::ReadyToRun (void)
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
		new ClickView(
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
/*      #########################################################       */

/*
 * 初期化と解放; ClickApp
 */
ClickApp::ClickApp (void)
	: BApplication("application/x-vnd.FtGUN-ClickApp")
{  /* do nothing */  }

ClickApp::~ClickApp (void)
{  /* do nothing */  }


/*
 * End of File
 */
