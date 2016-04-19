/***********************************************************************
 * $Id$
 * Copyright (c) 1998 by Fort Gunnison, Inc.
 * 
 * SearchWindow.h: 検索パネル用のウィンドウ
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 23, 1998
 * Last update:	May. 04, 1998
 ************************************************************************
*/

#ifndef _SEARCH_WINDOW_H_
#define _SEARCH_WINDOW_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "MonoWindow.h"


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/* 関連クラス・構造体 */
class	RetrieverView;


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * SearchWindowのクラス定義
 */
class SearchWindow : public MonoWindow {
// メソッド
public:
	// 初期化と解放
	SearchWindow(BRect frame, const char* title,
		window_type type, uint32 flags);
	~SearchWindow(void);
	void	InitContent(BView* inContent);
	void	InitContent(BMenuBar* inMenuBar, BView* inContent);

private:
	// メニュー調節
	void	MenusBeginning(void);
	
	// メッセージ処理
	void	MessageReceived(BMessage* message);

// データメンバ
private:
	RetrieverView*	fSearchView;	/* 検索用のビュー */
};


#endif  /* _SEARCH_WINDOW_H_ */

/*
 * End of File
 */
