/************************************************************************
 * $Id$
 * Copyright (c) 1997-1998 Fort Gunnison, Inc.
 *
 * WaveView.cp: WaveViewクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 22, 1997
 * Last update:	Mar. 04, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "WaveView.h"
#include "TimeKeeper.h"
#include "KGUtility/kgDebug.h"

#include <interface/Screen.h>
#include <interface/Window.h>
#include <support/Debug.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/* メッセージのコード */
enum {
	UPDATE_COLOR = 'upcl'
};

/* その他の定数 */
const float	kInitialRadius	= 10;	/* 波紋半径の初期値 */


/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/* WaveViewクラスの非公開メソッド */
/*
 * 描画処理; WaveView
 */
void
WaveView::AttachedToWindow (void)
{
	status_t		sts;
	ScheduleInfo	theSchedule;
	BRect			myBounds = this->Bounds();
	
	/* スケジュールを登録 */
	theSchedule.client     = this;
	theSchedule.message    = new BMessage(UPDATE_COLOR);
	theSchedule.period     = 4;
	theSchedule.first_time = ::real_time_clock() + 4;
	sts = fTimeKeeper->InsertSchedule(theSchedule);
	if (sts != B_OK)
		goto err;
	
	/* 水面の色の初期値を設定 */
	this->UpdateColor();
	this->Window()->SetPulseRate(100 * 1000);
	
	/* 波紋の原点座標を決定 */
	fWaveOrigin.x = myBounds.Width() / 2;
	fWaveOrigin.y = myBounds.Height() / 2;
	
	return;
err:
	::Error("WaveView::AttachedToWindow", sts);
	return;
}

void
WaveView::DetachedFromWindow (void)
{
	status_t	sts;
	
	sts = fTimeKeeper->RemoveSchedule(this, UPDATE_COLOR);
	if (sts != B_OK)
		goto err;
	
	return;
err:
	::Error("WaveView::DetachedFromWindow", sts);
	return;
}

/*
 * メッセージ処理; WaveView
 */
void
WaveView::MessageReceived (BMessage* message)
{
	switch (message->what) {
	case UPDATE_COLOR:
		this->UpdateColor();	break;
	default:
		BView::MessageReceived(message);
	}
	
	return;
}

void
WaveView::Pulse (void)
{
	BScreen		myScreen(this->Window());
	rgb_color	orgRGBColor = this->HighColor();
	uint8		waveColor;

	/* 波紋の色を決定 */	
	waveColor = myScreen.InvertIndex(fCurrColor);
	if (fCurrColor == 247)
		waveColor += 1;
	else if (fCurrColor == 16)
		waveColor -= 2;

	/* 波紋を現在位置に描画 */
	this->SetHighColor(myScreen.ColorForIndex(waveColor));  /* 波紋の色 */
	if (fWaveRadius == kInitialRadius) {  /* 初期状態 */
		this->FillEllipse(fWaveOrigin, fWaveRadius * 2, fWaveRadius * 2);
	} else {  /* 拡がった状態 */
		this->StrokeEllipse(fWaveOrigin,
			kInitialRadius + fWaveRadius, kInitialRadius + fWaveRadius);
		this->SetHighColor(myScreen.ColorForIndex(fCurrColor));
		this->FillEllipse(fWaveOrigin,
			fWaveRadius - kInitialRadius, fWaveRadius - kInitialRadius);
//		if (fWaveRadius - kInitialRadius == 1)
//			this->FillEllipse(fWaveOrigin,
//				fWaveRadius - kInitialRadius,
//				fWaveRadius - kInitialRadius
//			);
//		else
//			this->StrokeEllipse(fWaveOrigin,
//				fWaveRadius - kInitialRadius,
//				fWaveRadius - kInitialRadius
//			);
	}
	this->SetHighColor(orgRGBColor);  /* 元のペンカラーを復元 */

	/* 波紋の半径値を更新 */
	if ((fWaveOrigin.x * sqrt(2.0)) <= (fWaveRadius - kInitialRadius))
		fWaveRadius = kInitialRadius;
	else
		fWaveRadius += 1;
	
	return;
}

void
WaveView::UpdateColor (void)
{
	ASSERT(this->Window() != NULL);

	BScreen		myScreen(this->Window());
	rgb_color	newColor;

	/* 波紋の色を算出 */
	newColor = myScreen.ColorForIndex(fCurrColor--);
	
	/* 使う色がパレットを一周したら最初に戻す */
	if ((int16)fCurrColor < 0)
		fCurrColor = 255;
	
	return;
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * WaveViewクラスの公開メソッド
 */
WaveView::WaveView (
		BRect frame, uint32 resizeMask, TimeKeeper* inTimeKeeper)
	: BView(frame, "Wave", resizeMask, B_PULSE_NEEDED)
{
	ASSERT(inTimeKeeper != NULL);
	
	fTimeKeeper = inTimeKeeper;
	fCurrColor  = 255;
	fWaveRadius = kInitialRadius;
}

WaveView::~WaveView (void)
{  /* do nothing */  }


/*
 * End of File
 */
