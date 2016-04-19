/************************************************************************
 * $Id$
 * Copyright (c) 1997-1998 Fort Gunnison, Inc.
 *
 * ClickView.cp: ClickViewクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 11, 1997
 * Last update:	Feb. 09, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "ClickView.h"
#include <app/Application.h>
#include <interface/Window.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/* 定数定義 */
const BRect	kHotSpot(110, 30, 140, 60);


/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/* ClickViewクラスの非公開メソッド */
/*
 * 描画処理; ClickView
 */
void
ClickView::Draw (BRect /* updateRect */)
{
	/* 文字列と図形を描画 */
	this->DrawString("This is the my 2nd app.", BPoint(10, 10));
	this->SetHighColor(255, 0, 0);
	this->FillRect(kHotSpot);

	return;
}

/*
 * マウス応答; ClickView
 */
void
ClickView::MouseDown (BPoint where)
{
	uint32	buttons = 0;
	int32	theInt;
	bool	currInside = true;
	bool	prevInside = false;
	
	/* 現在押されているマウスボタンをチェック */
	this->Window()->CurrentMessage()->FindInt32("buttons", &theInt);
	buttons = theInt;
	if (buttons != B_PRIMARY_MOUSE_BUTTON)
		return;  /* 第一ボタン以外の場合は何もしない */
	
	/* クリック応答領域かどうかをチェック */
	if (! kHotSpot.Contains(where))
		return;  /* 他の場所 */
	this->HotSpotAction(currInside, prevInside);
	
	/* マウスアップまで追跡 */
	while (buttons & B_PRIMARY_MOUSE_BUTTON) {
		::snooze(20 * 1000);
		this->GetMouse(&where, &buttons, true);
		
		/* マウス位置に応じた処理 */
		prevInside = currInside;
		currInside = kHotSpot.Contains(where);
		this->HotSpotAction(currInside, prevInside);
	}
	
	/* 必要ならアクションを実行 */
//	if (kHotSpot.Contains(where))
	if (currInside)
		be_app->PostMessage(B_QUIT_REQUESTED, be_app);
	
	return;
}

void
ClickView::HotSpotAction (bool currInside, bool prevInside)
{
	rgb_color	orgColor;

	/* 変更がなければ何もしない */
	if (currInside == prevInside)
		return;

	/* マウス位置に応じて色を変える */
	orgColor = this->HighColor();  /* ビューの描画色を保存 */
	if (currInside)
		this->SetHighColor(0, 255, 0);
	else
		this->SetHighColor(255, 0, 0);
	this->FillRect(kHotSpot);
	this->SetHighColor(orgColor);  /* 描画色を元に戻す */
	
	return;
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * ClickViewクラスの公開メソッド
 */
ClickView::ClickView (BRect frame, const char* name,
		uint32 resizeMask, uint32 flags)
	: BView(frame, name, resizeMask, flags)
{
	this->SetViewColor(0xD0, 0xD0, 0xD0);
}

ClickView::~ClickView (void)
{  /* do nothing */  }


/*
 * End of File
 */
