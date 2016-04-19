/***********************************************************************
 * $Id$
 * Copyright (c) 1998 by Fort Gunnison, Inc.
 * 
 * DocumentsWin.h: 特定カテゴリに所属するドキュメントの表示用
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 24, 1998
 * Last update:	May. 04, 1998
 ************************************************************************
*/

#ifndef _DOCUMENTS_WIN_H_
#define _DOCUMENTS_WIN_H_

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
 * DocumentsWinクラスの定義
 */
class DocumentsWin : public RegularWindow {
// メソッド
public:
	// 初期化と解放
	DocumentsWin(BRect frame, const char* title,
		window_type type, uint32 flags, uint32 quitMsg);
	~DocumentsWin(void);
	void	InitContent(BView* inContent);
	void	InitContent(BMenuBar* inMenuBar, BView* inContent);

private:
	// メニュー調節
	void	MenusBeginning(void);
	
	// メッセージ処理
	void	MessageReceived(BMessage* message);

// データメンバ
private:
	RetrieverView*	fDocumentsView;	/* ドキュメント一覧ビュー */
};


#endif  /* _DOCUMENTS_WIN_H_ */

/*
 * End of File
 */
