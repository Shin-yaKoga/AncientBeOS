/***********************************************************************
 * $Id$
 * Copyright (c) 1998 by Fort Gunnison, Inc.
 * 
 * SearchView.h: 検索パネルのメインビュー
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 30, 1998
 * Last update:	Jan. 30, 1998
 ************************************************************************
*/

#ifndef _SEARCH_VIEW_H_
#define _SEARCH_VIEW_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "RetrieverView.h"
#include <storage/Query.h>


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/* 関連クラス・構造体 */
class	BMenu;


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * SearchViewクラスの定義
 */
class SearchView : public RetrieverView {
// メソッド
public:
	// 初期化と解放
	SearchView(BRect frame, uint32 resizeMask);
	~SearchView(void);
	
	// メッセージ処理
	void	MessageReceived(BMessage* message);
private:
	void	MouseDown(BPoint where);
	void	FrameResized(float newWidth, float newHeight);
	void	UpdateOperator(BMessage* message);
	void	ResetOperator(BMessage* message);
	void	VolumeMounted(BMessage* message);
	void	VolumeUnmounted(BMessage* message);
	
	// 検索処理
	void	StartSearch(void);
	void	MakeupQuery(BQuery* outQuery);
	
	// サブビュー管理
	void	AllAttached(void);
	void	DetachedFromWindow(void);
	void	MakeContent(void);
	void	MakeCriteriaBox(BView* inBox);
	BMenu*	MakeAttribMenu(void);
	void	AdjustContent(void);
	void	AdjustCriteriaBox(BView* inBox);
	void	AdjustVolumeMenu(void);

// データメンバ
private:
	query_op	fCurrOp;	/* 問い合わせの演算子 */
};


#endif  /* _SEARCH_VIEW_H_ */

/*
 * End of File
 */
