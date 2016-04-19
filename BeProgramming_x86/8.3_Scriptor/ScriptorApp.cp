/************************************************************************
 * $Id$
 * Copyright (c) 1998 Fort Gunnison, Inc.
 *
 * ScriptorApp.cp: ScriptorAppクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 03, 1998
 * Last update:	Jan. 05, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "ScriptorApp.h"
#include "ScriptorView.h"
#include "GUI/MonoWindow.h"

#include <interface/Alert.h>
#include <interface/MenuBar.h>
#include <interface/MenuItem.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/* メッセージのコード */
enum {
	ABOUT_CLOSED	= 'abcl'
};


/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/* アバウトダイアログのメッセージ */
const char	kAboutMsg[]	= "Scriptor\n\n"
	"Copyright " B_UTF8_COPYRIGHT " 1998 Fort Gunnison, Inc.\n"
	"Author: Shin'ya Koga (koga@ftgun.co.jp)";


/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/* ScriptorAppクラスの非公開メソッド */
/*
 * 起動と終了; ScriptorApp
 */
void
ScriptorApp::ReadyToRun (void)
{
	RegularWindow*	myWindow;
	BMenuBar*		theMenuBar;
	BView*			theView;
	BMenu*			aMenu;

	/* ウィンドウを生成 */
	myWindow = new MonoWindow(
		BRect(50, 50, 260, 270),
		"Scriptor",
		B_TITLED_WINDOW,
		B_NOT_RESIZABLE|B_NOT_ZOOMABLE
	);
	
	/* メニューバーとビューを生成 */
	theMenuBar = new BMenuBar(BRect(0, 0, 0, 0), "");
	aMenu = new BMenu("File");
	theMenuBar->AddItem(aMenu);
	(void)aMenu->AddItem(
		new BMenuItem(  /* about項目 */
			"About Scriptor" B_UTF8_ELLIPSIS,
			new BMessage(B_ABOUT_REQUESTED)
		)
	);
	(void)aMenu->AddSeparatorItem();  /* -- */
	(void)aMenu->AddItem(
		new BMenuItem(  /* Quit項目 */
			"Quit",
			new BMessage(B_QUIT_REQUESTED),
			'Q'
		)
	);
	(void)aMenu->SetTargetForItems(this);  /* ターゲット設定 */
	theView = new ScriptorView(BRect(0, 0, 0, 0), B_FOLLOW_ALL_SIDES);
	
	/* ウィンドウを表示 */
	myWindow->InitContent(theMenuBar, theView);
	
	return;
}

/*
 * メニュー応答; ScriptorApp
 */
void
ScriptorApp::MessageReceived (BMessage* message)
{
	switch (message->what) {
	case ABOUT_CLOSED:
		fAboutBox = NULL;	break;
	default:
		BApplication::MessageReceived(message);
	}
	
	return;
}

void
ScriptorApp::AboutRequested (void)
{
	/* 既に開いていれば何もしない */
	if (fAboutBox != NULL)
		return;
	
	/* アバウトダイアログを生成 */
	fAboutBox = new BAlert("about box", kAboutMsg, "OK");

	/* 表示 */
	this->SetCursor(B_HAND_CURSOR);
	(void)fAboutBox->Go(
		new BInvoker(new BMessage(ABOUT_CLOSED), this)
	);
	
	return;
/*
 * 注意：アバウトメニューの直下にBTextControlなどのテキスト部品があった
 *		場合、カーソルの形状が i-beam になったままになる場合がある。この
 *		ため、ダイアログを表示する直前にSetCursor()メソッドを使ってカー
 *		ソル形状をリセットしている。('98. 1/5, koga@ftgun.co.jp)
 */
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * ScriptorAppクラスの公開メソッド
 */
ScriptorApp::ScriptorApp (void)
	: BApplication("application/x-vnd.FtGUN-Scriptor")
{
	fAboutBox  = NULL;
}

ScriptorApp::~ScriptorApp (void)
{  /* do nothing */  }


/*
 * End of File
 */
