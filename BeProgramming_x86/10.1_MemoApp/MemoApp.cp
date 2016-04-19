/************************************************************************
 * $Id$
 * Copyright (c) 1998 Fort Gunnison, Inc.
 *
 * MemoApp.cp: MemoAppクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		May. 04, 1998
 * Last update:	May. 04, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "MemoApp.h"
#include "MemoAppMenu.h"
#include "MemoWindow.h"
#include "KGUtility/kgDebug.h"

#include <interface/Alert.h>
#include <interface/MenuBar.h>
#include <interface/MenuItem.h>
#include <storage/FilePanel.h>
#include <storage/NodeInfo.h>

#include <stdio.h>
#include <string.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/* メッセージのコード */
enum {
	ABOUT_CLOSED	= 'abcl',
	DOC_CLOSED		= 'dcls'
};

/* その他の定数 */
const BRect	kDefaultDocFrame(100, 100, 370, 300);

/* テキストファイル用のフィルタクラス */
class MyTextRefFilter : public BRefFilter {
public:
	bool	Filter(const entry_ref*, BNode*, struct stat*,
					   const char* mimetype);
};


/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/* アバウトダイアログのメッセージ */
const char	kAboutMsg[] = "MemoApp\n\n"
	"Copyright " B_UTF8_COPYRIGHT " 1998 Fort Gunnison, Inc.\n"
	"Author: Shin'ya Koga (koga@ftgun.co.jp)";

const char kMyAppSig[]	= "application/x-vnd.FtGUN-MemoApp";


/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/*
 * MyTextRefFilterのフィルタ関数;
 */
bool
MyTextRefFilter::Filter (const entry_ref* ref, BNode* /* node */,
	struct stat* /* st */, const char* mimetype)
{
	BEntry		theEntry(ref, true);  /* シンボリックリンクを解決 */
	BMimeType	theType(mimetype);
	entry_ref	orgRef;

	/* ディレクトリは通す */	
	if (theEntry.IsDirectory())
		return true;

	/* シンボリックリンクが指す先のファイルタイプ情報を取得 */
//	(void)theEntry.GetRef(&orgRef);
	if (theEntry.GetRef(&orgRef) != B_OK) {  /* for R3's symbolick link */
		orgRef = *ref;
	}
	if (orgRef != *ref) {
		char		orgType[B_MIME_TYPE_LENGTH];
		BNode		orgNode(&orgRef);
		BNodeInfo	orgNodeInfo(&orgNode);
		
		(void)orgNodeInfo.GetType(orgType);
		(void)theType.SetTo(orgType);
	}

	/* "text"タイプのファイルは全て通す */
	if (strcmp(theType.Type(), "text") == 0)
		return true;
	else {
		BMimeType	theSuperType;
		
		if (theType.GetSupertype(&theSuperType) == B_OK
				&& strcmp(theSuperType.Type(), "text") == 0)
			return true;
	}
	
	return false;
}


/* MemoAppクラスの非公開メソッド */
/*
 * 起動・終了時処理; MemoApp
 */
void
MemoApp::ReadyToRun (void)
{
	/* 必要なら新規ウィンドウを開く */
	if (fNumDocWindow == 0) {
		this->ConfigureFontMenu();
		this->NewDocument();
	}

	return;
/*
 * 注意：アプリケーションのアイコンにファイルをドラッグ＆ドロップして起
 *		動した場合、RefsReceived()メソッドの方が先に呼び出される。した
 *		がって、それによるウィンドウが開いていない場合に限って新規ウィ
 *		ンドウを開くようにしている。
 */
}

/*
 * メッセージ応答; MemoApp
 */
void
MemoApp::MessageReceived (BMessage* message)
{
	switch (message->what) {
	case NEW_DOC:	/* New */
		this->NewDocument();		break;
	case OPEN_DOC:	/* Open... */
		this->ShowOpenPanel();		break;
	case ABOUT_CLOSED:	/* アバウトダイアログが閉じた */
		fAboutBox = NULL;			break;
	case DOC_CLOSED:	/* ドキュメントウィンドウが閉じた */
		this->DocumentClosed();		break;
	default:
		BApplication::MessageReceived(message);
	}
	
	return;
}

void
MemoApp::RefsReceived (BMessage* message)
{
	status_t	sts;
	entry_ref	theRef;
	BEntry		entryObj;
	
	/* 必要ならフォントメニューを作成 */
	if (fFontMenuArchive == NULL)
		this->ConfigureFontMenu();
	
	for (int32 i = 0;; ++i) {
		sts = message->FindRef("refs", i, &theRef);
		if (sts != B_OK)
			break;
		(void)entryObj.SetTo(&theRef, true);  /* シンボリックリンクを解決 */
		if (entryObj.IsFile()) {
			(void)entryObj.GetRef(&theRef);  /* シンボリックリンク対策 */
			this->OpenDocument(theRef);
		}
	}
	
	return;
}

void
MemoApp::AboutRequested (void)
{
	if (fAboutBox != NULL)
		return;
	
	fAboutBox = new BAlert("about box", kAboutMsg, "OK");
	(void)fAboutBox->Go(
		new BInvoker(new BMessage(ABOUT_CLOSED), this)
	);
	
	return;
}

/*
 * ドキュメントウィンドウの管理; MemoApp
 */
MemoWindow*
MemoApp::NewDocument (bool doShow)
{
	MemoWindow*	newDocWin;
	char		docName[32+1];
	BMenuBar*	theMenuBar = MemoAppMenu::MakeDocMenuBar();
	BMenu*		theFontMenu;

	if (doShow) {
		/* 新規ウィンドウのタイトルは"Untitled xx"形式 */	
		sprintf(docName, "Untitled %d", ++fCreatedDocs);
		newDocWin = new MemoWindow(kDefaultDocFrame, docName, DOC_CLOSED);
	} else
		newDocWin = new MemoWindow(
			kDefaultDocFrame, B_EMPTY_STRING, DOC_CLOSED);

	/* ウィンドウにメニューバーをセットして表示 */
	theFontMenu = (BMenu*)BMenu::Instantiate(fFontMenuArchive);
	theMenuBar->AddItem(theFontMenu, 2);
	newDocWin->InitContent(theMenuBar, doShow);
	fNumDocWindow += 1;

	return newDocWin;
/*
 * 注意：ここでは、フォントメニューとそれ以外のメニューを別に作成してい
 *		る。これは、両者の作成方法が異なるためである。つまり、フォント
 *		メニュー以外のものは項目が固定されており、リソース化が可能であ
 *		る。これに対し、フォントメニューの項目はアプリケーションの起動
 *		時にシステムから取得しており、動的に構築される。
 *		('98. 5/11, koga@ftgun.co.jp)
 */
}

void
MemoApp::OpenDocument (const entry_ref& inRef)
{
	MemoWindow*	newDocWin;
	entry_ref	aRef;
	
	/* 既に同じファイルを開いていないかチェック */
	for (int32 i = 0, n = this->CountWindows(); i < n; ++i) {
		MemoWindow*	theWindow = cast_as(this->WindowAt(i), MemoWindow);
		
		if (theWindow != NULL) {
			if (theWindow->GetEntryRef(&aRef)) {
				if (aRef == inRef) {
					theWindow->Activate();
					return;
				}
			}
		}
	}

	/* ウィンドウを作ってファイルをロード */
	newDocWin = this->NewDocument(false);
	newDocWin->LoadFile(inRef);
	
	return;
}

void
MemoApp::ShowOpenPanel (void)
{
	/* 必要ならオープンダイアログを生成 */
	if (fOpenPanel == NULL) {
		fOpenPanel = new BFilePanel(B_OPEN_PANEL);
		fRefFilter = new MyTextRefFilter();
		fOpenPanel->SetRefFilter(fRefFilter);
	}
	
	/* 既に表示している場合は最前面に出すだけ */
	if (! fOpenPanel->IsShowing())
		fOpenPanel->Show();  /* 表示 */
	else
		fOpenPanel->Window()->Activate();  /* 最前面へ */
	
	return;
}

void
MemoApp::DocumentClosed (void)
{
	/* 全てのドキュメントウィンドウが閉じたら終了 */
	fNumDocWindow -= 1;
	if (fNumDocWindow == 0)
		this->PostMessage(B_QUIT_REQUESTED, this);
	
	return;
}

/*
 * フォントメニュー作成; MemoApp
 */
void
MemoApp::ConfigureFontMenu (void)
{
	status_t	sts;
	BMenu		theFontMenu("Font");
	BMenu*		theSizeMenu;

	/* アーカイブ用のメッセージを生成 */
	fFontMenuArchive = new BMessage();

	/* サイズ指定のサブメニューを生成 */	
	theSizeMenu = new BMenu("Size");
	theSizeMenu->AddItem(new BMenuItem("9", new BMessage(SET_SIZE)));
	theSizeMenu->AddItem(new BMenuItem("10", new BMessage(SET_SIZE)));
	theSizeMenu->AddItem(new BMenuItem("12", new BMessage(SET_SIZE)));
	theSizeMenu->AddItem(new BMenuItem("14", new BMessage(SET_SIZE)));
	theSizeMenu->AddItem(new BMenuItem("18", new BMessage(SET_SIZE)));
	theSizeMenu->AddItem(new BMenuItem("24", new BMessage(SET_SIZE)));
	theSizeMenu->AddItem(new BMenuItem("36", new BMessage(SET_SIZE)));
	theSizeMenu->AddItem(new BMenuItem("48", new BMessage(SET_SIZE)));
	theSizeMenu->AddItem(new BMenuItem("72", new BMessage(SET_SIZE)));
	theSizeMenu->SetRadioMode(true);
	theFontMenu.AddItem(theSizeMenu);	/* フォントメニューに追加 */
	theFontMenu.AddSeparatorItem();		/* -- */

	/* 全てのフォントファミリーについて繰り返し */
	for (int32 i = 0, n = ::count_font_families(); i < n; ++i) {
		font_family	aFamily;
		BMenu*		aFamilyMenu;
		
		/* ファミリー指定のサブメニューを生成 */
		sts = ::get_font_family(i, &aFamily);
		if (sts != B_OK)
			goto err;
		aFamilyMenu = new BMenu(aFamily);
		
		/* そのファミリーの全てのスタイルについて繰り返し */
		for (int32 j = 0, n = ::count_font_styles(aFamily); j < n; ++j) {
			font_style	aStyle;
			BMenuItem*	aStyleItem;
			
			/* スタイル指定のメニュー項目を生成 */
			sts = ::get_font_style(aFamily, j, &aStyle);
			if (sts != B_OK)
				goto err;
			aStyleItem = new BMenuItem(
				aStyle, new BMessage(SET_FONT_STYLE));

			/* ファミリー指定のサブメニューに追加 */
			aFamilyMenu->AddItem(aStyleItem);
		}
		
		/* フォントメニューに追加 */
		theFontMenu.AddItem(aFamilyMenu);
		aFamilyMenu->SetRadioMode(true);
		(void)aFamilyMenu->Superitem()->SetMessage(
			new BMessage(SET_FONT_FAMILY));
	}
	
	/* 作成したフォントメニューの内容をアーカイブ */
	theFontMenu.Archive(fFontMenuArchive);
	
	return;
err:
	::Error("MemoApp::ConfigureFontMenu", sts);
	return;
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * MemoAppクラスの公開メソッド
 */
MemoApp::MemoApp (void)
	: BApplication("application/x-vnd.FtGUN-MemoApp")
{
	fAboutBox = NULL;
	fCreatedDocs = fNumDocWindow = 0;
	fFontMenuArchive = NULL;
	fOpenPanel = NULL;
	fRefFilter = NULL;
}

MemoApp::~MemoApp (void)
{
	delete fFontMenuArchive;
	delete fOpenPanel;
	delete fRefFilter;
}


/*
 * End of File
 */
