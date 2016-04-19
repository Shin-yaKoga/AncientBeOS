/************************************************************************
 * $Id$
 * Copyright (c) 1997, Fort Gunnison, Inc.
 *
 * SimpleApp.cp: SimpleAppクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 10, 1997
 * Last update:	Dec. 10, 1997
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "SimpleApp.h"
#include "SimpleWin.h"


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
 * SimpleAppクラスの非公開メソッド
 */
void
SimpleApp::ReadyToRun (void)
{
	BWindow*	windowObj;
	
	/* SimpleWindowオブジェクトを生成 */
	windowObj = new SimplWin(  /* modified at '98. 1/13 */
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
 * SimpleAppクラスの公開メソッド
 */
SimpleApp::SimpleApp (void)
	: BApplication("application/x-vnd.FtGUN-SimpleApp")
{  /* do nothing */  }

SimpleApp::~SimpleApp (void)
{  /* do nothing */  }


/*
 * End of File
 */
