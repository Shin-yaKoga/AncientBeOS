/************************************************************************
 * $Id$
 * Copyright (c) 1998 Fort Gunnison, Inc.
 *
 * SimpleDialog.cp: SimpleDialogクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 27, 1998
 * Last update:	May. 04, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "SimpleDialog.h"
#include "KGUtility/kgDebug.h"

#include <app/Application.h>
#include <interface/TextControl.h>
#include <support/Debug.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/* 文字列定数 */
const char	kValueArg[]	= "value";


/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/*
 * SimpleDialogクラスの非公開メソッド
 */
void
SimpleDialog::Quit (void)
{
	be_app->SetCursor(B_HAND_CURSOR);
	BWindow::Quit();
	
	return;
}

void
SimpleDialog::DispatchMessage (BMessage* message, BHandler* handler)
{
	if (message->what == B_KEY_DOWN)
		this->CheckEscape(message);
	BWindow::DispatchMessage(message, handler);
	
	return;
}

void
SimpleDialog::MessageReceived (BMessage* message)
{
	switch (message->what) {
	case KGSD_OK:
		this->SendCompletionMessage();	break;
	case KGSD_CANCEL:
		this->Quit();					break;
	case B_KEY_DOWN:
		this->CheckEscape(message);		break;
	default:
		BWindow::MessageReceived(message);
	}
	
	return;
}

void
SimpleDialog::SendCompletionMessage (void)
{
	status_t	sts;
	BMessage	theMessage(fCompletionMsg);
	
	theMessage.AddString(kValueArg, fInputField->Text());
	sts = fTarget.SendMessage(&theMessage);
	if (sts != B_OK)
		goto err;
	be_app->SetCursor(B_HAND_CURSOR);  /* !! */
	this->Quit();
	
	return;
err:
	::Error("SimpleDialog::SendCompletionMessage", sts);
	return;
}

void
SimpleDialog::CheckEscape (BMessage* message)
{
	int32	theKeyCode;
	
	(void)message->FindInt32("raw_char", &theKeyCode);
	if (theKeyCode == B_ESCAPE)
		this->Quit();
	
	return;
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/* SimpleDialogクラスの公開メソッド */
/*
 * 初期化と解放; SimpleDialog
 */
SimpleDialog::SimpleDialog (BRect frame, const char* title,
		window_type type, uint32 resizeMask, uint32 completionMsg)
	: BWindow(frame, title, type, resizeMask)
{
	fInputField    = NULL;
	fCompletionMsg = completionMsg;
}

SimpleDialog::~SimpleDialog (void)
{  /* do nothing */  }

void
SimpleDialog::SetContent (BView* inContent, const char* inFieldName)
{
	BView*	theField;
	
	this->AddChild(inContent);
	theField = this->FindView(inFieldName);
	fInputField = cast_as(theField, BTextControl);
	
	return;
}

/*
 * ダイアログの表示; SimpleDialog
 */
status_t
SimpleDialog::Go (const BMessenger& inTarget)
{
	ASSERT(fInputField != NULL);

	fTarget = inTarget;
	fInputField->MakeFocus();
	this->Show();
	
	return B_OK;
}


/*
 * End of File
 */
