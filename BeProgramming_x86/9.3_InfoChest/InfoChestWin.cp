/************************************************************************
 * $Id$
 * Copyright (c) 1998 Fort Gunnison, Inc.
 *
 * InfoChestWin.cp: InfoChestWinクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 24, 1998
 * Last update:	May. 01, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "InfoChestWin.h"
#include "Categorizer.h"
#include "InfoChestView.h"
#include "GUI/MenuUtil.h"
#include "GUI/SimpleDialog.h"

#include <app/Application.h>
#include <interface/Button.h>
#include <interface/StringView.h>
#include <interface/TextControl.h>
#include <support/Beep.h>
#include <support/ClassInfo.h>
#include <support/Debug.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/* メッセージのコード */
enum {
	NEW_CATEGORY	= 'cctg',
	RMV_CATEGORY	= 'rctg',
	ADD_CATEGORY	= 'actg'
};

/* ビュー配置用の定数 */
const float	kHorizMargin	= 5;
const float	kVertMargin		= 5;


/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/* 文字列定数 */
const char	kInputField[]	= "input field";

/* メニュー情報 */
static MItemInfo	sCategoryMenu[]	= {  /* Categoryメニュー */
	{ "Add...",	NEW_CATEGORY, NULL },
	{ "Remove",	RMV_CATEGORY, NULL },
	NULL
};


/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/* InfoChestWinクラスの非公開メソッド */
/*
 * メッセージ処理; InfoChestWin
 */
void
InfoChestWin::MessageReceived (BMessage* message)
{
	switch (message->what) {
	case NEW_CATEGORY:
		this->CreateCategory();			break;
	case RMV_CATEGORY:
		this->RemoveCategory();			break;
	case ADD_CATEGORY:
		this->AddCategory(message);		break;
	default:
		RegularWindow::MessageReceived(message);
	}
	
	return;
}

void
InfoChestWin::CreateCategory (void)
{
	SimpleDialog*	theDialog;
	BMessenger		theTarget(this);
	
	theDialog = new SimpleDialog(
		BRect(300, 300, 500, 400),
		"New Category",
		B_MODAL_WINDOW,
		B_NOT_CLOSABLE|B_NOT_ZOOMABLE|B_NOT_RESIZABLE,
		ADD_CATEGORY
	);
	this->MakeDialogView(theDialog);
	(void)theDialog->Go(this);
	
	return;
}

void
InfoChestWin::RemoveCategory (void)
{
	const char*		theCategory;
	Categorizer*	categorizer = Categorizer::GetInstance();

	/* 選択されているカテゴリを取得 */
	theCategory = fInfoChestView->GetSelectedCategory();
	
	/* カテゴリの削除処理を実行してアプリケーションに通知 */
	if (categorizer->RemoveCategory(theCategory) == B_OK) {
		BMessage	theMessage(HIDE_DOCUMENTS);
	
		(void)theMessage.AddString(kArgCategory, theCategory);
		(void)be_app->PostMessage(&theMessage);
		fInfoChestView->UpdateList();
	}
	
	return;
/*
 * 注意：fInfoChestViewのUpdateList()メソッドを実行すると、
 *		theCategoryの値は無効になる。したがって、アプリケーションへメッ
 *		セージを送る前にUpdateList()を呼び出さないこと。
 *		('98. 5/1, koga@ftgun.co.jp)
 */
}

void
InfoChestWin::AddCategory (BMessage* message)
{
	const char*		theCategory;
	Categorizer*	categorizer = Categorizer::GetInstance();

	/* カテゴリ名を取得 */
	(void)message->FindString(kValueArg, &theCategory);
	
	/* カテゴリ名が無効なら何もしない */
	if (!theCategory || theCategory[0] == '\0') {
		(void)::beep();
		return;
	}
	
	/* カテゴリの新規作成処理を実行 */
	if (categorizer->AddCategory(theCategory) == B_OK)
		fInfoChestView->UpdateList();
	
	return;
}

/*
 * メニュー調節; InfoChestWin
 */
void
InfoChestWin::MenusBeginning (void)
{
	ASSERT(this->KeyMenuBar() != NULL && fInfoChestView != NULL);
	
	BMenuItem*	anItem;
	
	anItem = this->KeyMenuBar()->FindItem(RMV_CATEGORY);
	anItem->SetEnabled(
		fInfoChestView->GetSelectedCategory() != NULL
	);
	
	return;
}

/*
 * ダイアログ操作; InfoChestWin
 */
void
InfoChestWin::MakeDialogView (SimpleDialog* inDialog)
{
	BRect			theBounds = inDialog->Bounds();
	BView*			theContainer;
	BStringView*	theCaption;
	BTextControl*	theInputField;
	BButton*		theOKButton;
	BButton*		theCancelButton;
	float			theTop;

	theContainer = new BView(
		theBounds, "container", B_FOLLOW_ALL_SIDES, B_WILL_DRAW
	);
	theContainer->SetViewColor(224, 224, 224);
	theCaption = new BStringView(
		BRect(0, 0, 120, 20), "caption", "Enter the new category"
	);
	theInputField = new BTextControl(
//		BRect(0, 0, 0, 0), kInputField, NULL, NULL, NULL
		BRect(0, 0, 30, 0), kInputField, NULL, NULL, NULL
	);
	theOKButton = new BButton(
		BRect(0, 0, 40, 0),
		"ok button",
		"OK",
		new BMessage(KGSD_OK)
	);
	theCancelButton = new BButton(
		BRect(0, 0, 60, 0),
		"cancel button",
		"Cancel",
		new BMessage(KGSD_CANCEL)
	);

	theContainer->AddChild(theCaption);
	theContainer->AddChild(theInputField);
	theContainer->AddChild(theOKButton);
	theContainer->AddChild(theCancelButton);
	inDialog->SetContent(theContainer, kInputField);

	theCaption->MoveTo(
		(theBounds.Width() - theCaption->Bounds().Width()) / 2,
		kVertMargin
	);
	theTop = theCaption->Frame().bottom + kVertMargin;
	theInputField->ResizeTo(
		theBounds.Width() - 2 * kHorizMargin,
		theInputField->Bounds().Height()
	);
	theInputField->MoveTo(kHorizMargin, theTop);
	theOKButton->MoveTo(
		0,
		theBounds.bottom
			- theOKButton->Bounds().Height() - 2 * kVertMargin
	);
	theCancelButton->MoveTo(
		theBounds.right - theOKButton->Bounds().Width()
			- theCancelButton->Bounds().Width() - 30,
		theBounds.bottom
			- theOKButton->Bounds().Height() - 2 * kVertMargin
	);
	
	inDialog->SetDefaultButton(theOKButton);
	theOKButton->MoveTo(
		theBounds.right - theOKButton->Bounds().Width() - kHorizMargin,
		theOKButton->Frame().top
	);
	
	return;
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * InfoChestWinクラスの公開メソッド
 */
InfoChestWin::InfoChestWin (BRect frame, const char* title,
		window_type type, uint32 flags)
	: RegularWindow(frame, title, type, flags, B_QUIT_REQUESTED)
{
	fInfoChestView = NULL;
}

InfoChestWin::~InfoChestWin (void)
{  /* do nothing */  }

void
InfoChestWin::InitContent (BView* inContent)
{
	RegularWindow::InitContent(inContent);
	fInfoChestView = cast_as(inContent, InfoChestView);
	
	return;
}

void
InfoChestWin::InitContent (BMenuBar* inMenuBar, BView* inContent)
{
	BMenu*	theMenu;

	/* カテゴリメニューを生成してメニューバーに追加 */
	theMenu = MenuUtil::MakeMenu("Category", sCategoryMenu);
	inMenuBar->AddItem(theMenu);

	/* ウィンドウ内容をセットアップ */
	RegularWindow::InitContent(inMenuBar, inContent);
	fInfoChestView = cast_as(inContent, InfoChestView);
	
	return;
}


/*
 * End of File
 */
