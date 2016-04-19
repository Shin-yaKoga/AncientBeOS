/************************************************************************
 * $Id$
 * Copyright (c) 1997-1998 Fort Gunnison, Inc.
 *
 * ButtonView.cp: ButtonViewクラスの実装(標準部品を使わない版)
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 13, 1997
 * Last update:	Feb. 09, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "ButtonView.h"
#include "KGCircle.h"
#include <interface/Window.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/* 定数定義 */
const int32	kRectArea   = 1;
const int32	kCircleArea = 2;


/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/* ボタン処理用のオブジェクト */
const BRect		kHotSpotRect(110, 30, 140, 60);
const KGCircle	kHotSpotCircle(BPoint(30, 45), 15);


/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/* ButtonViewクラスの非公開メソッド */
/*
 * 描画処理; ButtonView
 */
void
ButtonView::Draw (BRect /* updateRect */)
{
	/* ボタンなどを描画 */
	this->DrawString("This is the my 3rd app.", BPoint(10, 10));
	this->SetHighColor(255, 0, 0);
	this->FillRect(kHotSpotRect);
	kHotSpotCircle.DrawOn(this);
	
	return;
}

/*
 * マウス応答; ButtonView
 */
void
ButtonView::MouseDown (BPoint where)
{
	uint32	buttons = 0;
	int32	theInt, theHotSpot;
	bool	currInside = true;
	bool	prevInside = false;
	
	/* 現在押されているマウスボタンをチェック */
	this->Window()->CurrentMessage()->FindInt32("buttons", &theInt);
	buttons = theInt;
	if (buttons != B_PRIMARY_MOUSE_BUTTON)
		return;  /* 第一ボタン以外の場合は何もしない */
	
	/* クリック応答領域かどうかをチェック */
	theHotSpot = this->FindHotSpot(where);
	if (theHotSpot < 0)
		return;  /* 他の場所 */
	this->HotSpotAction(currInside, prevInside, theHotSpot);
	
	/* マウスアップまで追跡 */
	while (buttons & B_PRIMARY_MOUSE_BUTTON) {
		::snooze(20 * 1000);
		this->GetMouse(&where, &buttons, true);
		
		/* マウス位置に応じた処理 */
		prevInside = currInside;
		currInside = (this->FindHotSpot(where) == theHotSpot);
		this->HotSpotAction(currInside, prevInside, theHotSpot);
	}

	/* 必要ならアクションを実行 */	
	if (currInside)
		this->HotSpotResult(theHotSpot);
	
	return;
}

void
ButtonView::HotSpotAction (
	bool currInside, bool prevInside, int32 hotSpot)
{
	rgb_color	orgColor;

	/* 変更がなければ何もしない */
	if (currInside == prevInside)
		return;
	
	/* マウス位置に応じて色を変える */
	orgColor = this->HighColor();  /* 前景色を保存 */
	if (currInside)
		this->SetHighColor(0, 255, 0);
	else
		this->SetHighColor(255, 0, 0);
	switch (hotSpot) {
	case kRectArea:
		this->FillRect(kHotSpotRect);
		break;
	case kCircleArea:
		kHotSpotCircle.DrawOn(this);
		break;
	}
	this->SetHighColor(orgColor);  /* 描画前の前景色を復元 */
	
	return;
}

int32
ButtonView::FindHotSpot (BPoint where)
{
	int32	theHotSpot = -1;
	
	if (kHotSpotRect.Contains(where))
		theHotSpot = kRectArea;
	else if (kHotSpotCircle.Contains(where))
		theHotSpot = kCircleArea;
	
	return theHotSpot;
}

void
ButtonView::HotSpotResult (int32 hotSpot)
{
	BMessage	theMessage;

	/* クリックされた領域に応じてメッセージを設定 */	
	switch (hotSpot) {
	case kRectArea:
		theMessage.what = B_QUIT_REQUESTED;
		break;
	case kCircleArea:
		theMessage.what = B_MINIMIZE;
		theMessage.AddInt64("when", ::real_time_clock());
		theMessage.AddBool("minimize", true);
		break;
	}

	/* メッセージを送付 */
	this->Window()->PostMessage(&theMessage, this->Window());

	return;
}


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
	this->SetViewColor(0xD0, 0xD0, 0xD0);
}

ButtonView::~ButtonView (void)
{  /* do nothing */  }


/*
 * End of File
 */
