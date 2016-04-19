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

#include "StickyMemoApp.h"
#include "StickyMemoMenu.h"
#include "MemoWindow.h"
#include "KGUtility/kgDebug.h"  /* デバッグ用 */

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

class MyTextRefFilter : public BRefFilter {
public:
	bool	Filter(const entry_ref*, BNode*, struct stat*,
					   const char* mimetype);
};


/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/* アバウトダイアログのメッセージ */
const char	kAboutMsg[] = "StickyMemo\n\n"
	"Copyright " B_UTF8_COPYRIGHT " 1998 Fort Gunnison, Inc.\n"
	"Author: Shin'ya Koga (koga@ftgun.co.jp)";

const char kMyAppSig[]	= "application/x-vnd.FtGUN-StickyMemo";


/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/*
 * MyTestRefFilterクラスのメソッド;
 */
bool
MyTextRefFilter::Filter (const entry_ref* ref, BNode* /* node */,
	struct stat* /* st */, const char* mimetype)
{
	BEntry		theEntry(ref, true);
	BMimeType	theType(mimetype);
	entry_ref	orgRef;
	
	if (theEntry.IsDirectory())
		return true;
	
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
	if (fNumDocWindow == 0) {
		this->ConfigureFontMenu();
		this->NewDocument();
	}

	return;
}

/*
 * メッセージ応答; MemoApp
 */
void
MemoApp::MessageReceived (BMessage* message)
{
	switch (message->what) {
	case NEW_DOC:
		this->NewDocument();		break;
	case OPEN_DOC:
		this->ShowOpenPanel();		break;
	case ABOUT_CLOSED:
		fAboutBox = NULL;			break;
	case DOC_CLOSED:
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
	
	/* 必要ならフォントメニューを生成 */
	if (fFontMenuArchive == NULL)
		this->ConfigureFontMenu();
	
	for (int32 i = 0;; ++i) {
		sts = message->FindRef("refs", i, &theRef);
		if (sts != B_OK)
			break;
		(void)entryObj.SetTo(&theRef, true);
		if (entryObj.IsFile()) {
			(void)entryObj.GetRef(&theRef);  /* 念のため */
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
		sprintf(docName, "Untitled %d", ++fCreatedDocs);
		newDocWin = new MemoWindow(kDefaultDocFrame, docName, DOC_CLOSED);
	} else
		newDocWin = new MemoWindow(
			kDefaultDocFrame, B_EMPTY_STRING, DOC_CLOSED);

	theFontMenu = (BMenu*)BMenu::Instantiate(fFontMenuArchive);
	theMenuBar->AddItem(theFontMenu, 2);
	newDocWin->InitContent(theMenuBar, doShow);
	fNumDocWindow += 1;
	
	return newDocWin;
}

void
MemoApp::OpenDocument (const entry_ref& inRef)
{
	MemoWindow*	newDocWin;
	entry_ref	aRef;
	
	/* find the window */
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
	
	newDocWin = this->NewDocument(false);
	newDocWin->LoadFile(inRef);
	
	return;
}

void
MemoApp::ShowOpenPanel (void)
{
	if (fOpenPanel == NULL) {
		fOpenPanel = new BFilePanel(B_OPEN_PANEL);
		fRefFilter = new MyTextRefFilter();
		fOpenPanel->SetRefFilter(fRefFilter);
	}
	if (! fOpenPanel->IsShowing())
		fOpenPanel->Show();
	else
		fOpenPanel->Window()->Activate();
	
	return;
}

void
MemoApp::DocumentClosed (void)
{
	fNumDocWindow -= 1;
	if (fNumDocWindow == 0)
		this->PostMessage(B_QUIT_REQUESTED, this);
	
	return;
}

/*
 * フォントメニューの生成
 */
void
MemoApp::ConfigureFontMenu (void)
{
	status_t	sts;
	BMenu		theFontMenu("Font");
	BMenu*		theSizeMenu;

	fFontMenuArchive = new BMessage();
	
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
	theFontMenu.AddItem(theSizeMenu);
	theFontMenu.AddSeparatorItem();

	for (int32 i = 0, n = count_font_families(); i < n; ++i) {
		font_family	aFamily;
		BMenu*		aFamilyMenu;
		
		sts = get_font_family(i, &aFamily);
		if (sts != B_OK)
			goto err;
		aFamilyMenu = new BMenu(aFamily);
		for (int32 j = 0, n = count_font_styles(aFamily); j < n; ++j) {
			font_style	aStyle;
			BMenuItem*	aStyleItem;
			
			sts = get_font_style(aFamily, j, &aStyle);
			if (sts != B_OK)
				goto err;
			aStyleItem = new BMenuItem(
				aStyle, new BMessage(SET_FONT_STYLE));
			aFamilyMenu->AddItem(aStyleItem);
		}
		aFamilyMenu->SetRadioMode(true);
		theFontMenu.AddItem(aFamilyMenu);
		(void)aFamilyMenu->Superitem()->SetMessage(
			new BMessage(SET_FONT_FAMILY));
	}
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
MemoApp::MemoApp (void) : BApplication(kMyAppSig)
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
