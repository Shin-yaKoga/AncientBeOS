/************************************************************************
 * $Id$
 * Copyright (c) 1997-1998 Fort Gunnison, Inc.
 *
 * MessengerView.cp: MessengerViewクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 24, 1997
 * Last update:	Apr. 23, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "MessengerView.h"
#include "KGUtility/kgDebug.h"

#include <app/Roster.h>
#include <interface/Button.h>
#include <interface/ListView.h>
#include <interface/MenuField.h>
#include <interface/MenuItem.h>
#include <interface/PopUpMenu.h>
#include <interface/ScrollView.h>
#include <support/Debug.h>

#include <string.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/* メッセージのコード */
enum {
	APP_SELECTED	= 'apsl',
	SEND_MESSAGE	= 'send'
};

/* ビュー配置用の定数 */
const float	kHorizMargin	= 5;
const float	kVertMargin		= 5;
const float	kControlsHeight	= 60;


/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/* 文字列定数 */
const char	kListView[]		= "AppList";
const char	kPopupMenu[]	= "MsgMenu";
const char	kSendButton[]	= "SendBtn";
const char	kScroller[]		= "Scroller";
const char	kAboutLabel[]	= "B_ABOUT_REQUESTED";
const char	kQuitLabel[]	= "B_QUIT_REQUESTED";


/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/* MessengerViewクラスの非公開メソッド */
/*
 * 描画処理; MessengerView
 */
void
MessengerView::AttachedToWindow (void)
{
	/* サブビューのサイズと位置を調節 */
	this->AdjustContent();
	
	/* 動作中のアプリケーションをチェック */
	this->CheckApps();
	
	return;
}

void
MessengerView::MakeContent (void)
{
	BListView*		theListView;
	BScrollView*	theScrollView;
	BMenuField*		theMenuField;
	BMenu*			theMenu;
	BButton*		theButton;
	BRect			initFrame(0, 0, 0, B_H_SCROLL_BAR_HEIGHT);

	ASSERT(this->CountChildren() == 0);

	/* リストビューを生成 */
	theListView = new BListView(
		initFrame, kListView, B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL_SIDES
	);
	theListView->SetSelectionMessage(new BMessage(APP_SELECTED));
	theScrollView = new BScrollView(
		kScroller, theListView, B_FOLLOW_ALL_SIDES, 0,
		false, true, B_PLAIN_BORDER
	);
	
	/* ポップアップメニューを生成 */
	theMenu = new BPopUpMenu(B_EMPTY_STRING);
	theMenu->AddItem(new BMenuItem(kAboutLabel, NULL));
	theMenu->AddItem(new BMenuItem(kQuitLabel, NULL));
	theMenuField = new BMenuField(
		initFrame, kPopupMenu, "message:",
		theMenu, B_FOLLOW_LEFT|B_FOLLOW_BOTTOM
	);
	theMenu->FindItem(kAboutLabel)->SetMarked(true);

	/* 送信ボタンを生成 */
	theButton = new BButton(
		initFrame, kSendButton, "Send",
		new BMessage(SEND_MESSAGE),
		B_FOLLOW_RIGHT|B_FOLLOW_BOTTOM
	);
	
	/* 各サブビューを自身に追加 */
	this->AddChild(theScrollView);
	this->AddChild(theMenuField);
	this->AddChild(theButton);

	return;
}

void
MessengerView::AdjustContent (void)
{
	BView*	aView;
	BRect	myBounds = this->Bounds();
	float	myWidth  = myBounds.Width();
	float	myHeight = myBounds.Height();

	/* リストビューの配置を設定 */
	aView = this->FindView(kScroller);
	aView->ResizeTo(
		myWidth - kHorizMargin * 2,
		myHeight - kVertMargin - kControlsHeight
	);
	aView->MoveTo(kHorizMargin, kVertMargin);
	((BListView*)this->FindView(kListView))->SetTarget(this);

	/* ポップアップメニューの配置を設定 */
	aView = this->FindView(kPopupMenu);
	aView->ResizeBy(180, 0);
	aView->MoveTo(kHorizMargin, myHeight - kControlsHeight + 5);
	((BMenuField*)aView)->SetDivider(50);

	/* 送信ボタンの配置を設定 */
	aView = this->FindView(kSendButton);
	aView->ResizeBy(50, 0);
	aView->MoveTo(
		myWidth - kHorizMargin - 50,
		myHeight - kVertMargin - aView->Frame().Height()
	);
	((BButton*)aView)->SetTarget(this);
	((BButton*)aView)->SetEnabled(false);
	
	return;
}

/*
 * メッセージ処理; MessengerView
 */
void
MessengerView::MessageReceived (BMessage* message)
{
	switch (message->what) {
	case APP_SELECTED:
		this->AppSelected();	break;
	case SEND_MESSAGE:
		this->SendMessage();	break;
	default:
		BView::MessageReceived(message);
	}
	
	return;
}

void
MessengerView::Pulse (void)
{
	this->CheckApps();
	return;
}

void
MessengerView::AppSelected (void)
{
	BButton*	theButton = (BButton*)this->FindView(kSendButton);
	BListView*	theList   = (BListView*)this->FindView(kListView);

	theButton->SetEnabled(theList->CurrentSelection() >= 0);
	
	return;
}

void
MessengerView::SendMessage (void)
{
	status_t	sts;
	team_id		theTeam;
	app_info	theInfo;
	BListView*	theList      = (BListView*)this->FindView(kListView);
	BMenuField*	theMenuField = (BMenuField*)this->FindView(kPopupMenu);
	BMenuItem*	theMenuItem;
	uint32		theCommand;
	
	/* 選択中のアプリケーションに対するBMessengerオブジェクトを生成 */
	theTeam = (team_id)fAppList->ItemAt(theList->CurrentSelection());
	be_roster->GetRunningAppInfo(theTeam, &theInfo);
	BMessenger	theMessenger(theInfo.signature, theTeam, &sts);
	if (sts != B_OK)
		goto err;
	
	/* ポップアップメニューで指定されたコマンドを取得 */
	theMenuItem = theMenuField->Menu()->FindMarked();
	if (strcmp(theMenuItem->Label(), kAboutLabel) == 0)
		theCommand = B_ABOUT_REQUESTED;
	else if (strcmp(theMenuItem->Label(), kQuitLabel) == 0)
		theCommand = B_QUIT_REQUESTED;
	else {
		sts = B_ERROR;
		goto err;
	}
	
	/* メッセージを送る */
	theMessenger.SendMessage(theCommand);
	
	return;
err:
	::Error("MessengerView::SendMessage", sts);
	return;
}

/*
 * アプリケーションの一覧表示; MessengerView
 */
void
MessengerView::CheckApps (void)
{
	BList	tmpList;

	/* 動作中のアプリケーションリストを取得 */
	be_roster->GetAppList(&tmpList);
	
	/* 現在表示しているものと違っていたら表示を更新 */
	if (tmpList.CountItems() != fAppList->CountItems())
		this->UpdateAppList(&tmpList);
	else {
		for (int32 i = 0, n = tmpList.CountItems(); i < n; ++i) {
			team_id	theTeam = (team_id)tmpList.ItemAt(i);
			
			if (theTeam != (team_id)fAppList->ItemAt(i)) {
				this->UpdateAppList(&tmpList);
				break;
			}
		}
	}
	
	return;
}

void
MessengerView::UpdateAppList (const BList* newList)
{
	BListView*	theListView = (BListView*)this->FindView(kListView);
	BListItem*	aListItem;
	app_info	anInfo;

	/* アプリケーションリストを更新 */
	*fAppList = *newList;
	
	/* 表示リストをクリア */
#ifdef oldSrc  /* '98. 1/25 (koga@ftgun.co.jp) */
	theListView->MakeEmpty();
#else
	while ((aListItem = theListView->RemoveItem((int32)0)) != NULL) {
		delete aListItem;
	}
#endif
	
	/* 各アプリケーションのシグネチャを表示リストに挿入 */
	for (int32 i = 0, n = newList->CountItems(); i < n; ++i) {
		team_id			theTeam = (team_id)newList->ItemAt(i);
		BStringItem*	newItem;
		
		if (be_roster->GetRunningAppInfo(theTeam, &anInfo) == B_OK) {
			newItem = new BStringItem(anInfo.signature);
			theListView->AddItem(newItem);
		}
	}
	
	/* 選択状態の解除 */
	this->AppSelected();
	
	return;
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * MessengerViewクラスの公開メソッド
 */
MessengerView::MessengerView (BRect frame, uint32 resizeMask)
	: BView(frame, "MessengerView", resizeMask, B_PULSE_NEEDED)
{
	fAppList = new BList();
	this->MakeContent();
}

MessengerView::~MessengerView (void)
{
	delete fAppList;
}


/*
 * End of File
 */
