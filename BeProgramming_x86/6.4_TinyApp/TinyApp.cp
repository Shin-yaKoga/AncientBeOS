/************************************************************************
 * $Id$
 * Copyright (c) 1997-1998 Fort Gunnison, Inc.
 *
 * TinyApp.cp: TinyAppクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 14, 1997
 * Last update:	Feb. 12, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "TinyApp.h"
#include "ButtonView.h"
#include "SmartWindow.h"
#include <interface/Alert.h>
#include <interface/MenuBar.h>
#include <interface/MenuItem.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/* アバウトダイアログのメッセージ */
const char	kAboutMsg[] = "TinyApp\n\n"
	"Copyright " B_UTF8_COPYRIGHT " 1997-1998 Fort Gunnison, Inc.\n"
	"Author: Shin'ya Koga (koga@ftgun.co.jp)";


/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/* TinyAppクラスの非公開メソッド */
/*
 * 起動と終了; TinyApp
 */
void
TinyApp::ReadyToRun (void)
{
	SmartWindow*	windowObj;
	BMenuBar*		menuBar;
	float			minWidth, minHeight;
	float			maxWidth, maxHeight;

	
	/* ウィンドウオブジェクトを生成 */
	windowObj = new SmartWindow(
		BRect(50, 50, 200, 140),
		"My Window",
		B_TITLED_WINDOW,
		0
	);
	
	/* メニューバーを生成 */
	menuBar = this->MakeMenuBar();
	
	/* ウィンドウの表示内容を設定して表示 */
	windowObj->InitContent(
		menuBar,
		new ButtonView(
			BRect(0, 0, 0, 0),
			"content",
			B_FOLLOW_ALL_SIDES,
			B_WILL_DRAW
		)
	);
	
	/* ウィンドウの最大サイズと最小サイズを調節 */
	windowObj->GetSizeLimits(
		&minWidth, &maxWidth, &minHeight, &maxHeight);
	windowObj->SetSizeLimits(
		140, maxWidth, 85, maxHeight);
	
	return;
}

/*
 * メニューの設定と応答; TinyApp
 */
BMenuBar*
TinyApp::MakeMenuBar (void)
{
	BMenuBar*	theMenuBar;
	BMenu*		theFileMenu;

	/* メニューバーとメニューを生成 */	
	theMenuBar  = new BMenuBar(BRect(0, 0, 0, 0), B_EMPTY_STRING);
	theFileMenu = new BMenu("File");
	theMenuBar->AddItem(theFileMenu);

	/* About項目を追加 */
	(void)theFileMenu->AddItem(
		new BMenuItem(
			"About TinyApp" B_UTF8_ELLIPSIS,
			new BMessage(B_ABOUT_REQUESTED)
		)
	);

	/* セパレータを追加 */
	(void)theFileMenu->AddSeparatorItem();
	
	/* Quit項目を追加 */
	(void)theFileMenu->AddItem(
		new BMenuItem(
			"Quit",
			new BMessage(B_QUIT_REQUESTED),
			'Q'
		)
	);
	
	/* メニューのターゲットを設定 */
	theFileMenu->SetTargetForItems(this);
	
	return theMenuBar;
}

void
TinyApp::AboutRequested (void)
{
	BAlert*	alertPanel;
	
	alertPanel = new BAlert(
		"about box", kAboutMsg, "OK"
	);
	(void)alertPanel->Go(NULL);
	
	return;
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * TinyAppクラスの公開メソッド
 */
TinyApp::TinyApp (void)
	: BApplication("application/x-vnd.FtGUN-TinyApp")
{  /* do nothing */  }

TinyApp::~TinyApp (void)
{  /* do nothing */  }


/*
 * End of File
 */
