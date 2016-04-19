/***********************************************************************
 * $Id$
 * Copyright (c) 1997 by Fort Gunnison, Inc.
 * 
 * ButtonView.h: 二つのボタンを持つビュー(標準部品を使う版)
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 13, 1997
 * Last update: Dec. 13, 1997
 ************************************************************************
*/

#ifndef _BUTTON_VIEW_H_
#define _BUTTON_VIEW_H_

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
 * ButtonViewクラスの定義
 */
class ButtonView : public BView {
// メソッド
public:
	// 初期化と解放
	ButtonView(
		BRect frame, const char* title, uint32 resizeMask, uint32 flags);
	~ButtonView(void);
};


#endif  /* _BUTTON_VIEW_H_ */

/*
 * End of File
 */
