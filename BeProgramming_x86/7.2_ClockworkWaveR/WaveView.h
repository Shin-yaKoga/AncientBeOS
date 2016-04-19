/***********************************************************************
 * $Id$
 * Copyright (c) 1997 by Fort Gunnison, Inc.
 * 
 * WaveView.h: 波紋のアニメーションを表示するビュー(改訂版)
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 22, 1997
 * Last update:	Dec. 23, 1997
 ************************************************************************
*/

#ifndef _WAVE_VIEW_H_
#define _WAVE_VIEW_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "TimedView.h"


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * WaveViewクラスの定義
 */
class WaveView : public TimedView {
// メソッド
public:
	// 初期化と解放
	WaveView(BRect frame, uint32 resizeMask, TimeKeeper* inTimeKeeper);
	~WaveView(void);

private:
	// 描画処理
	void	AttachedToWindow(void);
	void	DetachedFromWindow(void);
	void	Draw(BRect updateRect);
	void	DrawWave(BView* inTargetView,
				rgb_color waveColor, rgb_color backColor);
	
	// メッセージ処理
	void	MessageReceived(BMessage* message);
	void	Pulse(void);
	void	UpdateColor(void);

// データメンバ
private:
	uint8		fCurrColor;		/* 水面の色 */
	float		fWaveRadius;	/* 波紋の半径 */
	BPoint		fWaveOrigin;	/* 波紋の中心 */
	BBitmap*	fOffscreen;		/* オフスクリーン描画用 */
};


#endif  /* _WAVE_VIEW_H_ */

/*
 * End of File
 */
