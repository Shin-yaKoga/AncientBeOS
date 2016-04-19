/************************************************************************
 * $Id$
 * Copyright (c) 1998 Fort Gunnison, Inc.
 *
 * MakeResApp.cp: MakeResAppクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Apr. 22, 1998
 * Last update:	May. 17, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "MakeResApp.h"
#include "KGUtility/kgDebug.h"

#include <interface/TextControl.h>
#include <storage/Directory.h>
#include <storage/FilePanel.h>
#include <storage/Resources.h>

#include <stdio.h>
#include <stdlib.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/* MakeResAppクラスの非公開メソッド */
/*
 * 起動と終了; MakeResApp
 */
void
MakeResApp::ArgvReceived (int32 argc, char** argv)
{
	status_t	sts;
	entry_ref	theRef;
	
	/* 引数の数をチェック */
	if (argc != 2) {
		printf("usage: MakeRes <file>.\n");
		exit(1);
	}

	/* ファイル名を受け取ったらリソースファイルを作成 */
	sts = get_ref_for_path(argv[1], &theRef);
	if (sts == B_OK) {
		sts = this->MakeResFile(theRef, false);
		if (sts == B_FILE_EXISTS)
			printf(
				"Sorry, but the file '%s' is already exist.\n", argv[1]);
	}

	/* 成功したらその旨を記録 */
	if (sts == B_OK)
		fArgReceived = true;
	
	return;
}

void
MakeResApp::ReadyToRun (void)
{
	/* コマンド引数を処理済みなら終了 */
	if (fArgReceived)
		(void)this->PostMessage(B_QUIT_REQUESTED, this);
	else
		this->OpenSavePanel();
	
	return;
}

/*
 * メッセージ応答; MakeResApp
 */
void
MakeResApp::MessageReceived (BMessage* message)
{
	switch (message->what) {
	case B_SAVE_REQUESTED:
		this->SaveRequested(message);
		break;
	case B_CANCEL:
		(void)this->PostMessage(B_QUIT_REQUESTED, this);
		break;  /* 保存ダイアログが閉じたので終了 */
	default:
		BApplication::MessageReceived(message);
	}
	
	return;
}

void
MakeResApp::SaveRequested (BMessage* message)
{
	status_t	sts;
	entry_ref	theRef, theDirRef;
	BDirectory	theDir;
	BEntry		theEntry;
	char*		theFileName;

	/* セーブ先のファイル情報を作成 */
	sts = message->FindRef("directory", &theDirRef);
	if (sts != B_OK)
		goto err;
	sts = message->FindString("name", &theFileName);
	if (sts != B_OK)
		goto err;
	sts = theDir.SetTo(&theDirRef);
	if (sts != B_OK)
		goto err;
	sts = theEntry.SetTo(&theDir, theFileName);
	if (sts != B_OK)
		goto err;
	sts = theEntry.GetRef(&theRef);
	if (sts != B_OK)
		goto err;

	/* 受け取った情報に応じてリソースファイルを作成 */
	sts = this->MakeResFile(theRef, true);
	if (sts != B_OK)
		goto err;
	
	return;
err:
	::Error("MakeResApp::RefsReceived", sts);
	return;
}

/*
 * リソースファイルの作成; MakeResApp
 */
status_t
MakeResApp::MakeResFile (const entry_ref& inRef, bool clobber)
{
	status_t	sts = B_OK;
	BFile		theFile;
	
	/* 既存ファイルの有無をチェック */
	if (theFile.SetTo(&inRef, B_READ_ONLY) == B_OK) {
		if (! clobber)  /* 上書きしないモード */
			sts = B_FILE_EXISTS;
		else {  /* 上書きモードなら既存ファイルを削除 */
			BEntry	theEntry(&inRef);
			
			sts = theEntry.Remove();
			if (sts == B_OK)
				theFile.Unset();
		}
	}
	
	/* 新しくファイルを作ってリソースファイル化 */
	if (sts == B_OK) {
		BResources	newResFile;
	
		sts = theFile.SetTo(&inRef, B_READ_WRITE|B_CREATE_FILE);
		if (sts != B_OK)
			goto err;
		sts = newResFile.SetTo(&theFile, true);
		if (sts != B_OK)
			goto err;
	}
	
	return sts;
err:
	Error("MakeResApp::MakeResFile", sts);
	return sts;
}

void
MakeResApp::OpenSavePanel (void)
{
	BFilePanel*		thePanel;
	BWindow*		theWindow;
	BTextControl*	theEditField;
	
	thePanel  = new BFilePanel(B_SAVE_PANEL);
	theWindow = thePanel->Window();
	theWindow->Lock();
	theEditField = cast_as(
		thePanel->Window()->FindView("text view"), BTextControl
	);
	theEditField->SetText(".rsrc");
	theWindow->Unlock();
	thePanel->Show();
	
	return;
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * MakeResAppクラスの公開メソッド
 */
MakeResApp::MakeResApp (void)
	: BApplication("application/x-vnd.FtGUN-MakeRes")
{
	fArgReceived = false;
}

MakeResApp::~MakeResApp (void)
{  /* do nothing */  }


/*
 * End of File
 */
