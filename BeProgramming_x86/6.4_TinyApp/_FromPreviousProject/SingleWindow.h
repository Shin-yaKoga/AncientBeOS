/***********************************************************************
 * $Id$
 * Copyright (c) 1997 by Fort Gunnison, Inc.
 * 
 * SingleWindow.h: 一枚しかウィンドウを持たないアプリケーション用
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 12, 1997
 * Last update: Dec. 12, 1997
 ************************************************************************
*/

#ifndef _SINGLE_WINDOW_H_
#define _SINGLE_WINDOW_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include <interface/Window.h>


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * SingleWindowクラスの定義
 */
class SingleWindow : public BWindow {
// メソッド
public:
	// 初期化と解放
	SingleWindow(BRect frame, const char* title,
		window_type type, uint32 flags,
		uint32 workspace = B_CURRENT_WORKSPACE);
	~SingleWindow(void);
	void	InitContent(BView* inContent);
	
	// 終了時処理
	void	Quit(void);
};


#endif  /* _SINGLE_WINDOW_H_ */

/*
 * End of File
 */
