/***********************************************************************
 * $Id$
 * Copyright (c) 1998 by Fort Gunnison, Inc.
 * 
 * MyMemoWindow.h: MemoMailer用のテキスト編集ウィンドウ
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		May. 04, 1998
 * Last update:	May. 11, 1998
 ************************************************************************
*/

#ifndef _MY_MEMO_WINDOW_H_
#define _MY_MEMO_WINDOW_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "MemoWindow.h"


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * MemoWindowクラスの定義
 */
class MyMemoWindow : public MemoWindow {
// メソッド
public:
	// 初期化と解放
	MyMemoWindow(BRect frame, const char* inTitle, uint32 quitMsg);
	~MyMemoWindow(void);

	// 表示テキストの取得
	const char*	Text(void);

private:
	// メッセージ応答
	void	MessageReceived(BMessage* message);
	void	SendDocument(void);
};


#endif  /* _MY_MEMO_WINDOW_H_ */

/*
 * End of File
 */
