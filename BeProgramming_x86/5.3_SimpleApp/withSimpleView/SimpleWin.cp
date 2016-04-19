/************************************************************************
 * $Id$
 * Copyright (c) 1997, Fort Gunnison, Inc.
 *
 * SimpleWin.cp: SimpleWinクラスの実装(改訂版)
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 10, 1997
 * Last update:	Dec. 11, 1997
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "SimpleWin.h"
#include "SimpleView.h"
#include <app/Application.h>


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

/* SimpleWinクラスの公開メソッド */
/*
 * 初期化と解放; SimpleWin
 */
SimpleWin::SimpleWin (BRect frame, const char* title,
		window_type type, uint32 flags, uint32 workspace)
	: BWindow(frame, title, type, flags, workspace)
{
	BView*	viewObj;

	/* BViewオブジェクトを生成して自身に貼り付ける */
	viewObj = new SimpleView(
		this->Bounds(),
		"content",
		B_FOLLOW_ALL_SIDES,
		B_WILL_DRAW);
	this->AddChild(viewObj);
}

SimpleWin::~SimpleWin (void)
{  /* do nothing */  }

/*
 * 終了時処理; SimpleWin
 */
void
SimpleWin::Quit (void)
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
