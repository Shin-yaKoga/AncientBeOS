/***********************************************************************
 * $Id$
 * Copyright (c) 1997-1998 by Fort Gunnison, Inc.
 * 
 * MonoWindow.h: 一枚しかウィンドウを持たないアプリケーション用
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 24, 1997
 * Last update:	Jan. 23, 1998
 ************************************************************************
*/

#ifndef _MONO_WINDOW_H_
#define _MONO_WINDOW_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "RegularWindow.h"


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * MonoWindowのクラス定義
 */
class MonoWindow : public RegularWindow {
// メソッド
public:
	// 初期化と解放
	MonoWindow(BRect frame, const char* title,
		window_type type, uint32 flags);
	virtual ~MonoWindow(void);
	
	// 終了時処理
	bool	QuitRequested(void);
};


#endif  /* _MONO_WINDOW_H_ */

/*
 * End of File
 */
