/***********************************************************************
 * $Id$
 * Copyright (c) 1997-1998 by Fort Gunnison, Inc.
 * 
 * SmartWindow.h: 閉じる前にユーザに確認を求めるウィンドウ
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 14, 1997
 * Last update: Feb. 12, 1998
 ************************************************************************
*/

#ifndef _SMART_WINDOW_H_
#define _SMART_WINDOW_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "SingleWindow.h"


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * SmartWindowクラスの定義
 */
class SmartWindow : public SingleWindow {
// メソッド
public:
	// 初期化と解放
	SmartWindow(BRect frame, const char* title,
		window_type type, uint32 flags,
		uint32 workspace = B_CURRENT_WORKSPACE);
	~SmartWindow(void);
	void	InitContent(BMenuBar* inMenuBar, BView* inContent);

private:
	// 終了時処理
	bool	QuitRequested(void);
};


#endif  /* _SMART_WINDOW_H_ */

/*
 * End of File
 */
