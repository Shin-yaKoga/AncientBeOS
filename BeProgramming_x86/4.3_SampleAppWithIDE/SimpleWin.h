/***********************************************************************
 * $Id$
 * Copyright (c) 1997 by Fort Gunnison, Inc.
 * 
 * SimpleWin.h: 画面表示のサンプル用ウィンドウ(改訂版)
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 10, 1997
 * Last update:	Dec. 11, 1997
 ************************************************************************
*/

#ifndef _SIMPLE_WIN_H_
#define _SIMPLE_WIN_H_

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
 * SimpleWinクラスの定義
 */
class SimpleWin : public BWindow {
// メソッド
public:
	// 初期化と解放
	SimpleWin(BRect frame, const char* title,
		window_type type, uint32 flags,
		uint32 workspace = B_CURRENT_WORKSPACE);
	~SimpleWin(void);
	
	// 終了時処理
	void	Quit(void);
};


#endif  /* _SIMPLE_WIN_H_ */

/*
 * End of File
 */
