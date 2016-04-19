/************************************************************************
 * $Id$
 * Copyright (c) 1998 Fort Gunnison, Inc.
 *
 * ScriptEngine.cp: ScriptEngineクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 05, 1998
 * Last update:	Apr. 23, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "ScriptEngine.h"
#include "KGUtility/kgAlert.h"
#include "KGUtility/kgDebug.h"

#include <app/Roster.h>
#include <support/Debug.h>

#include <stdlib.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/* 文字列定数 */
const char	kData[]		= "data";
const char	kFrame[]	= "Frame";
const char	kResult[]	= "result";
const char	kValue[]	= "Value";
const char	kView[]		= "View";
const char	kWindow[]	= "Window";
const char	kMsngr[]	= "Messenger";


/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/* ScriptEngineクラスの公開メソッド */
/*
 * 初期化と解放; ScriptEngine
 */
ScriptEngine::ScriptEngine (void)
{
	fTargetApp = -1;  /* 無効値 */
}

ScriptEngine::~ScriptEngine (void)
{
#if DEBUG
	if (fTargetApp >= 0) {
		::Error("ScriptEngine::~ScriptEngine", B_ERROR);
		this->EndTell();
	}
#endif
	/* do nothing */
}

/*
 * アプリケーション操作の開始と終了; ScriptEngine
 */
status_t
ScriptEngine::TellApplication (const char inAppSig[])
{
	ASSERT(fTargetApp < 0);
	
	status_t	sts;
	char		theAppMimeType[B_MIME_TYPE_LENGTH];
	app_info	theAppInfo;
	
	/* アプリケーション情報を取得 */
	sprintf(theAppMimeType, "application/%s", inAppSig);
	sts = be_roster->GetAppInfo(theAppMimeType, &theAppInfo);
	
	/* 動作していなければ起動する */
	if (sts != B_OK) {
		sts = be_roster->Launch(
			theAppMimeType, (BMessage*)NULL, &theAppInfo.team);
		if (sts != B_OK) {
			char	theMsg[256];
			
			sprintf(theMsg,
				"Sorry, but there are no such app '%s'", inAppSig);
			::KGAlert(theMsg);
		}
	}
	
	/* チームIDを記録 */
	if (sts == B_OK)
		fTargetApp = theAppInfo.team;
	
	return sts;
}

void
ScriptEngine::EndTell (void)
{
	ASSERT(fTargetApp >= 0);
	
	fTargetApp = -1;
	return;
}

/*
 * メッセージ送信オブジェクトの生成; ScriptEngine
 */
status_t
ScriptEngine::GetRmtApplication (BMessenger* outMessenger)
{
	ASSERT(fTargetApp >= 0);

	status_t	sts;
	BMessenger	theRmtApp(NULL, fTargetApp, &sts);
	
	if (sts == B_OK)
		*outMessenger = theRmtApp;
	
	return sts;
}

status_t
ScriptEngine::GetRmtWindow (
	const char inWindowInfo[], BMessenger* outMessenger)
{
	ASSERT(fTargetApp >= 0);

	status_t	sts;
	BMessage	theRequest(B_GET_PROPERTY), reply;
	BMessenger	appAgent;
	
	/* ウィンドウのスペシファイアを作成 */
	sts = theRequest.AddSpecifier(kMsngr);
	if (sts != B_OK)
		goto err;
	sts = AddSpecifierTo(kWindow, inWindowInfo, &theRequest);
	if (sts != B_OK)
		goto err;

	/* Messengerを返すようにアプリケーションへ要求 */
	sts = this->GetRmtApplication(&appAgent);
	if (sts != B_OK)
		goto err;  /* アプリケーションが見つからない */
	sts = appAgent.SendMessage(&theRequest, &reply);
	if (sts != B_OK)
		goto err;

	/* 成功したら出力バッファにセット */
	sts = reply.FindMessenger(kResult, outMessenger);
	if (sts != B_OK) {
		char	theMsg[256];
		
		sprintf(theMsg,
			"Sorry, but there are no such window '%s'", inWindowInfo
		);
		::KGAlert(theMsg);
	}
	
	return B_OK;
err:
	::Error("ScriptEngine::GetRmtWindow", sts);
	return sts;
}

status_t
ScriptEngine::GetRmtControl (const char inWindowInfo[],
	const char inControlInfo[], BMessenger* outMessenger)
{
	ASSERT(fTargetApp >= 0);
	
	status_t	sts;
	BMessage	theRequest(B_GET_PROPERTY), reply;
	BMessenger	winAgent;
	
	/* コントロールのスペシファイアを作成 */
	sts = theRequest.AddSpecifier(kMsngr);
	if (sts != B_OK)
		goto err;
	sts = AddSpecifierTo(kView, inControlInfo, &theRequest);
	if (sts != B_OK)
		goto err;

	/* Messengerを返すようにウィンドウへ要求 */
	sts = this->GetRmtWindow(inWindowInfo, &winAgent);
	if (sts != B_OK)
		goto err;  /* ウィンドウの取得に失敗 */
	else if (! winAgent.IsValid())
		return B_OK;  /* 遠隔ウィンドウが見つからない */
	sts = winAgent.SendMessage(&theRequest, &reply);
	if (sts != B_OK)
		goto err;

	/* 成功したら出力バッファにセット */
	sts = reply.FindMessenger(kResult, outMessenger);
	if (sts != B_OK) {
		char	theMsg[256];
		
		sprintf(theMsg,
			"Sorry, but there are no such view '%s'", inControlInfo
		);
		::KGAlert(theMsg);
	}
	
	return B_OK;
err:
	::Error("ScriptEngine::GetRmtControl", sts);
	return sts;
}

/*
 * ユーティリティ; ScriptEngine
 */
status_t
ScriptEngine::AddSpecifierTo (const char inPropertyName[],
	const char inPropertyInfo[], BMessage* ioMessage)
{
	status_t	sts = B_ERROR;

	if (inPropertyInfo[0] == '#')  /* インデックス指定 */
		sts = ioMessage->AddSpecifier(
			inPropertyName, atol(inPropertyInfo + 1)
		);
	else  /* 名前指定 */
		sts = ioMessage->AddSpecifier(inPropertyName, inPropertyInfo);

	return sts;
}

status_t
ScriptEngine::MoveWindow (const BMessenger& inRmtWindow, BPoint inNewPoint)
{
	status_t	sts;
	BMessage	theRequest, reply;
	BRect		theFrame;

	/* ウィンドウのフレーム矩型を取得 */
	theRequest.what = B_GET_PROPERTY;
	sts = theRequest.AddSpecifier(kFrame);
	if (sts != B_OK)
		goto err;
	sts = inRmtWindow.SendMessage(&theRequest, &reply);
	if (sts != B_OK)
		goto err;
	sts = reply.FindRect(kResult, &theFrame);
	if (sts != B_OK)
		goto err;

	/* inNewPointに始点を合わせてフレーム矩型を変更 */
	theFrame.OffsetTo(inNewPoint);
	theRequest.what = B_SET_PROPERTY;
	sts = theRequest.AddRect(kData, theFrame);
	if (sts != B_OK)
		goto err;
	sts = inRmtWindow.SendMessage(&theRequest);
	if (sts != B_OK)
		goto err;
	
	return B_OK;
err:
	::Error("ScriptEngine::MoveWindow", sts);
	return sts;
}

status_t
ScriptEngine::PushButton (const BMessenger& inRmtControl)
{
	status_t	sts;
	BMessage	theRequest(B_SET_PROPERTY);

	/* メッセージのスペシファイアをセット */
	sts = theRequest.AddSpecifier(kValue);
	if (sts != B_OK)
		goto err;

	/* コントロールの値を1にセット */
	sts = theRequest.AddInt32(kData, 1);
	if (sts != B_OK)
		goto err;
	sts = inRmtControl.SendMessage(&theRequest);
	if (sts != B_OK)
		goto err;

	/* コントロールの値を0にセット */
	sts = theRequest.ReplaceInt32(kData, 0);
	if (sts != B_OK)
		goto err;
	sts = inRmtControl.SendMessage(&theRequest);
	if (sts != B_OK)
		goto err;
	
	return B_OK;
err:
	::Error("ScriptEngine::PushButton", sts);
	return sts;
}


/*
 * End of File
 */
