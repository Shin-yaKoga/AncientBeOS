/***********************************************************************
 * $Id$
 * Copyright (c) 1997 by Fort Gunnison, Inc.
 * 
 * SimpleView.h: 画面表示のサンプル用ウィンドウ
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 11, 1997
 * Last update: Dec. 11, 1997
 ************************************************************************
*/

#ifndef _SIMPLE_VIEW_H_
#define _SIMPLE_VIEW_H_

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
 * SimpleViewクラスの定義
 */
class SimpleView : public BView {
// メソッド
public:
	// 初期化と解放
	SimpleView(
		BRect frame, const char* name, uint32 resizeMask, uint32 flags);
	~SimpleView(void);

private:	
	// 描画処理
	void	Draw(BRect updateRect);
};


#endif  /* _SIMPLE_VIEW_H_ */

/*
 * End of File
 */
