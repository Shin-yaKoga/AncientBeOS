/************************************************************************
 * $Id$
 * Copyright (c) 1997-1998 Fort Gunnison, Inc.
 *
 * ClockworkWaveApp.cp: ClockworkWaveAppクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 21, 1997
 * Last update:	Feb. 13, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "ClockworkWaveApp.h"
#include "ClockView.h"
#include "WaveView.h"
#include "TimeKeeper.h"
#include "GUI/RegularWindow.h"
#include "KGUtility/kgDebug.h"

#include <interface/Alert.h>
#include <interface/MenuBar.h>
#include <interface/MenuItem.h>
#include <support/Autolock.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/* メッセージのコード */
enum {
	WINDOW_CLOSED	= 'wcld',
	SHOW_CLOCK		= 'sclk',
	SHOW_WAVE		= 'swav'
};


/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/* アバウトダイアログのメッセージ */
const char	kAboutMsg[]	= "ClockworkWave\n\n"
	"Copyright " B_UTF8_COPYRIGHT " 1997-1998 Fort Gunnison, Inc.\n"
	"Author: Shin'ya Koga (koga@ftgun.co.jp)";


/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/* ClockworkWaveAppクラスの非公開メソッド */
/*
 * 起動と終了; ClockworkWaveApp
 */
void
ClockworkWaveApp::ReadyToRun (void)
{
	/* TimeKeeperオブジェクトを生成 */
	fTimeKeeper = new TimeKeeper();
	
	/* 二つのウィンドウを生成して表示 */
	this->MakeClockWin();
	this->MakeWaveWin();
	
	return;
}

void
ClockworkWaveApp::Quit (void)
{
	/* TimeKeeperオブジェクトを解放 */
	delete fTimeKeeper;
	
	/* 親クラスのメソッドを実行 */
	BApplication::Quit();
	
	return;
}

/*
 * ビュー部品の生成; ClockworkWaveApp
 */
void
ClockworkWaveApp::MakeClockWin (void)
{
	BMenuBar*	aMenuBar;
	BView*		aView;
	
	/* 時計を表示するウィンドウを生成して表示 */
	fClockWin = new RegularWindow(
		BRect(50, 50, 200, 140),
		"Clock Window",
		B_TITLED_WINDOW,
		0,
		WINDOW_CLOSED
	);
	aMenuBar = this->MakeMenuBar(true);
	aView = new ClockView(
		BRect(0, 0, 0, 0), B_FOLLOW_ALL_SIDES, fTimeKeeper
	);
	aView->SetViewColor(0xD0, 0xD0, 0xD0);
	fClockWin->InitContent(aMenuBar, aView);
	
	return;
}

void
ClockworkWaveApp::MakeWaveWin (void)
{
	BMenuBar*	aMenuBar;
	BView*		aView;

	/* 水面を表示するウィンドウを生成して表示 */
	fWaveWin  = new RegularWindow(
		BRect(250, 200, 450, 400),
		"Wave Window",
		B_TITLED_WINDOW,
		B_NOT_RESIZABLE|B_NOT_ZOOMABLE,
		WINDOW_CLOSED
	);
	aMenuBar = this->MakeMenuBar(false);
	aView = new WaveView(
		BRect(0, 0, 0, 0), B_FOLLOW_ALL_SIDES, fTimeKeeper
	);
	fWaveWin->InitContent(aMenuBar, aView);

	return;
}

BMenuBar*
ClockworkWaveApp::MakeMenuBar (bool forClock)
{
	BMenuBar*	theMenuBar;
	BMenu*		theFileMenu;
	
	/* メニューバーと「File」メニューを生成 */
	theMenuBar = new BMenuBar(BRect(0, 0, 0, 0), B_EMPTY_STRING);
	theFileMenu = new BMenu("File");
	theMenuBar->AddItem(theFileMenu);
	
	/* about項目 */
	(void)theFileMenu->AddItem(
		new BMenuItem(
			"About ClockworkWave" B_UTF8_ELLIPSIS,
			new BMessage(B_ABOUT_REQUESTED)
		)
	);
	
	/* -- */
	(void)theFileMenu->AddSeparatorItem();
	
	/* ウィンドウ表示項目 */
	if (forClock) {
		(void)theFileMenu->AddItem(
			new BMenuItem(
				"Show Wave",
				new BMessage(SHOW_WAVE)
			)
		);
	} else {
		(void)theFileMenu->AddItem(
			new BMenuItem(
				"Show Clock",
				new BMessage(SHOW_CLOCK)
			)
		);
	}
	
	/* -- */
	(void)theFileMenu->AddSeparatorItem();
	
	/* Quit項目 */
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

/*
 * メニュー応答; ClockworkWaveApp
 */
void
ClockworkWaveApp::MessageReceived (BMessage* message)
{
	switch (message->what) {
	case WINDOW_CLOSED:
		this->CheckWindow(message);		break;
	case SHOW_CLOCK:
		this->ShowClock();				break;
	case SHOW_WAVE:
		this->ShowWave();				break;
	default:
		BApplication::MessageReceived(message);
	}
	
	return;
}

void
ClockworkWaveApp::AboutRequested (void)
{
	BAlert*	alertPanel;
	
	alertPanel = new BAlert("about box", kAboutMsg, "OK");
	(void)alertPanel->Go(NULL);
	
	return;
}

void
ClockworkWaveApp::CheckWindow (BMessage* message)
{
	status_t	sts;
	BWindow*	sender;

	/* 終了したウィンドウを記録 */	
	sts = message->FindPointer(kSender, &sender);
	if (sts != B_OK)
		goto err;
	if (sender == fClockWin)
		fClockWin = NULL;
	else if (sender == fWaveWin)
		fWaveWin = NULL;
	else {
		sts = B_ERROR;
		goto err;
	}
	
	/* 全てのウィンドウが閉じられた場合は終了 */
	if (fClockWin == NULL && fWaveWin == NULL)
		(void)this->PostMessage(B_QUIT_REQUESTED);
	
	return;
err:
	::Error("ClockworkWaveApp::CheckWindow", sts);
	return;
}

void
ClockworkWaveApp::ShowClock (void)
{
	if (fClockWin == NULL)
		this->MakeClockWin();
	else
		this->ShowWindow(fClockWin);
	
	return;
}

void
ClockworkWaveApp::ShowWave (void)
{
	if (fWaveWin == NULL)
		this->MakeWaveWin();
	else
		this->ShowWindow(fWaveWin);
	
	return;
}

/*
 * ウィンドウ操作; ClockworkWaveApp
 */
void
ClockworkWaveApp::ShowWindow (BWindow* inWindow)
{
	BAutolock	lock(inWindow);
	
	if (inWindow->IsHidden())
		inWindow->Show();
	else if (! inWindow->IsActive())
		inWindow->Activate();
	
	return;
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * ClockworkWaveAppクラスの公開メソッド
 */
ClockworkWaveApp::ClockworkWaveApp (void)
	: BApplication("application/x-vnd.FtGUN-ClockworkWave")
{
	fClockWin = fWaveWin = NULL;
	fTimeKeeper = NULL;
}

ClockworkWaveApp::~ClockworkWaveApp (void)
{  /* do nothing */  }


/*
 * End of File
 */
