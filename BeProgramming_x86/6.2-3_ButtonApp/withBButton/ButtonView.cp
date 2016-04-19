/************************************************************************
 * $Id$
 * Copyright (c) 1997, Fort Gunnison, Inc.
 *
 * ButtonView.cp: ButtonViewクラスの実装(標準部品を使う版)
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 13, 1997
 * Last update:	Dec. 14, 1997
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "ButtonView.h"
#include <interface/Button.h>
#include <interface/StringView.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * ButtonViewクラスの公開メソッド
 */
ButtonView::ButtonView (BRect frame, const char* title,
		uint32 resizeMask, uint32 flags)
	: BView(frame, title, resizeMask, flags)
{
	BButton*	aButton;
	BMessage*	aMessage;

	/* Hideボタンを作って貼る */
	aButton  = new BButton(
		BRect(20, 30, 70, 60),
		"hide button", "Hide", NULL
	);
	aMessage = new BMessage(B_MINIMIZE);
	aMessage->AddBool("minimize", true);
	aButton->SetMessage(aMessage);
	this->AddChild(aButton);
	
	/* Closeボタンを作って貼る */
	aButton  = new BButton(
		BRect(90, 30, 140, 60),
		"close button", "Close", NULL
	);
	aMessage = new BMessage(B_QUIT_REQUESTED);
	aButton->SetMessage(aMessage);
	this->AddChild(aButton);
	
	/* 文字列表示用のビューを作って貼る */
	this->AddChild(
		new BStringView(
			BRect(10, 10, 120, 20), "caption",
			"This is the my 4th app."
		)
	);
	
	/* 背景色をセット */
	this->SetViewColor(0xD0, 0xD0, 0xD0);
}

ButtonView::~ButtonView (void)
{  /* do nothing */  }


/*
 * End of File
 */
