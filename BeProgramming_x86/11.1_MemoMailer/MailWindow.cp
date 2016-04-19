/************************************************************************
 * $Id$
 * Copyright (c) 1998 Fort Gunnison, Inc.
 *
 * MailWindow.cp: MailWindowクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		May. 05, 1998
 * Last update:	May. 17, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "MailWindow.h"
#include "MailSender.h"
#include "MyMemoWindow.h"

#include <interface/Button.h>
#include <interface/TextControl.h>
#include <support/Beep.h>

#include <string.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/* メッセージコード */
enum {
	SEND_MAIL	= 'send',
	CANCEL_SEND	= 'cncl'
};

/* レイアウト用の定数 */
const float	kMailWinWidth	= 240;
const float	kMailWinHeight	= 110;
const float	kMailWinLeft	= 150;
const float	kMailWinTop		= 150;
const BRect	kAddrFieldFrame	= BRect(20,  20, 220, 40);
const BRect	kCancelBtnFrame	= BRect(95,  70, 145, 70);
const BRect	kOkBtnFrame		= BRect(165, 70, 215, 70);


/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/* 文字列定数 */
const char	kAddrFieldName[]	= "to addr";
const char	kAddrFieldLabel[]	= "Send To:";
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
	switch (message->what) {
	case SEND_MAIL:
		this->SendMail();	break;
	case CANCEL_SEND:
		this->Cancel();		break;
	default:
		BWindow::MessageReceived(message);
	}
	
	return;
}

void
MailWindow::SendMail (void)
{
	BTextControl*	theTextControl;
	const char*		toAddr;
	const char*		subject;
	const char*		body;
	
	/* 念のため */
	if (fMailSender->IsSending()) {
		::beep();
		return;
	}
	
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

	/* コントロール部品をディム化 */	
	((BTextControl*)this->FindView(kAddrFieldName))->SetEnabled(false);
	((BButton*)this->FindView(kOkBtnName))->SetEnabled(false);
	
	/* 送信処理を実行 */
	fMailSender->SetHeader(toAddr, NULL, subject);
	fMailSender->SetContent(body);
	fMailSender->Send();
	
	return;
/*
 * 注意：SMTPサーバと直接通信する場合、多少時間がかかる。そのため、通信
 *		していることが分かるように、コントロール部品をディム化している。
 *		しかし、必要なメール関連のプリファレンスが設定されていない場合、
 *		現在の実装ではfMailSenderの送信スレッド内でアラートを出し、何も
 *		しないで復帰する。そのため、ディム化を解除する手段がない。本当
 *		は、送信スレッドではなく、MailSender::Send()メソッドの本体で
 *		チェックを行うようにして、送信できなかった場合は、そのことを検
 *		出してディム化を解除できるようにすべき。
 *		('98. 5/17, koga@ftgun.co.jp)
 */
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

/* MailWindowクラスの公開メソッド */
/*
 * 初期化と解放; MailWindow
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
