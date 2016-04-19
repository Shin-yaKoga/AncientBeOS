/************************************************************************
 * $Id$
 * Copyright (c) 1998 Fort Gunnison, Inc.
 *
 * SearchView.cp: SearchViewクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 30, 1998
 * Last update:	Apr. 30, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "SearchView.h"
#include "GUI/EntryListView.h"
#include "GUI/MenuUtil.h"
#include "KGUtility/kgDebug.h"

#include <interface/Box.h>
#include <interface/Button.h>
#include <interface/MenuField.h>
#include <interface/PopUpMenu.h>
#include <interface/ScrollBar.h>
#include <interface/ScrollView.h>
#include <interface/StringView.h>
#include <interface/TextControl.h>
#include <storage/Volume.h>
#include <support/Debug.h>

#include <parsedate.h>
#include <stdlib.h>
#include <string.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/* メッセージのコード */
enum {
	SEARCH_FILE		= 'srch',
	PARTIAL_SEL		= 'psel'
};

/* ビュー配置用の定数 */
const float	kHorizMargin	= 5;
const float	kVertMargin		= 5;
const float	kCriteriaWidth	= 150;


/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/* 文字列定数 */
const char	kSourceArg[]	= "source";

const char	kCriteria[]		= "Criteria";
const char	kButtonSearch[]	= "SearchBtn";
const char	kListView[]		= "Found List";
const char	kScroller[]		= "Scroller";
const char	kMenuAttrib[]	= "AttribMenu";
const char	kCaption[]		= "Caption";
const char	kEditAttrib[]	= "AttribField";
const char	kMenuVol[]		= "VolMenu";

const char	kAllVolLabel[]		= "All disks";
const char	kNameLabel[]		= "Name";
const char	kSizeLabel[]		= "Size";
const char	kModifiedLabel[]	= "Modified";
const char	kContainsLabel[]	= "contains";
const char	kIsLabel[]			= "is";
const char	kNotLabel[]			= "is not";
const char	kStartsLabel[]		= "starts with";
const char	kEndsLabel[]		= "ends with";
const char	kGreaterLabel[]		= "greater than";
const char	kLessLabel[]		= "less than";
const char	kBeforeLabel[]		= "before";
const char	kAfterLabel[]		= "after";

const char	kAttrName[]		= "name";
const char	kAttrSize[]		= "size";
const char	kAttrModified[]	= "last_modified";

/* メニュー情報 */
static MItemInfo	sNameMenu[]	= {		/* 名前検索用 */
	{ kContainsLabel,	B_CONTAINS,		NULL },
	{ kIsLabel,			B_EQ,			NULL },
	{ kNotLabel,		B_NE,			NULL },
	{ kStartsLabel,		B_BEGINS_WITH,	NULL },
	{ kEndsLabel,		B_ENDS_WITH,	NULL },
	NULL
};

static MItemInfo	sSizeMenu[]	= {		/* サイズ検索用 */
	{ kGreaterLabel,	B_GT,	NULL },
	{ kLessLabel,		B_LT,	NULL },
	{ kIsLabel,			B_EQ,	NULL },
	NULL
};

static MItemInfo	sModifiedMenu[]	= {	/* 更新日検索用 */
	{ kBeforeLabel,		B_LE,	NULL },
	{ kAfterLabel,		B_GE,	NULL },
	NULL
};


/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/* SearchViewクラスの非公開メソッド */
/*
 * メッセージ処理; SearchView
 */
void
SearchView::MouseDown (BPoint /* where */)
{
	this->MakeFocus();
	return;
}

void
SearchView::FrameResized (float /* newWidth */, float /* newHeight */)
{
	BView*	theScrollView = this->FindView(kScroller);
	BRect	theKnobRect;
	
	/* スクロールビューの右下隅の再描画を強制 */
	theKnobRect = theScrollView->Bounds();
	theKnobRect.left = theKnobRect.right - B_V_SCROLL_BAR_WIDTH;
	theKnobRect.top  = theKnobRect.bottom - B_H_SCROLL_BAR_HEIGHT;
	theScrollView->Invalidate(theKnobRect);
	
	return;
}

void
SearchView::UpdateOperator (BMessage* message)
{
	BMenuItem*		theSender;
	BStringView*	theCaption;
	
	/* 選択項目の上位項目にもマークをセット */
	(void)message->FindPointer(kSourceArg, &theSender);
	theSender->Menu()->Superitem()->SetMarked(true);
	
	/* 条件演算子の表示を更新 */
	theCaption = (BStringView*)this->FindView(kCaption);
	theCaption->SetText(theSender->Label());
	fCurrOp = (query_op)message->what;
	
	return;
}

void
SearchView::ResetOperator (BMessage* message)
{
	BMenuItem*		theSender;
	BMenuItem*		operatorItem;
	BStringView*	theCaption;
	
	/* 部分選択されたサブメニューから選択項目を取得 */
	(void)message->FindPointer(kSourceArg, &theSender);
	operatorItem = theSender->Submenu()->FindMarked();
	
	/* 演算子文字列と演算子を更新 */
	theCaption = (BStringView*)this->FindView(kCaption);
	theCaption->SetText(operatorItem->Label());
	fCurrOp = (query_op)operatorItem->Command();
	
	return;
}

void
SearchView::VolumeMounted (BMessage* /* message */)
{
	this->AdjustVolumeMenu();
	return;
}

void
SearchView::VolumeUnmounted (BMessage* message)
{
	/* 親クラスのメソッドを実行 */
	RetrieverView::VolumeUnmounted(message);
	
	/* ボリュームメニューを再構築 */
	this->AdjustVolumeMenu();
	
	return;
}

/*
 * 検索処理; SearchView
 */
void
SearchView::StartSearch (void)
{
	BMenuField*	theMenuField;
	BMenuItem*	theMenuItem;
	int32		theItemIndex;
	BList		mountedVols, targetVols;

	/* 検索対象ボリュームを取得 */
	(void)GetQueryAwareVolumes(&mountedVols);
	theMenuField = (BMenuField*)this->FindView(kMenuVol);
	theMenuItem  = theMenuField->Menu()->FindMarked();
	theItemIndex = theMenuField->Menu()->IndexOf(theMenuItem);
	if (theItemIndex == 0)
		targetVols = mountedVols;  /* 全ボリューム */
	else
		targetVols.AddItem(mountedVols.ItemAt(theItemIndex - 2));
	
	/* 検索処理を開始 */
	RetrieverView::StartSearch(targetVols, false, B_FILE_NODE);
	
	return;
}

void
SearchView::MakeupQuery (BQuery* outQuery)
{
	const char*		theAttr;
	const char*		theValue;
	const char*		theLabel;
	BMenuField*		theMenuField;
	BTextControl*	theEditField;

	/* 属性名をプッシュ */
	theMenuField = (BMenuField*)this->FindView(kMenuAttrib);
	theLabel = theMenuField->Menu()->FindMarked()->Label();
	if (strcmp(theLabel, kNameLabel) == 0)
		theAttr = kAttrName;
	else if (strcmp(theLabel, kSizeLabel) == 0)
		theAttr = kAttrSize;
	else if (strcmp(theLabel, kModifiedLabel) == 0)
		theAttr = kAttrModified;
	else
		goto err;
	outQuery->PushAttr(theAttr);

	/* 属性の値をプッシュ */
	theEditField = (BTextControl*)this->FindView(kEditAttrib);
	theValue = theEditField->Text();
	if (theAttr == kAttrName)
		outQuery->PushString(theValue);
	else if (theAttr == kAttrSize)
		outQuery->PushInt64(strtoll(theValue, NULL, 10));
	else {
		time_t	theSec = parsedate(theValue, -1);
		outQuery->PushInt32(theSec);
	}

	/* 演算子をプッシュ */
	outQuery->PushOp(fCurrOp);
	
	return;
err:
	::Error("SearchView::MakeupQuery", B_ERROR);
	return;
}

/*
 * サブビュー管理; SearchView
 */
void
SearchView::AllAttached (void)
{
	/* サブビューをレイアウト */
	this->AdjustContent();
	
	/* ボリュームメニューを再構成 */
	this->AdjustVolumeMenu();
	
	return;
}

void
SearchView::DetachedFromWindow (void)
{
	/* 検索処理を強制終了 */
	this->AbortSearch();
	return;
}

void
SearchView::MakeContent (void)
{
	ASSERT(this->CountChildren() == 0);
	
	BView*			theBoxView;
	BButton*		theButton;
	EntryListView*	theListView;
	BScrollView*	theScrollView;
	BRect			initFrame(0, 0, 0, 0);
	
	/* 検索条件のボックスを生成 */
	theBoxView = new BBox(
		BRect(0, 0, 0, 0),
		kCriteria,
		B_FOLLOW_TOP_BOTTOM
	);
	this->MakeCriteriaBox(theBoxView);
	
	/* 検索ボタンを生成 */
	theButton = new BButton(
		BRect(0, 0, 0, 0),
		kButtonSearch,
		"Search",
		new BMessage(SEARCH_FILE),
		B_FOLLOW_LEFT|B_FOLLOW_BOTTOM
	);
	
	/* 検索結果のリストボックスを生成 */
	initFrame.right  = B_V_SCROLL_BAR_WIDTH;
	initFrame.bottom = B_H_SCROLL_BAR_HEIGHT;
	theListView = new EntryListView(
		initFrame, 
		kListView,
		B_SINGLE_SELECTION_LIST,
		B_FOLLOW_ALL_SIDES
	);
	theScrollView = new BScrollView(
		kScroller,
		theListView,
		B_FOLLOW_ALL_SIDES,
		0,
		true, true, B_PLAIN_BORDER
	);
	theScrollView->SetViewColor(224, 224, 224);
	
	/* 各サブビューを自身に登録 */
	this->AddChild(theBoxView);
	this->AddChild(theButton);
	this->AddChild(theScrollView);
	this->SetEntryListView(theListView);
	
	return;
}

void
SearchView::MakeCriteriaBox (BView* inBox)
{
	BMenu*			aMenu;
	BMenuField*		aMenuField;
	BStringView*	theCaption;
	BTextControl*	theEditField;
	
	/* 属性指定のポップアップメニューを生成 */
	aMenu = this->MakeAttribMenu();
	aMenuField = new BMenuField(
		BRect(0, 0, 0, 0), kMenuAttrib, B_EMPTY_STRING, aMenu
	);
	inBox->AddChild(aMenuField);
	
	/* 検索条件の演算子表示ラベルを生成 */
	theCaption = new BStringView(
		BRect(0, 0, 0, 0), kCaption, kContainsLabel
	);
	inBox->AddChild(theCaption);
	
	/* 属性値の入力フィールドを生成 */
	theEditField = new BTextControl(
		BRect(0, 0, 20, 0),  /* width must not zero!! */
		kEditAttrib,
		NULL,
		NULL,
		NULL,
		B_FOLLOW_LEFT_RIGHT|B_FOLLOW_TOP
	);
	inBox->AddChild(theEditField);
	
	/* ボリューム指定のポップアップメニューを作成 */
	aMenu = new BPopUpMenu(B_EMPTY_STRING);
	aMenu->AddItem(new BMenuItem(kAllVolLabel, NULL));
	aMenu->AddSeparatorItem();
	aMenuField = new BMenuField(
		BRect(0, 0, 0, 0), kMenuVol, "On:", aMenu
	);
	inBox->AddChild(aMenuField);
	
	return;
}

BMenu*
SearchView::MakeAttribMenu (void)
{
	BMenu*	theMenu = new BPopUpMenu(B_EMPTY_STRING);
	BMenu*	aSubMenu;
	
	/* 名前検索用のサブメニューを生成 */
	aSubMenu = MenuUtil::MakeMenu(kNameLabel, sNameMenu);
	aSubMenu->SetRadioMode(true);
	theMenu->AddItem(aSubMenu);
	(void)aSubMenu->Superitem()->SetMessage(new BMessage(PARTIAL_SEL));
	
	/* サイズ検索用のサブメニューを生成 */
	aSubMenu = MenuUtil::MakeMenu(kSizeLabel, sSizeMenu);
	aSubMenu->SetRadioMode(true);
	theMenu->AddItem(aSubMenu);
	(void)aSubMenu->Superitem()->SetMessage(new BMessage(PARTIAL_SEL));
	
	/* 更新日検索用のサブメニューを生成 */
	aSubMenu = MenuUtil::MakeMenu(kModifiedLabel, sModifiedMenu);
	aSubMenu->SetRadioMode(true);
	theMenu->AddItem(aSubMenu);
	(void)aSubMenu->Superitem()->SetMessage(new BMessage(PARTIAL_SEL));
	
	/* デフォルトで選択される項目を設定 */
	theMenu->FindItem(kNameLabel)->SetMarked(true);
	theMenu->FindItem(kContainsLabel)->SetMarked(true);
	theMenu->FindItem(kGreaterLabel)->SetMarked(true);
	theMenu->FindItem(kAfterLabel)->SetMarked(true);
	fCurrOp = B_CONTAINS;
	
	return theMenu;
}

void
SearchView::AdjustContent (void)
{
	BView*			aView;
	BView*			theButton;
	BScrollView*	theScroller;
	BListView*		theListView;
	BRect			myBounds = this->Bounds();
	float			myWidth  = myBounds.Width();
	float			myHeight = myBounds.Height();
	
	/* 検索条件のボックスをレイアウト */
	aView = this->FindView(kCriteria);
	aView->ResizeTo(
		kCriteriaWidth - kHorizMargin, myHeight - kVertMargin - 30
	);
	aView->MoveTo(kHorizMargin, kVertMargin);
	this->AdjustCriteriaBox(aView);
	
	/* 検索ボタンをレイアウト */
	theButton = this->FindView(kButtonSearch);
	theButton->ResizeBy(50, 0);
	theButton->MoveTo(
		aView->Frame().right - theButton->Bounds().Width(),
		myHeight - theButton->Bounds().Height() - 5
	);
	((BButton*)theButton)->SetTarget(this);
	
	/* 検索結果のリストボックスをレイアウト */
	theScroller = (BScrollView*)this->FindView(kScroller);
	theScroller->ResizeTo(
		myWidth - kCriteriaWidth - 2 * kHorizMargin,
		myHeight - 2 * kVertMargin
	);
	theScroller->MoveTo(kCriteriaWidth + kHorizMargin, kVertMargin);
	
	/* リスト表示ビューと自身を結合 */
	theListView = (BListView*)this->FindView(kListView);
	theListView->SetTarget(this);
	
	return;
}

void
SearchView::AdjustCriteriaBox (BView* inBox)
{
	BView*	aView;
	BRect	theBounds = inBox->Bounds();
	float	theWidth  = theBounds.Width();
	float	theTop;
	BMenu*	theMenu;
	int32	i, n;
	
	/* 属性指定のポップアップメニューをレイアウト */
	aView = this->FindView(kMenuAttrib);
	aView->ResizeBy(80, 0);
	aView->MoveTo(kHorizMargin, kVertMargin);
	theTop = aView->Frame().bottom + kVertMargin;
	theMenu = ((BMenuField*)aView)->Menu();
	for (i = 0, n = theMenu->CountItems(); i < n; ++i) {
		BMenu*	aMenu = theMenu->SubmenuAt(i);
		aMenu->SetTargetForItems(this);
	}
	theMenu->SetTargetForItems(this);
	
	/* 検索条件の演算子表示ラベルをレイアウト */
	aView = this->FindView(kCaption);
	aView->ResizeTo(100, 10);
	aView->MoveTo(kHorizMargin, theTop);
	theTop = aView->Frame().bottom + kVertMargin;
	
	/* 属性値の入力フィールドをレイアウト */
	aView = this->FindView(kEditAttrib);
	aView->ResizeTo(
		theWidth - 2 * kHorizMargin - 2,
		aView->Bounds().Height()
	);
	aView->MoveTo(kHorizMargin, theTop);
	theTop = aView->Frame().bottom + kVertMargin;
	
	/* ボリューム指定のポップアップメニューをレイアウト */
	aView = this->FindView(kMenuVol);
	aView->ResizeBy(100, 0);
	aView->MoveTo(kHorizMargin, theTop);
	((BMenuField*)aView)->SetDivider(20);
	
	return;
}

void
SearchView::AdjustVolumeMenu (void)
{
	BMenuField*	theMenuField;
	BMenu*		theMenu;
	BMenuItem*	aMenuItem;
	BList		volumeList;
	BVolume		aVolume;
	char		volName[B_FILE_NAME_LENGTH];

	/* 不要なメニュー項目を削除 */
	theMenuField = (BMenuField*)this->FindView(kMenuVol);
	theMenu = theMenuField->Menu();
	while ((aMenuItem = theMenu->RemoveItem(2)) != NULL) {
		delete aMenuItem;
	}
	
	/* マウントされているボリューム一覧を取得 */
	(void)GetQueryAwareVolumes(&volumeList);
	
	/* ボリュームごとにメニュー項目を追加 */
	for (int32 i = 0, n = volumeList.CountItems(); i < n; ++i) {
		dev_t	aDevice = (dev_t)volumeList.ItemAt(i);
		
		(void)aVolume.SetTo(aDevice);
		(void)aVolume.GetName(volName);
		theMenu->AddItem(new BMenuItem(volName, NULL));
	}
	theMenu->FindItem(kAllVolLabel)->SetMarked(true);
	
	return;
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/* SearchViewクラスの公開メソッド */
/*
 * 初期化と解放; SearchView
 */
SearchView::SearchView (BRect frame, uint32 resizeMask)
	: RetrieverView(frame, "SearchView",
		resizeMask, B_WILL_DRAW|B_FRAME_EVENTS)
{
	this->MakeContent();
	this->SetViewColor(224, 224, 224);
}

SearchView::~SearchView (void)
{  /* do nothing */  }

/*
 * メッセージ処理; SearchView
 */
void
SearchView::MessageReceived (BMessage* message)
{
	switch (message->what) {
	case B_EQ:
	case B_GT:
	case B_GE:
	case B_LT:
	case B_LE:
	case B_NE:
	case B_CONTAINS:
	case B_BEGINS_WITH:
	case B_ENDS_WITH:
		this->UpdateOperator(message);	break;
	case PARTIAL_SEL:
		this->ResetOperator(message);	break;
	case SEARCH_FILE:
		this->StartSearch();			break;
	default:
		RetrieverView::MessageReceived(message);
	}
	
	return;
}


/*
 * End of File
 */
