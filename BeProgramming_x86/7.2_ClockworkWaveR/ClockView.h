/***********************************************************************
 * $Id$
 * Copyright (c) 1997 by Fort Gunnison, Inc.
 * 
 * ClockView.h: 現在時刻をデジタル表示するビュー(改訂版)
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 21, 1997
 * Last update:	Dec. 22, 1997
 ************************************************************************
*/

#ifndef _CLOCK_VIEW_H_
#define _CLOCK_VIEW_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "TimedView.h"
#include <time.h>


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * ClockViewクラスの定義
 */
class ClockView : public TimedView {
// メソッド
public:
	// 初期化と解放
	ClockView(BRect frame, uint32 resizeMask, TimeKeeper* inTimeKeeper);
	~ClockView(void);

private:
	// 描画処理
	void	AttachedToWindow(void);
	void	DetachedFromWindow(void);
	void	Draw(BRect updateRect);
	
	// メッセージ処理
	void	MessageReceived(BMessage* message);
	void	UpdateTime(void);

// データメンバ
private:
	tm			fTime;			/* 現在時刻 */
	BBitmap*	fOffscreen;		/* オフスクリーン描画用 */
};


#endif  /* _CLOCK_VIEW_H_ */

/*
 * End of File
 */
