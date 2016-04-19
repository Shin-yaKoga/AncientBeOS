/************************************************************************
 * $Id$
 * Copyright (c) 1997-1998 Fort Gunnison, Inc.
 *
 * ClockView.cp: ClockViewクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 21, 1997
 * Last update:	Apr. 21, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "ClockView.h"
#include "TimeKeeper.h"
#include "KGUtility/kgDebug.h"

#include <support/Debug.h>
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

/* ClockViewクラスの非公開メソッド  */
/*
 * 描画処理; ClockView
 */
void
ClockView::AttachedToWindow (void)
{
	status_t		sts;
	ScheduleInfo	theSchedule;

	/* スケジュールを登録 */
	theSchedule.client     = this;
	theSchedule.message    = new BMessage(UPDATE_TIME);
	theSchedule.period     = 1;
	theSchedule.first_time = ::real_time_clock();
	sts = fTimeKeeper->InsertSchedule(theSchedule);
	if (sts != B_OK)
		goto err;
	
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
	
	sts = fTimeKeeper->RemoveSchedule(this, UPDATE_TIME);
	if (sts != B_OK)
		goto err;
	
	return;
err:
	::Error("ClockView::DetachedFromWindow", sts);
	return;
}

void
ClockView::Draw (BRect /* updateRect */)
{
	char		timeStr[64];
	rgb_color	orgColor;
	
	orgColor = this->HighColor();
	this->SetHighColor(this->ViewColor());
	this->FillRect(this->Bounds());
	this->SetHighColor(orgColor);

	this->SetFontSize(30);
	sprintf(timeStr, "%02d:%02d:%02d",
		fTime.tm_hour, fTime.tm_min, fTime.tm_sec);
	this->DrawString(timeStr, BPoint(18, 40));
	
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
		this->UpdateTime();		break;
	default:
		BView::MessageReceived(message);
	}
	
	return;
}

void
ClockView::UpdateTime (void)
{
	time_t	curTime = time(0);

	/* 時刻情報を更新 */
	fTime = *(localtime(&curTime));
	
	/* 再描画要求を発行 */
	this->Draw(this->Bounds());
	
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
	: BView(frame, "ClockView", resizeMask, B_WILL_DRAW)
{
	ASSERT(inTimeKeeper != NULL);

	fTimeKeeper = inTimeKeeper;
	fTime.tm_hour = fTime.tm_min = fTime.tm_sec = 0;
}

ClockView::~ClockView (void)
{  /* do nothing */  }


/*
 * End of File
 */
