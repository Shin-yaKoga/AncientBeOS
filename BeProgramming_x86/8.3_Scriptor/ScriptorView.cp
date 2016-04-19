/************************************************************************
 * $Id$
 * Copyright (c) 1998 Fort Gunnison, Inc.
 *
 * ScriptorView.cp: ScriptorViewクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 04, 1998
 * Last update:	Apr. 28, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "ScriptorView.h"
#include "ScriptEngine.h"
#include "KGUtility/kgAlert.h"
#include "KGUtility/kgDebug.h"

#include <interface/Box.h>
#include <interface/Button.h>
#include <interface/RadioButton.h>
#include <interface/TextControl.h>
#include <support/Debug.h>

#include <stdlib.h>
#include <string.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/* ビュー配置用の定数 */
float	kHorizMargin = 5;
float	kVertMargin  = 5;

/* メッセージのコード */
enum {
	EXEC_SCRIPT		= 'exec',
	SEL_SHOW_ABOUT	= 'ssab',
	SEL_QUIT		= 'sqit',
	SEL_MOVE_WIN	= 'smvw',
	SEL_PUSH_BTN	= 'spbt'
};


/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/* 文字列定数 */
const char	kEditAppFile[]  = "app_file efield";
const char	kEditWindow[]   = "window_title efield";
const char	kBox[]          = "box";
const char	kButtonExec[]   = "exec button";
const char	kRadioAbout[]   = "about rbutton";
const char	kRadioQuit[]    = "quit rbutton";
const char	kRadioMoveWin[] = "move_win rbutton";
const char	kRadioPushBtn[] = "push_btn rbutton";
const char	kEditPoint[]    = "window_pos efield";
const char	kEditButton[]   = "button efield";

const char	kInvalWinMsg[]		=
	"You must specify window by index such as \"#0\".";
const char	kSameLooperMsg[]	=
	"Sorry, but I cannot send message to myself.";


/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/* ScriptorViewクラスの非公開メソッド */
/*
 * 描画処理; ScriptorView
 */
void
ScriptorView::AttachedToWindow (void)
{
	this->AdjustContent();
	return;
}

void
ScriptorView::MakeContent (void)
{
	ASSERT(this->CountChildren() == 0);
	
	BTextControl*	aEditField;
	BBox*			aBox;
	BButton*		aButton;

	/* アプリケーションのファイル名入力フィールドを生成 */
	aEditField = new BTextControl(
		BRect(0, 0, 110, 0),
		kEditAppFile,
		"application:",
		NULL,
		NULL,
		B_FOLLOW_LEFT_RIGHT|B_FOLLOW_TOP
	);
	this->AddChild(aEditField);
	
	/* ウィンドウ指定の入力フィールドを生成 */
	aEditField = new BTextControl(
		BRect(0, 0, 85, 0),
		kEditWindow,
		"window:",
		NULL,
		NULL,
		B_FOLLOW_LEFT_RIGHT|B_FOLLOW_TOP
	);
	this->AddChild(aEditField);
	
	/* アクション内容の入力ボックスを生成 */
	aBox = new BBox(
		BRect(0, 0, 0, 0),
		kBox,
		B_FOLLOW_ALL_SIDES
	);
	aBox->SetLabel("action");
	this->MakeActionBox(aBox);
	this->AddChild(aBox);
	
	/* スクリプト実行指定ボタンを生成 */
	aButton = new BButton(
		BRect(0, 0, 0, 0),
		kButtonExec,
		"Exec",
		new BMessage(EXEC_SCRIPT),
		B_FOLLOW_RIGHT|B_FOLLOW_BOTTOM
	);
	this->AddChild(aButton);
	
	this->SetViewColor(0xD0, 0xD0, 0xD0);
	
	return;
}

void
ScriptorView::MakeActionBox (BBox* inBox)
{
	ASSERT(inBox->CountChildren() == 0);
	
	BRadioButton*	aRadioButton;
	BTextControl*	aEditField;
	
	/* アバウト表示指定のラジオボタン */
	aRadioButton = new BRadioButton(
		BRect(0, 0, 0, 0),
		kRadioAbout,
		"Show AboutBox",
		new BMessage(SEL_SHOW_ABOUT)
	);
	inBox->AddChild(aRadioButton);
	
	/* 終了指定のラジオボタン */
	aRadioButton = new BRadioButton(
		BRect(0, 0, 0, 0),
		kRadioQuit,
		"Quit",
		new BMessage(SEL_QUIT)
	);
	inBox->AddChild(aRadioButton);
	
	/* ウィンドウ移動指定のラジオボタン */
	aRadioButton = new BRadioButton(
		BRect(0, 0, 0, 0),
		kRadioMoveWin,
		"Move Window",
		new BMessage(SEL_MOVE_WIN)
	);
	inBox->AddChild(aRadioButton);
	aEditField = new BTextControl(
		BRect(0, 0, 30, 0),
		kEditPoint,
		"to:",
		NULL,
		NULL,
		B_FOLLOW_LEFT_RIGHT|B_FOLLOW_TOP
	);
	inBox->AddChild(aEditField);
	
	/* ボタン押下指定のラジオボタン */
	aRadioButton = new BRadioButton(
		BRect(0, 0, 0, 0),
		kRadioPushBtn,
		"Push Button",
		new BMessage(SEL_PUSH_BTN)
	);
	inBox->AddChild(aRadioButton);
	aEditField = new BTextControl(
		BRect(0, 0, 64, 0),
		kEditButton,
		"which:",
		NULL,
		NULL,
		B_FOLLOW_LEFT_RIGHT|B_FOLLOW_TOP
	);
	inBox->AddChild(aEditField);
	
	return;
}

void
ScriptorView::AdjustContent (void)
{
	BView*	aView;
	BRect	myBounds = this->Bounds();
	float	myWidth  = myBounds.Width();
	float	myHeight = myBounds.Height();
	float	theTop;
	
	/* アプリケーションのファイル名入力フィールドのサイズと位置を調整 */
	aView = this->FindView(kEditAppFile);
	aView->ResizeTo(
		myWidth - kHorizMargin * 2, aView->Bounds().Height()
	);
	aView->MoveTo(kHorizMargin, kVertMargin);
	theTop = aView->Frame().bottom + kVertMargin;
	
	/* ウィンドウ指定の入力フィールドのサイズと位置を調整 */
	aView = this->FindView(kEditWindow);
	aView->ResizeTo(
		myWidth - kHorizMargin * 2 - 13, aView->Bounds().Height()
	);
	aView->MoveTo(kHorizMargin + 13, theTop);
	theTop  = aView->Frame().bottom + kVertMargin;
	
	/* アクション内容の入力ボックスのサイズと位置を調整 */
	aView = this->FindView(kBox);
	aView->ResizeTo(
		myWidth - kHorizMargin * 2,
		myHeight - theTop - kVertMargin - 30
	);
	aView->MoveTo(kHorizMargin, theTop);
	this->AdjustActionBox((BBox*)aView);
	
	/* スクリプト実行指定ボタンのサイズと位置を調整 */
	aView = this->FindView(kButtonExec);
	aView->ResizeBy(50, 0);
	aView->MoveTo(
		myWidth - aView->Bounds().Width() - kHorizMargin,
		myHeight - aView->Bounds().Height() - kVertMargin
	);
	((BButton*)aView)->SetTarget(this);
	
	return;
}

void
ScriptorView::AdjustActionBox (BBox* inBox)
{
	BView*	aView;
	BRect	theBounds = inBox->Bounds();
	float	theWidth  = theBounds.Width();
	float	theHeight = theBounds.Height();
	float	theTop, theLeft;
	
	/* アバウト表示指定のラジオボタンのサイズと位置を調整 */
	aView = inBox->FindView(kRadioAbout);
	aView->ResizeBy(100, 0);
	aView->MoveTo(kHorizMargin + 5, kVertMargin + 10);
	((BRadioButton*)aView)->SetTarget(this);
	theTop = aView->Frame().bottom + kVertMargin;
	
	/* 終了指定のラジオボタンのサイズと位置を調整 */
	aView = inBox->FindView(kRadioQuit);
	aView->ResizeBy(40, 0);
	aView->MoveTo(kHorizMargin + 5, theTop);
	((BRadioButton*)aView)->SetTarget(this);
	theTop = aView->Frame().bottom + kVertMargin;
	
	/* ウィンドウ移動指定のラジオボタンのサイズと位置を調整 */
	aView = inBox->FindView(kRadioMoveWin);
	aView->ResizeBy(90, 0);
	aView->MoveTo(kHorizMargin + 5, theTop);
	((BRadioButton*)aView)->SetTarget(this);
	theLeft = aView->Frame().right + kHorizMargin;
	
	/* ウィンドウの移動位置入力フィールドを調整 */
	aView = inBox->FindView(kEditPoint);
	aView->ResizeTo(
		theWidth - theLeft - kHorizMargin - 3,
		aView->Bounds().Height()
	);
	aView->MoveTo(theLeft, theTop - 1);
	theTop = aView->Frame().bottom + kVertMargin;
	
	/* ボタン押下指定のラジオボタンのサイズと位置を調整 */
	aView = inBox->FindView(kRadioPushBtn);
	aView->ResizeBy(73, 0);
	aView->MoveTo(kHorizMargin + 5, theTop);
	((BRadioButton*)aView)->SetTarget(this);
	theLeft = aView->Frame().right + kHorizMargin;
	
	/* ボタン指定の入力フィールドを調整 */
	aView = inBox->FindView(kEditButton);
	aView->ResizeTo(
		theWidth - theLeft - kHorizMargin - 3,
		aView->Bounds().Height()
	);
	aView->MoveTo(theLeft, theTop - 1);
	
	/* アバウト表示指定を選択状態にセット(初期状態) */
	((BRadioButton*)inBox->FindView(kRadioAbout))->SetValue(1);
	fCurrAction = SEL_SHOW_ABOUT;
	
	return;
}

/*
 * メッセージ処理; ScriptorView
 */
void
ScriptorView::MessageReceived (BMessage* message)
{
	switch (message->what) {
	case SEL_SHOW_ABOUT:
	case SEL_QUIT:
	case SEL_MOVE_WIN:
	case SEL_PUSH_BTN:
		fCurrAction = message->what;	break;
	case EXEC_SCRIPT:
		this->ExecScript();				break;
	default:
		BView::MessageReceived(message);
	}
	
	return;
}

void
ScriptorView::MouseDown (BPoint /* where */)
{
	this->MakeFocus();
	return;
}

void
ScriptorView::ExecScript (void)
{
	BTextControl*	theAppFileField;

	/* アプリケーションの操作を開始 */
	theAppFileField = (BTextControl*)this->FindView(kEditAppFile);
	if (fEngine->TellApplication(theAppFileField->Text()) != B_OK)
		return;  /* 開始できない */

	/* スクリプトを実行 */
	switch (fCurrAction) {
	case SEL_SHOW_ABOUT:
		this->DoShowAbout();	break;
	case SEL_QUIT:
		this->DoQuit();			break;
	case SEL_MOVE_WIN:
		this->DoMoveWindow();	break;
	case SEL_PUSH_BTN:
		this->DoPushButton();	break;
	}

	/* アプリケーションの操作を終了 */
	fEngine->EndTell();
	
	return;
}

/*
 * スクリプト実行; ScriptorView
 */
void
ScriptorView::DoShowAbout (void)
{
	this->SendCommonMessage(B_ABOUT_REQUESTED);
	return;
}

void
ScriptorView::DoQuit (void)
{
	this->SendCommonMessage(B_QUIT_REQUESTED);
	return;
}

void
ScriptorView::DoMoveWindow (void)
{
	status_t		sts;
	const char*		theWinStr;
	BTextControl*	thePointField;
	char			strBuf[32+1];
	char*			commaPos;
	BPoint			thePoint;
	BMessenger		theMessenger;

	/* 送り先のウィンドウ指定文字列を取得 */
	theWinStr = this->GetWindowInfo();
	if (theWinStr == NULL)
		return;  /* 取得できない */

	/* 移動先の座標位置を取得 */
	thePointField = (BTextControl*)this->FindView(kEditPoint);
	strncpy(strBuf, thePointField->Text(), sizeof(strBuf) - 1);
	strBuf[sizeof(strBuf) - 1] = '\0';
	commaPos = strchr(strBuf, ',');
	if (commaPos == NULL) {
		sts = B_ERROR;
		goto err;
	}
	*commaPos = '\0';
	thePoint.x = atol(strBuf);
	thePoint.y = atol(commaPos + 1);

	/* 移動操作を実行 */
	sts = fEngine->GetRmtWindow(theWinStr, &theMessenger);
	if (sts != B_OK)
		goto err;
	else if (! theMessenger.IsValid())
		return;  /* 遠隔ウィンドウが見つからない */
	sts = ScriptEngine::MoveWindow(theMessenger, thePoint);
	if (sts != B_OK)
		goto err;
	
	return;
err:
	::Error("ScriptorView::DoMoveWindow", sts);
	return;
}

void
ScriptorView::DoPushButton (void)
{
	status_t		sts;
	const char*		theWinStr;
	const char*		theButtonStr;
	BTextControl*	theButtonField;
	BMessenger		theMessenger;

	/* 送り先のウィンドウ指定文字列を取得 */
	theWinStr = this->GetWindowInfo();
	if (theWinStr == NULL)
		return;  /* 取得できない */

	/* 送り先のコントロール指定文字列を取得 */
	theButtonField = (BTextControl*)this->FindView(kEditButton);
	theButtonStr = theButtonField->Text();
	if (theButtonStr[0] == '\0' || theButtonStr[0] == '#') {
		sts = B_ERROR;
		goto err;  /* 名前指定されていない */
	}

	/* ボタンクリック操作を実行 */
	sts = fEngine->GetRmtControl(theWinStr, theButtonStr, &theMessenger);
	if (sts != B_OK)
		goto err;
	else if (! theMessenger.IsValid())
		return;  /* 遠隔ボタンが見つからない */
	sts = ScriptEngine::PushButton(theMessenger);
	if (sts != B_OK)
		goto err;
	
	return;
err:
	::Error("ScriptorView::DoPushButton", sts);
	return;
}

void
ScriptorView::SendCommonMessage (uint32 theCommand)
{
	status_t		sts;
	BTextControl*	theWindowField;
	const char*		theWinStr;
	BMessenger		theMessenger;
	
	/* ウィンドウの指定文字列を取得 */
	theWindowField = (BTextControl*)this->FindView(kEditWindow);
	theWinStr = theWindowField->Text();
	if (theWinStr[0] != '\0' && theWinStr[0] != '#') {
		::KGAlert(kInvalWinMsg);
		return;  /* インデックス指定されていない */
	}
	
	/* 自分自身に送ろうとする場合は拒否 */
	if (theWinStr[0] != '\0') {
		BMessenger	appAgent;
		
		(void)fEngine->GetRmtApplication(&appAgent);
		if (appAgent.IsTargetLocal() && strcmp(theWinStr, "#0") == 0) {
			::KGAlert(kSameLooperMsg);
			return;
		}
	}

	/* ウィンドウ指定の有無に応じて送り先を決定 */
	if (theWinStr[0] == '\0')
		sts = fEngine->GetRmtApplication(&theMessenger);
	else
		sts = fEngine->GetRmtWindow(theWinStr, &theMessenger);
	if (sts != B_OK)
		goto err;
	if (! theMessenger.IsValid())
		return;  /* 遠隔オブジェクトが見つからない */
	
	/* 指定されたコマンドを送付 */
	sts = theMessenger.SendMessage(theCommand);
	if (sts != B_OK)
		goto err;
	
	return;
err:
	::Error("ScriptorView::SendCommonMessage", sts);
	return;
}

const char*
ScriptorView::GetWindowInfo (void)
{
	const char*		theStr;
	BTextControl*	theWindowField;

	/* ウィンドウ指定文字列を取得 */	
	theWindowField = (BTextControl*)this->FindView(kEditWindow);
	theStr = theWindowField->Text();
	
	/* 指定内容が妥当かチェック */
    if (theStr[0] == '\0')
        goto err;		/* ウィンドウが指定されていない */
    else if (theStr[0] != '#') {
        ::KGAlert(kInvalWinMsg);
        theStr = NULL;	/* インデックス指定されていない */
    } else {  /* 自分自身に送ろうとする場合は拒否 */
		BMessenger		appAgent;

		(void)fEngine->GetRmtApplication(&appAgent);
		if (appAgent.IsTargetLocal() && strcmp(theStr, "#0") == 0) {
			::KGAlert(kSameLooperMsg);
            theStr = NULL;
		}
	}
	
	return theStr;
err:
	::Error("ScriptorView::GetWindowInfo", B_ERROR);
	return NULL;
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * ScriptorViewクラスの公開メソッド
 */
ScriptorView::ScriptorView (BRect frame, uint32 resizeMask)
	: BView(frame, "ScriptorView", resizeMask, B_WILL_DRAW)
{
	this->MakeContent();
	fCurrAction = 0;  /* 無効値 */
	fEngine = new ScriptEngine();
}

ScriptorView::~ScriptorView (void)
{
	delete fEngine;
}


/*
 * End of File
 */
