/************************************************************************
 * $Id$
 * Copyright (c) 1997, Fort Gunnison, Inc.
 *
 * SimpleWin.cp: SimpleWinクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 10, 1997
 * Last update:	Dec. 12, 1997
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "SimpleWin.h"
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
	viewObj = new BView(
		this->Bounds(),
		"content",
		B_FOLLOW_ALL_SIDES,
		0);
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
 * 描画処理; SimpleWin
 */
void
SimpleWin::Show (void)
{
	BView*		viewObj   = this->FindView("content");
	rgb_color	currColor = viewObj->HighColor();

	/* 親クラスのメソッドを実行 */
	BWindow::Show();
	::snooze(40 * 1000);  /* wait for unvealing the view */
	
	/* 文字列と図形を描画 */
	this->Lock();  /* 自身をロック */
	viewObj->SetHighColor(0xD0, 0xD0, 0xD0);
	viewObj->FillRect(viewObj->Frame());
	viewObj->SetHighColor(currColor);
	viewObj->DrawString("This is the my first app.", BPoint(10, 10));
	viewObj->SetHighColor(255, 0, 0);
	viewObj->FillRect(BRect(110, 30, 140, 60));
	this->Unlock();  /* ロック解除 */
	this->Flush();  /* flushing the draw instruction */

	return;
}


/*
 * End of File
 */
