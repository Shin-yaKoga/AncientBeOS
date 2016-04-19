/************************************************************************
 * $Id$
 * Copyright (c) 1997-1998 Fort Gunnison, Inc.
 *
 * WaveView.cp: WaveViewクラスの実装(改訂版)
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

#include <interface/Bitmap.h>
#include <interface/Screen.h>
#include <support/Debug.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/* メッセージのコード */
enum {
	UPDATE_COLOR	= 'upcl'
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
	BView*			theOffView;
	
	/* 表示色更新スケジュールを登録 */
	theSchedule.client     = this;
	theSchedule.message    = new BMessage(UPDATE_COLOR);
	theSchedule.period     = 4;
	theSchedule.first_time = ::real_time_clock() + 4;
	sts = this->AddSchedule(theSchedule);
	if (sts != B_OK)
		goto err;
	
	/* オフスクリーン描画用のBBitmapとBViewを生成 */
	fOffscreen = new BBitmap(myBounds, B_COLOR_8_BIT, true);
	theOffView = new BView(myBounds, "", B_FOLLOW_ALL_SIDES, 0);
	fOffscreen->AddChild(theOffView);
	
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
	
	/* 表示色更新スケジュールを削除 */
	sts = this->RmvSchedule(UPDATE_COLOR);
	if (sts != B_OK)
		goto err;
	
	/* 不要なBBitmapを解放 */
	delete fOffscreen;
	fOffscreen = NULL;
	
	return;
err:
	::Error("WaveView::DetachedFromWindow", sts);
	return;
}

void
WaveView::Draw (BRect updateRect)
{
	/* オフスクリーンからVRAMに転送 */
	this->DrawBitmap(fOffscreen, updateRect, updateRect);
	
	return;
}

void
WaveView::DrawWave (
	BView* inTargetView, rgb_color waveColor, rgb_color backColor)
{
	rgb_color	orgRGBColor = inTargetView->HighColor();

	/* 波紋を現在位置に描画 */
	inTargetView->SetHighColor(waveColor);  /* 波紋の色 */
	if (fWaveRadius == kInitialRadius) {  /* 初期状態 */
		inTargetView->FillEllipse(
			fWaveOrigin, fWaveRadius * 2, fWaveRadius * 2);
	} else {  /* 拡がった状態 */
		inTargetView->StrokeEllipse(fWaveOrigin,
			kInitialRadius + fWaveRadius, kInitialRadius + fWaveRadius);
		inTargetView->SetHighColor(backColor);
		inTargetView->FillEllipse(fWaveOrigin,
			fWaveRadius - kInitialRadius, fWaveRadius - kInitialRadius);
//		if (fWaveRadius - kInitialRadius == 1)
//			inTargetView->FillEllipse(fWaveOrigin,
//				fWaveRadius - kInitialRadius,
//				fWaveRadius - kInitialRadius
//			);
//		else
//			inTargetView->StrokeEllipse(fWaveOrigin,
//				fWaveRadius - kInitialRadius,
//				fWaveRadius - kInitialRadius
//			);
	}
	inTargetView->SetHighColor(orgRGBColor);  /* 元のペンカラーを復元 */

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
	BView*		offView;
	uint8		waveColor;
	BScreen		myScreen(this->Window());
	rgb_color	waveRGBColor, backRGBColor;

	/* 波紋の色を決定 */	
	waveColor = myScreen.InvertIndex(fCurrColor);
	if (fCurrColor == 247)
		waveColor += 1;
	else if (fCurrColor == 16)
		waveColor -= 2;
	waveRGBColor = myScreen.ColorForIndex(waveColor);
	backRGBColor = myScreen.ColorForIndex(fCurrColor);
	
	/* 波紋を描画 */
	this->DrawWave(this, waveRGBColor, backRGBColor);

	/* オフスクリーンにも描画 */
	fOffscreen->Lock();
	offView = fOffscreen->ChildAt(0);
	this->DrawWave(offView, waveRGBColor, backRGBColor);
	offView->Window()->Flush();  /* !! */
	fOffscreen->Unlock();

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
	: TimedView(frame, "Wave", resizeMask,
		B_PULSE_NEEDED|B_WILL_DRAW, inTimeKeeper)
{
	fCurrColor  = 255;
	fWaveRadius = kInitialRadius;
	fOffscreen  = NULL;
}

WaveView::~WaveView (void)
{
	delete fOffscreen;
}



/*
 * End of File
 */
