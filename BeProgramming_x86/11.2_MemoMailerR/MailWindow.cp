/************************************************************************
 * $Id$
 * Copyright (c) 1998 Fort Gunnison, Inc.
 *
 * MailWindow.cp: MailWindowクラスの実装(改訂版)
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		May. 05, 1998
 * Last update:	May. 16, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "MailWindow.h"
#include "MailSender.h"
#include "MyMemoWindow.h"
#include "GUI/EntryListView.h"
#include "GUI/EntryListItem.h"

#include <interface/Button.h>
#include <interface/ScrollView.h>
#include <interface/StringView.h>
#include <interface/TextControl.h>
#include <support/Beep.h>
#include <support/Debug.h>

#include <string.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/* メッセージコード */
enum {
	SEND_MAIL		= 'send',
	CANCEL_SEND		= 'cncl',
	ENTRY_SELECTED	= 'esel',
	REMOVE_ENTRY	= 'rmfl'
};

/* レイアウト用の定数 */
const float	kMailWinWidth	= 240;
const float	kMailWinHeight	= 200;
const float	kMailWinLeft	= 150;
const float	kMailWinTop		= 150;
const BRect	kAddrFieldFrame	= BRect(20,  20,  220, 40);
const BRect	kCaptionFrame	= BRect(30,  50,  160, 68);
const BRect	kListBoxFrame	= BRect(30,  70,  145, 135);
const BRect kRemoveBtnFrame	= BRect(170, 110, 220, 110);
const BRect	kCancelBtnFrame	= BRect(95,  160, 145, 160);
const BRect	kOkBtnFrame		= BRect(165, 160, 215, 160);


/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/* 文字列定数 */
const char	kAddrFieldName[]	= "to addr";
const char	kAddrFieldLabel[]	= "Send To:";
const char	kListBoxCaption[]	= "Enclosures:";
const char	kListBoxName[]		= "attach files";
const char	kRemoveBtnName[]	= "remove button";
const char	kRemoveLabel[]		= "Remove";
const char	kCancelBtnName[]	= "cancel button";
const char	kCancelLabel[]		= "Cancel";
const char	kOkBtnName[]		= "ok button";
const char	kOkLabel[]			= "Send";


/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/* MailWindowクラスの非公開メソッド */
/*
 * メッセージ応答; MailWindow
 */
void
MailWindow::DispatchMessage (BMessage* message, BHandler* handler)
{
	/* Escapeボタンを押されたらキャンセル処理 */
	if (message->what == B_KEY_DOWN) {
		int32	theKeyCode;
		
		(void)message->FindInt32("raw_char", &theKeyCode);
		if (theKeyCode == B_ESCAPE) {
			this->Cancel();
			return;
		}
	}
	BWindow::DispatchMessage(message, handler);
	
	return;
}

void
MailWindow::MessageReceived (BMessage* message)
{
	/* ドラッグ＆ドロップ対応 */
	if (message->WasDropped()) {
		this->MessageDropped(message);
		return;
	}

	/* 普通のメッセージ */
	switch (message->what) {
	case SEND_MAIL:
		this->SendMail();		break;
	case CANCEL_SEND:
		this->Cancel();			break;
	case ENTRY_SELECTED:
		this->EntrySelected();	break;
	case REMOVE_ENTRY:
		this->RemoveEntry();	break;
	default:
		BWindow::MessageReceived(message);
	}
	
	return;
}

void
MailWindow::MessageDropped (BMessage* message)
{
	BPoint			theDropPoint = message->DropPoint();
	EntryListView*	theListView;

	/* ファイルを受け取ったらリストに追加 */
	theListView = (EntryListView*)this->FindView(kListBoxName);
	theListView->ConvertFromScreen(&theDropPoint);
	if (theListView->Bounds().Contains(theDropPoint)) {
		entry_ref	aRef;
		BEntry		anEntry;
		int32		theIndex = 0;
		
		while (message->FindRef("refs", theIndex++, &aRef) == B_OK) {
			if (anEntry.SetTo(&aRef, true) != B_OK
					|| anEntry.IsDirectory()) {
				::beep();
				continue;
			}
			theListView->CreateItem(aRef);
		}
	}
	
	return;
}

void
MailWindow::SendMail (void)
{
	BTextControl*	theTextControl;
	EntryListView*	theListView;
	const char*		toAddr;
	const char*		subject;
	const char*		body;
	
	/* 念のため */
	if (fMailSender->IsSending()) {
		::beep();
		return;
	}
	fMailSender->Init();

	/* 宛先アドレスを取得 */
	theTextControl = cast_as(this->FindView(kAddrFieldName), BTextControl);
	toAddr = theTextControl->Text();
	if (strlen(toAddr) == 0) {
		::beep();
		return;
	}
	
	/* メモウィンドウのタイトルを取得してサブジェクトに指定 */
	subject = fMemoWin->Title();
	
	/* メモのテキストを取得してメール本文に指定 */
	body = fMemoWin->Text();
	
	/* ヘッダ情報をセット */
	fMailSender->SetHeader(toAddr, NULL, subject);
	if (strlen(body) > 0)
		fMailSender->SetContent(body);

	/* 添付ファイルを追加 */
	theListView = cast_as(this->FindView(kListBoxName), EntryListView);
	for (int32 i = 0, n = theListView->CountItems(); i < n; ++i) {
		EntryListItem*	anItem;
		
		anItem = (EntryListItem*)theListView->ItemAt(i);
		fMailSender->AddEnclosure(*anItem->EntryRef());
	}

	/* 送信処理を実行 */
	fMailSender->Send();
	
	return;
}

void
MailWindow::Cancel (void)
{
	/* 送信処理を実行中ならユーザに確認 */
	if (fMailSender->IsSending() && !fMailSender->Cancel())
		return;  /* 強制終了をやめた */

	/* 自身に終了を通知 */
	this->PostMessage(B_QUIT_REQUESTED, this);
	
	return;
}

void
MailWindow::EntrySelected (void)
{
	BButton*	theButton = (BButton*)this->FindView(kRemoveBtnName);
	BListView*	theList   = (BListView*)this->FindView(kListBoxName);

	/* ファイルリストの選択状態に応じて"Remove"ボタンの状態をセット */	
	theButton->SetEnabled(theList->CurrentSelection() >= 0);
	
	return;
}

void
MailWindow::RemoveEntry (void)
{
	BButton*	theButton = (BButton*)this->FindView(kRemoveBtnName);
	BListView*	theList   = (BListView*)this->FindView(kListBoxName);
	BListItem*	theItem;

	/* 選択されているファイルをリストから削除 */	
	theItem = theList->RemoveItem(theList->CurrentSelection());
	delete theItem;
	theButton->SetEnabled(false);
	
	return;
}

/*
 * ビュー部品の管理; MailWindow
 */
void
MailWindow::MakeContent (void)
{
	BView*	backView;
	BView*	theSubView;

	/* 自身のサイズと位置を調節 */
	this->ResizeTo(kMailWinWidth, kMailWinHeight);
	this->MoveTo(kMailWinLeft, kMailWinTop);
	
	/* 背景用のビューを生成して貼りつける */
	backView  = new BView(
		this->Bounds(), B_EMPTY_STRING, B_FOLLOW_ALL_SIDES, B_WILL_DRAW
	);
	backView->SetViewColor(224, 224, 224);
	this->AddChild(backView);
	
	/* 宛先アドレスの入力フィールドを生成して貼りつける */
	theSubView = new BTextControl(
		kAddrFieldFrame, kAddrFieldName, kAddrFieldLabel, NULL, NULL
	);
	((BTextControl*)theSubView)->SetDivider(50);
	backView->AddChild(theSubView);
	((BTextControl*)theSubView)->MakeFocus();
	
	/* 添付書類用のリストボックスを生成して貼りつける */
	theSubView = new BStringView(
		kCaptionFrame, B_EMPTY_STRING, kListBoxCaption
	);
	backView->AddChild(theSubView);
	theSubView = new EntryListView(
		kListBoxFrame, kListBoxName,
		B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL_SIDES
	);
	((BListView*)theSubView)->SetSelectionMessage(
		new BMessage(ENTRY_SELECTED)
	);
	theSubView = new BScrollView(
		B_EMPTY_STRING, theSubView, B_FOLLOW_ALL_SIDES, 0,
		false, true, B_PLAIN_BORDER
	);
	backView->AddChild(theSubView);
	
	/* 添付書類の削除ボタンを生成して貼りつける */
	theSubView = new BButton(
		kRemoveBtnFrame, kRemoveBtnName, kRemoveLabel,
		new BMessage(REMOVE_ENTRY)
	);
	((BButton*)theSubView)->SetEnabled(false);
	backView->AddChild(theSubView);
	
	/* キャンセルボタンを生成して貼りつける */
	theSubView = new BButton(
		kCancelBtnFrame, kCancelBtnName, kCancelLabel,
		new BMessage(CANCEL_SEND)
	);
	backView->AddChild(theSubView);
	
	/* OKボタンを生成して貼りつける */
	theSubView = new BButton(
		kOkBtnFrame, kOkBtnName, kOkLabel,
		new BMessage(SEND_MAIL)
	);
	backView->AddChild(theSubView);
	((BButton*)theSubView)->MakeDefault(true);
	
	return;
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * MailWindowクラスの公開メソッド
 */
MailWindow::MailWindow (MyMemoWindow* inMemoWin)
	: BWindow(
		BRect(0, 0, 1, 1), B_EMPTY_STRING, B_MODAL_WINDOW,
		B_NOT_RESIZABLE|B_NOT_CLOSABLE|B_NOT_ZOOMABLE)
{
	this->MakeContent();
	fMemoWin    = inMemoWin;
	fMailSender = new MailSender(new BMessage(B_QUIT_REQUESTED), this);
}

MailWindow::~MailWindow (void)
{
	delete fMailSender;
}


/*
 * End of File
 */
