/************************************************************************
 * $Id$
 * Copyright (c) 1998 Fort Gunnison, Inc.
 *
 * InfoChestView.cp: InfoChestViewクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 24, 1998
 * Last update:	Feb. 01, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "InfoChestView.h"
#include "Categorizer.h"
#include "KGUtility/kgDebug.h"

#include <app/Application.h>
#include <interface/Button.h>
#include <interface/ListView.h>
#include <interface/ScrollBar.h>
#include <interface/ScrollView.h>
#include <support/Debug.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/* メッセージのコード */
enum {
	CATEGORY_SELECTED	= 'ctgs',
	OPEN_CATEGORY		= 'octg'
};

/* ビュー配置用の定数 */
const float	kHorizMargin	= 5;
const float	kVertMargin		= 5;
const float	kControlsHeight	= 35;


/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/* 文字列定数 */
const char	kListView[]		= "CategoryList";
const char	kScroller[]		= "Scroller";
const char	kOpenButton[]	= "OpenBtn";
const char	kArgCategory[]	= "category";


/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/* InfoChestViewクラスの非公開メソッド */
/*
 * カテゴリの表示管理; InfoChestView
 */
void
InfoChestView::CleanupList (void)
{
	BListView*	theListView = (BListView*)this->FindView(kListView);
	BListItem*	anItem;
	
	while ((anItem = theListView->RemoveItem((int32)0)) != NULL)
		delete anItem;
	
	return;
}

/*
 * メッセージ処理; InfoChestView
 */
void
InfoChestView::MessageReceived (BMessage* message)
{
	switch (message->what) {
	case CATEGORY_SELECTED:
		this->SelectionChanged();	break;
	case OPEN_CATEGORY:
		this->ShowDocuments();		break;
	default:
		BView::MessageReceived(message);
	}
	
	return;
}

void
InfoChestView::SelectionChanged (void)
{
	BButton*	theButton;
	BListView*	theListView;
	
	theListView = (BListView*)this->FindView(kListView);
	theButton   = (BButton*)this->FindView(kOpenButton);
	theButton->SetEnabled(theListView->CurrentSelection() >= 0);
	
	return;
}

void
InfoChestView::ShowDocuments (void)
{
	status_t	sts;
	const char*	theCategory = this->GetSelectedCategory();
	BMessage	theMessage(SHOW_DOCUMENTS);
	
	/* カテゴリが選択されていなければ何もしない */
	if (! theCategory)
		return;
	
	(void)theMessage.AddString(kArgCategory, theCategory);
	sts = be_app->PostMessage(&theMessage);
	if (sts != B_OK)
		goto err;
	
	return;
err:
	::Error("InfoChestView::ShowDocuments", sts);
	return;
}

/*
 * 描画処理; InfoChestView
 */
void
InfoChestView::AttachedToWindow (void)
{
	/* サブビューをレイアウト */
	this->AdjustContent();

	/* カテゴリを一覧表示 */
	this->UpdateList();

	return;
}

void
InfoChestView::DetachedFromWindow (void)
{
	this->CleanupList();
	return;
}

void
InfoChestView::MakeContent (void)
{
	BListView*		theListView;
	BScrollView*	theScrollView;
	BButton*		theButton;
	BRect			initFrame(0, 0, 0, B_H_SCROLL_BAR_HEIGHT);
	
	ASSERT(this->CountChildren() == 0);

	/* カテゴリの一覧表示リストを生成 */
	theListView = new BListView(
		initFrame, kListView, B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL_SIDES
	);
	theListView->SetSelectionMessage(new BMessage(CATEGORY_SELECTED));
	theListView->SetInvocationMessage(new BMessage(OPEN_CATEGORY));
	theScrollView = new BScrollView(
		kScroller, theListView, B_FOLLOW_ALL_SIDES, 0,
		false, true, B_PLAIN_BORDER
	);

	/* オープンボタンを生成 */
	theButton = new BButton(
		BRect(0, 0, 0, 0), kOpenButton, "Open",
		new BMessage(OPEN_CATEGORY),
		B_FOLLOW_RIGHT|B_FOLLOW_BOTTOM
	);
	
	/* サブビューとして登録 */
	this->AddChild(theScrollView);
	this->AddChild(theButton);
	
	return;
}

void
InfoChestView::AdjustContent (void)
{
	BView*	aView;
	BRect	myBounds = this->Bounds();
	float	myWidth  = myBounds.Width();
	float	myHeight = myBounds.Height();
	
	/* カテゴリの一覧表示リストをレイアウト */
	aView = this->FindView(kScroller);
	aView->ResizeTo(
		myWidth - kHorizMargin * 2,
		myHeight - kVertMargin - kControlsHeight
	);
	aView->MoveTo(kHorizMargin, kVertMargin);
	aView = this->FindView(kListView);
	((BListView*)aView)->SetTarget(this);
	
	/* オープンボタンをレイアウト */
	aView = this->FindView(kOpenButton);
	aView->ResizeBy(50, 0);
	aView->MoveTo(
		myWidth - kHorizMargin - 50,
		myHeight - kVertMargin - aView->Frame().Height()
	);
	((BButton*)aView)->SetTarget(this);
	((BButton*)aView)->SetEnabled(false);
	
	return;
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/* InfoChestViewクラスの公開メソッド */
/*
 * 初期化と解放; InfoChestView
 */
InfoChestView::InfoChestView (BRect frame, uint32 resizeMask)
	: BView(frame, "InfoChestView", resizeMask, B_WILL_DRAW)
{
	this->MakeContent();
	this->SetViewColor(224, 224, 224);
}

InfoChestView::~InfoChestView (void)
{  /* do nothing */  }

/*
 * カテゴリの表示管理; InfoChestView
 */
void
InfoChestView::UpdateList (void)
{
	Categorizer*	categorizer = Categorizer::GetInstance();
	BList			theCategories;
	BListView*		theListView = (BListView*)this->FindView(kListView);
	BStringItem*	newItem;

	this->CleanupList();	
	categorizer->GetCategories(&theCategories);
	for (int32 i = 0, n = theCategories.CountItems(); i < n; ++i) {
		char*	aCategory = (char*)theCategories.ItemAt(i);
		
		newItem = new BStringItem(aCategory);
		theListView->AddItem(newItem);
	}
	
	return;
}

const char*
InfoChestView::GetSelectedCategory (void)
{
	const char*	theCategory = NULL;
	BListView*	theListView;
	int32		theIndex;
	
	theListView = (BListView*)this->FindView(kListView);
	theIndex    = theListView->CurrentSelection();
	if (theIndex >= 0) {
		BStringItem*	theItem;
		
		theItem = (BStringItem*)theListView->ItemAt(theIndex);
		theCategory = theItem->Text();
	}
	
	return theCategory;
}


/*
 * End of File
 */
