/***********************************************************************
 * $Id$
 * Copyright (c) 1998 by Fort Gunnison, Inc.
 * 
 * SearchWindow.h: 検索パネル用のウィンドウ(改訂版)
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 23, 1998
 * Last update:	May. 01, 1998
 *
 * 注意：この改訂版は、MonoWindowではなくRegularWindowクラスを継承する
 *		ように変更した以外は、"FileSearch"アプリケーションで使っている
 *		オリジナルと変わらない。
 ************************************************************************
*/

#ifndef _SEARCH_WINDOW_H_
#define _SEARCH_WINDOW_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "GUI/RegularWindow.h"


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
class SearchWindow : public RegularWindow {
// メソッド
public:
	// 初期化と解放
	SearchWindow(BRect frame, const char* title,
		window_type type, uint32 flags, uint32 quitMsg);
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
	BMenuBar*		fMenuBar;		/* メニューバー */
};


#endif  /* _SEARCH_WINDOW_H_ */

/*
 * End of File
 */
