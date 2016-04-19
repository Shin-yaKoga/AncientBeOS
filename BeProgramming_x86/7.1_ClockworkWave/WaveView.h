/***********************************************************************
 * $Id$
 * Copyright (c) 1997-1998 by Fort Gunnison, Inc.
 * 
 * WaveView.h: 波紋のアニメーションを表示するビュー
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 22, 1997
 * Last update:	Feb. 13, 1998
 ************************************************************************
*/

#ifndef _WAVE_VIEW_H_
#define _WAVE_VIEW_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include <interface/View.h>


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/* 関連クラス・構造体 */
class	TimeKeeper;


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * WaveViewクラスの定義
 */
class WaveView : public BView {
// メソッド
public:
	// 初期化と解放
	WaveView(BRect frame, uint32 resizeMask, TimeKeeper* inTimeKeeper);
	~WaveView(void);

private:
	// 描画処理
	void	AttachedToWindow(void);
	void	DetachedFromWindow(void);

	// メッセージ処理
	void	MessageReceived(BMessage* message);
	void	Pulse(void);
	void	UpdateColor(void);
	
// データメンバ
private:
	TimeKeeper*	fTimeKeeper;	/* スケジューラ */
	uint8		fCurrColor;		/* 水面の色 */
	float		fWaveRadius;	/* 波紋の半径 */
	BPoint		fWaveOrigin;	/* 波紋の中心 */
};


#endif  /* _WAVE_VIEW_H_ */

/*
 * End of File
 */
