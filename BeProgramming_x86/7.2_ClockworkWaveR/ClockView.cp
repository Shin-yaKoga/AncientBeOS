/************************************************************************
 * $Id$
 * Copyright (c) 1997-1998 Fort Gunnison, Inc.
 *
 * ClockView.cp: ClockViewクラスの実装(改訂版)
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 21, 1997
 * Last update:	Mar. 04, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "ClockView.h"
#include "TimeKeeper.h"
#include "KGUtility/kgDebug.h"

#include <interface/Bitmap.h>
#include <stdio.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/* メッセージのコード */
enum {
	UPDATE_TIME	= 'uptm'
};


/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/* ClockViewクラスの非公開メソッド */
/*
 * 描画処理; ClockView
 */
void
ClockView::AttachedToWindow (void)
{
	status_t		sts;
	ScheduleInfo	theSchedule;
	BView*			theOffView;

	/* 時刻更新スケジュールを登録 */
	theSchedule.client    = this;
	theSchedule.message   = new BMessage(UPDATE_TIME);
	theSchedule.period    = 1;
	theSchedule.first_time = ::real_time_clock();
	sts = this->AddSchedule(theSchedule);
	if (sts != B_OK)
		goto err;

	/* オフスクリーン描画用のBBitmapとBViewを生成 */
	fOffscreen = new BBitmap(this->Bounds(), B_COLOR_8_BIT, true);
	theOffView = new BView(this->Bounds(), "", B_FOLLOW_ALL_SIDES, 0);
	theOffView->SetViewColor(this->ViewColor());
	fOffscreen->AddChild(theOffView);
	
	/* 現在の時刻を取得 */
	this->UpdateTime();
	
	return;
err:
	::Error("ClockView::AttachedToWindow", sts);
	return;
}

void
ClockView::DetachedFromWindow (void)
{
	status_t	sts;

	/* 時刻更新スケジュールを削除 */
	sts = this->RmvSchedule(UPDATE_TIME);
	if (sts != B_OK)
		goto err;
	
	/* 不要なBBitmapオブジェクトを解放 */
	delete fOffscreen;
	fOffscreen = NULL;

	return;
err:
	::Error("ClockView::DetachedFromWindow", sts);
	return;
}

void
ClockView::Draw (BRect updateRect)
{
	/* オフスクリーンからVRAMにコピー */
	this->DrawBitmap(fOffscreen, updateRect, updateRect);
	
	return;
}

/*
 * メッセージ処理; ClockView
 */
void
ClockView::MessageReceived (BMessage* message)
{
	switch (message->what) {
	case UPDATE_TIME:
		this->UpdateTime();	break;
	default:
		BView::MessageReceived(message);
	}
	
	return;
}

void
ClockView::UpdateTime (void)
{
	time_t		curTime = time(0);
	char		timeStr[64];
	BView*		offView;
	rgb_color	orgColor;
	
	/* 現在時刻情報を更新 */
	fTime = *(localtime(&curTime));
	sprintf(timeStr, "%02d:%02d:%02d",
		fTime.tm_hour, fTime.tm_min, fTime.tm_sec);
	
	/* BBitmapをロック */
	(void)fOffscreen->Lock();

	/* オフスクリーンに時刻を描画 */
	offView = fOffscreen->ChildAt(0);
	orgColor = offView->HighColor();
	offView->SetHighColor(offView->ViewColor());
	offView->FillRect(offView->Bounds());
	offView->SetHighColor(orgColor);
	offView->SetFontSize(30);
	offView->DrawString(timeStr, BPoint(18, 40));

	/* 更新すべき領域を再描画 */
	this->Draw(offView->Bounds());
	
	/* ロックを解除 */
	fOffscreen->Unlock();
	
	return;
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * ClockViewクラスの公開メソッド
 */
ClockView::ClockView (
		BRect frame, uint32 resizeMask, TimeKeeper* inTimeKeeper)
	: TimedView(frame, "ClockView", resizeMask, B_WILL_DRAW, inTimeKeeper)
{
	fOffscreen = NULL;
}

ClockView::~ClockView (void)
{
	delete fOffscreen;
}


/*
 * End of File
 */
