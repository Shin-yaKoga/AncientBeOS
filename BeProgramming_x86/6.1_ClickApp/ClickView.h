/***********************************************************************
 * $Id$
 * Copyright (c) 1997 by Fort Gunnison, Inc.
 * 
 * ClickView.h: マウス応答のデモ用ビュー
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 11, 1997
 * Last update: Dec. 11, 1997
 ************************************************************************
*/

#ifndef _CLICK_VIEW_H_
#define _CLICK_VIEW_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include <interface/View.h>


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * ClickViewクラスの定義
 */
class ClickView : public BView {
// メソッド
public:
	// 初期化と解放
	ClickView(
		BRect frame, const char* name, uint32 resizeMask, uint32 flags);
	~ClickView(void);

private:
	// 描画処理
	void	Draw(BRect updateRect);
	
	// マウス応答
	void	MouseDown(BPoint where);
	void	HotSpotAction(bool currInside, bool prevInside);
};


#endif  /* _CLICK_VIEW_H_ */

/*
 * End of File
 */
