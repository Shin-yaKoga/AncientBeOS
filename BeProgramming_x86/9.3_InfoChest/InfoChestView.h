/***********************************************************************
 * $Id$
 * Copyright (c) 1998 by Fort Gunnison, Inc.
 * 
 * InfoChestView.h: カテゴリの一覧表示と呼び出し用
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 24, 1998
 * Last update:	May. 01, 1998
 ************************************************************************
*/

#ifndef _INFO_CHEST_VIEW_H_
#define _INFO_CHEST_VIEW_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include <interface/View.h>


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/* 公開メッセージのコード */
enum {
	SHOW_DOCUMENTS	= 'sdoc',	/* ドキュメントを一覧表示 */
	HIDE_DOCUMENTS	= 'hdoc'	/* ドキュメントの表示を隠す */
};

/* 文字列定数 */
extern const char	kArgCategory[];


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * InfoChestViewクラスの定義
 */
class InfoChestView : public BView {
// メソッド
public:
	// 初期化と解放
	InfoChestView(BRect frame, uint32 resizeMask);
	~InfoChestView(void);
	
	// カテゴリの表示管理
	void		UpdateList(void);
	const char*	GetSelectedCategory(void);
private:
	void		CleanupList(void);

	// メッセージ処理
	void		MessageReceived(BMessage* message);
	void		SelectionChanged(void);
	void		ShowDocuments(void);

	// 描画処理
	void		AttachedToWindow(void);
	void		DetachedFromWindow(void);
	void		MakeContent(void);
	void		AdjustContent(void);
};


#endif  /* _INFO_CHEST_VIEW_H_ */

/*
 * End of File
 */
