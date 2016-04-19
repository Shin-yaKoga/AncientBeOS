/************************************************************************
 * $Id$
 * Copyright (c) 1998 Fort Gunnison, Inc.
 *
 * FileSearchApp.cp: FileSearchAppクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 15, 1998
 * Last update:	Apr. 29, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "FileSearchApp.h"
#include "SearchView.h"
#include "SearchWindow.h"
#include "GUI/EntryListView.h"
#include "GUI/MenuUtil.h"
#include "GUI/MonoWindow.h"
#include "KGUtility/kgDebug.h"

#include <interface/Alert.h>
#include <storage/VolumeRoster.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/* メッセージコード */
enum {
	ABOUT_CLOSED	= 'abcl'
};


/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/* アバウトダイアログのメッセージ */
const char	kAboutMsg[]	= "FileSearch\n\n"
	"Copyright " B_UTF8_COPYRIGHT " 1998 Fort Gunnison, Inc.\n"
	"Author: Shin'ya Koga (koga@ftgun.co.jp)";

/* メニュー情報 */
static MItemInfo	sFileMenu[]	= {  /* Fileメニュー */
	{ "About FileSearch" B_UTF8_ELLIPSIS,	B_ABOUT_REQUESTED,	NULL },
	{ B_EMPTY_STRING,			0, NULL },
	{ "Open",					OPEN_DOCUMENT,		'O' },
	{ "Open Parent",			OPEN_PARENT,		NULL },
	{ B_EMPTY_STRING,			0, NULL },
	{ "Quit",					B_QUIT_REQUESTED,	'Q' },
	NULL
};

static MInfo	sMyMenuBar[]	= {  /* メニューバー */
	{ "File",	sFileMenu },
	NULL
};


/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/* FileSearchAppクラスの非公開メソッド */
/*
 * 初期化と解放; FileSearchApp
 */
void
FileSearchApp::ReadyToRun (void)
{
	status_t		sts;
	BMessenger		theMessenger(this, NULL, &sts);
	RegularWindow*	myWindow;
	BMenuBar*		theMenuBar;
	BMenu*			theMenu;
	SearchView*		theView;
	float			minWidth, minHeight;
	float			maxWidth, maxHeight;
	
	/* ボリュームの監視を開始 */
	if (sts != B_OK)
		goto err;  /* BMessengerの初期化に失敗 */
	sts = fVolRoster->StartWatching(theMessenger);
	if (sts != B_OK)
		goto err;
	
	/* ウィンドウを生成 */
	myWindow = new SearchWindow(
		BRect(50, 50, 330, 210),
		"FileSearch",
		B_TITLED_WINDOW,
		0
	);
	
	/* メニューバーを生成 */
	theMenuBar = MenuUtil::MakeMenuBar(sMyMenuBar);

	/* メインビューを生成 */
	theView = new SearchView(BRect(0, 0, 0, 0), B_FOLLOW_ALL_SIDES);
	
	/* ウィンドウをセットアップ */
	myWindow->InitContent(theMenuBar, theView);
	myWindow->GetSizeLimits(&minWidth, &maxWidth, &minHeight, &maxHeight);
	myWindow->SetSizeLimits(250, maxWidth, 150, maxHeight);
	fMainWindow = myWindow;

	/* メニューのターゲットを設定 */
	theMenu = theMenuBar->SubmenuAt(0);
	theMenu->SetTargetForItems(this);
	MenuUtil::SetTargetOf(theMenu, OPEN_DOCUMENT, theView);
	MenuUtil::SetTargetOf(theMenu, OPEN_PARENT, theView);
	theView->AddAdjustableMenuItem(OPEN_DOCUMENT);
	theView->AddAdjustableMenuItem(OPEN_PARENT);
	
	return;
err:
	::Error("FileSearchApp::ReadyToRun", sts);
	return;
/*
 * 注意：ここでは、BVolumeRosterオブジェクトのStartWatching()メソッドに
 *		BMessengerオブジェクトを渡しているが、これは必ずしも必要ではな
 *		い。StartWatching()の引数はオプションであり、何も指定しない場合
 *		はbe_app_messenger、つまりアプリケーションオブジェクトに対する
 *		BMessengerオブジェクトが渡されるからである。
 *		('98. 5/1, koga@ftgun.co.jp)
 */
}

/*
 * メッセージ処理; FileSearchApp
 */
void
FileSearchApp::MessageReceived (BMessage* message)
{
	switch (message->what) {
	case ABOUT_CLOSED:
		fAboutBox = NULL;					break;
	case B_NODE_MONITOR:
		fMainWindow->PostMessage(message);	break;
	default:
		BApplication::MessageReceived(message);
	}
	
	return;
/*
 * 注意：BVolumeRosterオブジェクトから送られてくるB_NODE_MONITORメッ
 *		セージは、単にウィンドウ(SearchWindow)オブジェクトを経由して
 *		SearchViewオブジェクトに転送しているだけである。本当は、
 *		BVolumeRosterをSearchViewに持たせれば、無駄なメッセージ転送
 *		がなくなって良いのだが、ここではそうしていない。"InfoChest"
 *		アプリケーションのように複数枚のウィンドウを持つ場合には、
 *		アプリケーションが代表してメッセージを受け取り、そこから他の
 *		ウィンドウへ転送するのが自然であり、そのスタイルに合わせてい
 *		るためである。('98. 5/1, koga@ftgun.co.jp)
 */
}

void
FileSearchApp::AboutRequested (void)
{
	/* 既に開いていたら何もしない */
	if (fAboutBox != NULL)
		return;
	
	/* アバウトダイアログを生成 */
	fAboutBox = new BAlert("about box", kAboutMsg, "OK");
	
	/* ダイアログを表示 */
	(void)fAboutBox->Go(
		new BInvoker(new BMessage(ABOUT_CLOSED), this)
	);
	
	return;
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * FileSearchAppクラスの公開メソッド
 */
FileSearchApp::FileSearchApp (void)
	: BApplication("application/x-vnd.FtGUN-FileSearch")
{
	fAboutBox  = NULL;
	fVolRoster = new BVolumeRoster();
}

FileSearchApp::~FileSearchApp (void)
{
	delete fVolRoster;
}


/*
 * End of File
 */
