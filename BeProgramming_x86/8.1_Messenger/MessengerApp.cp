/************************************************************************
 * $Id$
 * Copyright (c) 1997-1998 Fort Gunnison, Inc.
 *
 * MessengerApp.cp: MessengerAppクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 24, 1997
 * Last update:	Apr. 22, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "MessengerApp.h"
#include "MessengerView.h"
#include "GUI/MonoWindow.h"

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
const char	kAboutMsg[] = "Messenger\n\n"
	"Copyright " B_UTF8_COPYRIGHT " 1997-1998 Fort Gunnison, Inc.\n"
	"Author: Shin'ya Koga (koga@ftgun.co.jp)";


/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/* MessengerAppクラスの非公開メソッド */
/*
 * 起動と終了; MessengerApp
 */
void
MessengerApp::ReadyToRun (void)
{
	RegularWindow*	myWindow;
	BMenuBar*		theMenuBar;
	BView*			theView;
	BMenu*			aMenu;
	float			minWidth, minHeight;
	float			maxWidth, maxHeight;

	/* ウィンドウを生成 */
	myWindow = new MonoWindow(
		BRect(50, 50, 250, 200),
		"Messenger",
		B_TITLED_WINDOW,
		0
	);
	
	/* メニューバーとビューを生成 */
	theMenuBar = new BMenuBar(BRect(0, 0, 0, 0), "");
	aMenu = new BMenu("File");
	theMenuBar->AddItem(aMenu);
	(void)aMenu->AddItem(
		new BMenuItem(  /* about項目 */
			"About Messenger" B_UTF8_ELLIPSIS,
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
	theView = new MessengerView(BRect(0, 0, 0, 0), B_FOLLOW_ALL_SIDES);

	/* ウィンドウを表示 */
	myWindow->InitContent(theMenuBar, theView);
	myWindow->GetSizeLimits(&minWidth, &maxWidth, &minHeight, &maxHeight);
	myWindow->SetSizeLimits(190, maxWidth, 130, maxHeight);
	
	return;
}

/*
 * メニュー応答; MessengerApp
 */
void
MessengerApp::AboutRequested (void)
{
	BAlert*	alertPanel;
	
	alertPanel = new BAlert("about box", kAboutMsg, "OK");
	(void)alertPanel->Go(NULL);
	
	return;
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * MessengerAppクラスの公開メソッド
 */
MessengerApp::MessengerApp (void)
	: BApplication("application/x-vnd.FtGUN-Messenger")
{  /* do nothing */  }

MessengerApp::~MessengerApp (void)
{  /* do nothing */  }


/*
 * End of File
 */
