/************************************************************************
 * $Id$
 * Copyright (c) 1998 Fort Gunnison, Inc.
 *
 * InfoChestApp.cp: InfoChestAppクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 24, 1998
 * Last update:	May. 02, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "InfoChestApp.h"
#include "Categorizer.h"
#include "DocumentsView.h"
#include "DocumentsWin.h"
#include "InfoChestView.h"
#include "InfoChestWin.h"
#include "SearchView.h"
#include "SearchWindow.h"
#include "GUI/EntryListView.h"  /* for OPEN_DOCUMENT */
#include "GUI/MenuUtil.h"
#include "KGUtility/kgDebug.h"

#include <interface/Alert.h>
#include <storage/VolumeRoster.h>
#include <support/Autolock.h>
#include <support/Debug.h>

#include <string.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/* メッセージのコード */
enum {
	ABOUT_CLOSED		= 'abcl',
	SHOW_SEARCH_PANEL	= 'find',
	SEARCH_PANEL_CLOSED	= 'fpcl',
	DOCS_WIN_CLOSED		= 'dwcl'
};


/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/* アバウトダイアログのメッセージ */
const char	kAboutMsg[]	= "InfoChest\n\n"
	"Copyright " B_UTF8_COPYRIGHT " 1998 Fort Gunnison, Inc.\n"
	"Author: Shin'ya Koga (koga@ftgun.co.jp)";

/* メインウィンドウ用のメニュー情報 */
static MItemInfo	sMainFileMenu[]	= {  /* Fileメニュー */
	{ "About InfoChest" B_UTF8_ELLIPSIS,	B_ABOUT_REQUESTED,	NULL },
	{ B_EMPTY_STRING,		0, NULL },
	{ "Find" B_UTF8_ELLIPSIS,	SHOW_SEARCH_PANEL,	'F' },
	{ B_EMPTY_STRING,		0, NULL },
	{ "Quit",				B_QUIT_REQUESTED,	'Q' },
	NULL
};
static MInfo	sMainMenuBar[]	= {  /* メニューバー */
	{ "File",	sMainFileMenu },
	NULL
};

/* 検索ウィンドウ用のメニュー項目 */
static MItemInfo	sSearchFileMenu[]	= {  /* Fileメニュー */
	{ "About InfoChest" B_UTF8_ELLIPSIS,	B_ABOUT_REQUESTED,	NULL },
	{ B_EMPTY_STRING,		0, NULL },
	{ "Open",				OPEN_DOCUMENT,		'O' },
	{ "Open Parent",		OPEN_PARENT,		NULL },
	{ B_EMPTY_STRING,		0, NULL },
	{ "Quit",					B_QUIT_REQUESTED,	'Q' },
	NULL
};
static MInfo	sSearchMenuBar[]	= {  /* メニューバー */
	{ "File",	sSearchFileMenu },
	NULL
};

/* ドキュメントウィンドウ用のメニュー項目 */
static MItemInfo	sDocFileMenu[]	= {  /* Fileメニュー */
	{ "About InfoChest" B_UTF8_ELLIPSIS,	B_ABOUT_REQUESTED,	NULL },
	{ B_EMPTY_STRING,			0, NULL },
	{ "Add Document" B_UTF8_ELLIPSIS,		ADD_DOCUMENT,		NULL },
	{ "Remove Document",		RMV_DOCUMENT,		NULL },
	{ B_EMPTY_STRING,			0, NULL },
	{ "Open",					OPEN_DOCUMENT,		'O' },
	{ "Open Parent",			OPEN_PARENT,		NULL },
	{ B_EMPTY_STRING,			0, NULL },
	{ "Find" B_UTF8_ELLIPSIS,				SHOW_SEARCH_PANEL,	'F' },
	{ B_EMPTY_STRING,			0 },
	{ "Quit",					B_QUIT_REQUESTED,	'Q' },
	NULL
};
static MInfo	sDocMenuBar[]	= {  /* メニューバー */
	{ "File",	sDocFileMenu },
	NULL
};


/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/* InfoChestAppクラスの非公開メソッド */
/*
 * 起動と終了; InfoChestApp
 */
void
InfoChestApp::ReadyToRun (void)
{
	status_t	sts;

	/* カテゴリ管理オブジェクトを生成 */
	sts = Categorizer::Startup();
	if (sts != B_OK)
		goto err;

	/* メインウィンドウを表示 */
	this->ShowMainWindow();
	
	/* ボリュームの監視を開始 */
	sts = fVolRoster->StartWatching();
	if (sts != B_OK)
		goto err;

	return;
err:
	::Error("InfoChestApp::ReadyToRun", sts);
	return;
}

void
InfoChestApp::Quit (void)
{
	/* カテゴリ管理オブジェクトを解放 */
	(void)Categorizer::Cleanup();
	BApplication::Quit();
	
	return;
}

/*
 * メッセージ処理; InfoChestApp
 */
void
InfoChestApp::MessageReceived (BMessage* message)
{
	switch (message->what) {
	/* アバウトダイアログから */
	case ABOUT_CLOSED:
		fAboutBox = NULL;					break;

	/* カテゴリウィンドウから */
	case SHOW_DOCUMENTS:
		this->ShowDocumentsOf(message);		break;
	case HIDE_DOCUMENTS:
		this->CategoryRemoved(message);		break;
		
	/* ドキュメントウィンドウから */
	case DOCS_WIN_CLOSED:
		this->DocWinClosed(message);		break;
	
	/* 検索ウィンドウから */
	case SEARCH_PANEL_CLOSED:
		fSearchWindow = NULL;				break;

	/* その他のウィンドウから */
	case SHOW_SEARCH_PANEL:
		this->ShowSearchPanel();			break;
		
	/* BVolumeRosterから */
	case B_NODE_MONITOR:
		this->VolumeChanged(message);		break;
	
	/* それ以外 */
	default:
		BApplication::MessageReceived(message);
	}
	
	return;
}

void
InfoChestApp::AboutRequested (void)
{
	/* 既に開いていたら何もしない */
	if (fAboutBox != NULL)
		return;
	
	/* アバウトダイアログを生成 */
	fAboutBox = new BAlert("abot box", kAboutMsg, "OK");
	
	/* ダイアログを表示 */
	(void)fAboutBox->Go(
		new BInvoker(new BMessage(ABOUT_CLOSED), this)
	);
	
	return;
}

void
InfoChestApp::ShowDocumentsOf (BMessage* message)
{
	status_t		sts;
	const char*		theCategory;
	RegularWindow*	theWindow;
	DocumentsView*	theView;
	BMenuBar*		theMenuBar;
	BMenu*			theFileMenu;
	float			minWidth, minHeight;
	float			maxWidth, maxHeight;
	
	/* 既に生成済みなら表示するだけ */
	sts = message->FindString(kArgCategory, &theCategory);
	if (sts != B_OK)
		goto err;
	theWindow = this->FindDocumentsWindow(theCategory);
	if (theWindow != NULL) {
		BAutolock	lock(theWindow);
	
		if (theWindow->IsHidden())
			theWindow->Show();
		theWindow->Activate();
		return;
	}
	
	/* ウィンドウを生成 */
	theWindow = new DocumentsWin(
		BRect(50, 50, 400, 210),
		theCategory,
		B_DOCUMENT_WINDOW,
		0,
		DOCS_WIN_CLOSED
	);
	
	/* メニューバーを生成 */
	theMenuBar = MenuUtil::MakeMenuBar(sDocMenuBar);
	
	/* ビューを生成 */
	theView = new DocumentsView(
		BRect(0, 0, 0, 0), B_FOLLOW_ALL_SIDES, theCategory
	);
	
	/* ウィンドウをセットアップ */
	theWindow->InitContent(theMenuBar, theView);
	theWindow->GetSizeLimits(
		&minWidth, &maxWidth, &minHeight, &maxHeight
	);
	theWindow->SetSizeLimits(130, maxWidth, 120, maxHeight);
	(void)fDocWindows->AddItem(theWindow);
	theView->SetDragEnabled(true);  /* xx */
	
	/* メニューのターゲットを設定 */
	theFileMenu = theMenuBar->SubmenuAt(0);
	theFileMenu->SetTargetForItems(this);
	MenuUtil::SetTargetOf(theFileMenu, ADD_DOCUMENT, theView);
	MenuUtil::SetTargetOf(theFileMenu, RMV_DOCUMENT, theView);
	MenuUtil::SetTargetOf(theFileMenu, OPEN_DOCUMENT, theView);
	MenuUtil::SetTargetOf(theFileMenu, OPEN_PARENT, theView);
	theView->AddAdjustableMenuItem(RMV_DOCUMENT);
	theView->AddAdjustableMenuItem(OPEN_DOCUMENT);
	theView->AddAdjustableMenuItem(OPEN_PARENT);
	
	return;
err:
	::Error("InfoChestApp::ShowDocumentsOf", sts);
	return;
}

void
InfoChestApp::ShowSearchPanel (void)
{
	status_t		sts;
	RegularWindow*	searchPanel;
	SearchView*		theView;
	BMenuBar*		theMenuBar;
	BMenu*			theFileMenu;
	float			minWidth, minHeight;
	float			maxWidth, maxHeight;
	
	/* 既に生成済みなら表示するだけ */
	if (fSearchWindow != NULL) {
		BAutolock	lock(fSearchWindow);
	
		if (fSearchWindow->IsHidden())
			fSearchWindow->Show();
		fSearchWindow->Activate();
		return;
	}
	
	/* ウィンドウを生成 */
	searchPanel = new SearchWindow(
		BRect(50, 50, 330, 210),
		"InfoChest:FileSearch",
		B_TITLED_WINDOW,
		0,
		SEARCH_PANEL_CLOSED
	);
	
	/* メニューバーを生成 */
	theMenuBar = MenuUtil::MakeMenuBar(sSearchMenuBar);
	
	/* ビューを生成 */
	theView = new SearchView(BRect(0, 0, 0, 0), B_FOLLOW_ALL_SIDES);
	
	/* ウィンドウをセットアップ */
	searchPanel->InitContent(theMenuBar, theView);
	searchPanel->GetSizeLimits(
		&minWidth, &maxWidth, &minHeight, &maxHeight);
	searchPanel->SetSizeLimits(250, maxWidth, 150, maxHeight);
	fSearchWindow = searchPanel;
	theView->SetDragEnabled(true);  /* xx */
	
	/* メニューのターゲットを設定 */
	theFileMenu = theMenuBar->SubmenuAt(0);
	theFileMenu->SetTargetForItems(this);
	MenuUtil::SetTargetOf(theFileMenu, OPEN_DOCUMENT, theView);
	MenuUtil::SetTargetOf(theFileMenu, OPEN_PARENT, theView);
	theView->AddAdjustableMenuItem(OPEN_DOCUMENT);
	theView->AddAdjustableMenuItem(OPEN_PARENT);
	
	return;
err:
	::Error("InfoChestApp::ShowSearchPanel", sts);
	return;
}

void
InfoChestApp::ShowMainWindow (void)
{
	InfoChestWin*	mainWindow;
	BView*			theView;
	BMenuBar*		theMenuBar;
	BMenu*			theFileMenu;
	float			minWidth, minHeight;
	float			maxWidth, maxHeight;
	
	/* 既に生成済みなら表示するだけ */
	if (fMainWindow != NULL) {
		BAutolock	lock(fMainWindow);
	
		if (fMainWindow->IsHidden())
			fMainWindow->Show();
		fMainWindow->Activate();
		return;
	}

	/* ウィンドウを生成 */
	mainWindow = new InfoChestWin(
		BRect(50, 50, 250, 220),
		"InfoChest",
		B_TITLED_WINDOW,
		B_NOT_CLOSABLE
	);
	
	/* メニューバーを生成 */
	theMenuBar = MenuUtil::MakeMenuBar(sMainMenuBar);
	
	/* コンテントビューを生成 */
	theView = new InfoChestView(BRect(0, 0, 0, 0), B_FOLLOW_ALL_SIDES);
	
	/* ウィンドウをセットアップ */
	mainWindow->InitContent(theMenuBar, theView);
	mainWindow->GetSizeLimits(
		&minWidth, &maxWidth, &minHeight, &maxHeight
	);
	mainWindow->SetSizeLimits(120, maxWidth, 170, maxHeight);
	fMainWindow = mainWindow;
	
	/* メニューのターゲットを設定 */
	theFileMenu = theMenuBar->SubmenuAt(0);
	theFileMenu->SetTargetForItems(this);
	
	return;
}

void
InfoChestApp::DocWinClosed (BMessage* message)
{
	status_t	sts;
	BWindow*	theSender;

	/* 送信元のウィンドウを取得 */
	sts = message->FindPointer(kSender, &theSender);
	if (sts != B_OK)
		goto err;
	
	/* ドキュメントウィンドウのリストから削除 */
	(void)fDocWindows->RemoveItem(theSender);
	
	return;
err:
	::Error("InfoChestApp::DocWinClosed", sts);
	return;
}

void
InfoChestApp::CategoryRemoved (BMessage* message)
{
	status_t		sts;
	const char*		theCategory;
	RegularWindow*	theWindow;

	/* 対応するウィンドウが開いていれば閉じる */
	sts = message->FindString(kArgCategory, &theCategory);
	if (sts != B_OK)
		goto err;
	theWindow = this->FindDocumentsWindow(theCategory);
	if (theWindow != NULL)
		theWindow->Close();
	
	return;
err:
	::Error("InfoChestApp::CategoryRemoved", sts);
	return;
}

void
InfoChestApp::VolumeChanged (BMessage* message)
{
	/* 検索ウィンドウに通知 */
	if (fSearchWindow != NULL)
		fSearchWindow->PostMessage(message);
	
	/* ドキュメントウィンドウに通知 */
	for (int32 i = 0, n = fDocWindows->CountItems(); i < n; ++i) {
		DocumentsWin*	aWindow = (DocumentsWin*)fDocWindows->ItemAt(i);
		
		aWindow->PostMessage(message);
	}
	
	return;
}

/*
 * ビュー部品の管理; InfoChestApp
 */
DocumentsWin*
InfoChestApp::FindDocumentsWindow (const char* inCategory)
{
	DocumentsWin*	theWindow = NULL;
	
	for (int32 i = 0, n = fDocWindows->CountItems(); i < n; ++i) {
		DocumentsWin*	aWindow = (DocumentsWin*)fDocWindows->ItemAt(i);
		
		if (strcmp(aWindow->Title(), inCategory) == 0) {
			theWindow = aWindow;
			break;
		}
	}
	
	return theWindow;
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * InfoChestAppクラスの公開メソッド
 */
InfoChestApp::InfoChestApp (void)
	: BApplication("application/x-vnd.FtGUN-InfoChest")
{
	fDocWindows   = new BList();
	fVolRoster    = new BVolumeRoster();
	fAboutBox     = NULL;
	fSearchWindow = NULL;
	fMainWindow   = NULL;
}

InfoChestApp::~InfoChestApp (void)
{
	delete fDocWindows;
	delete fVolRoster;
}


/*
 * End of File
 */
