/***********************************************************************
 * $Id$
 * Copyright (c) 1997-1998 by Fort Gunnison, Inc.
 * 
 * RegularWindow.h: 一般的なウィンドウのクラス
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 21, 1997
 * Last update:	Jan. 23, 1998
 ************************************************************************
*/

#ifndef _REGULAR_WINDOW_H_
#define _REGULAR_WINDOW_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include <interface/Window.h>


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/* 文字列定数 */
extern const char	kSender[];


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * RegularWindowのクラス定義
 */
class RegularWindow : public BWindow {
// メソッド
public:
	// 初期化と解放
	RegularWindow(BRect frame, const char* title,
		window_type type, uint32 flags, uint32 quitMsg);
	virtual ~RegularWindow(void);
	virtual void	InitContent(BView* inContent);
	virtual void	InitContent(BMenuBar* inMenuBar, BView* inContent);
	
	// 終了時処理
	void	Quit(void);

// データメンバ
private:
	uint32	fQuitMsg;	/* 終了通知メッセージのコード */
};


#endif  /* _REGULAR_WINDOW_H_ */

/*
 * End of File
 */
