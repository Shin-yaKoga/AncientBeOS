/***********************************************************************
 * $Id$
 * Copyright (c) 1998 by Fort Gunnison, Inc.
 * 
 * DocumentsView.h: カテゴリによるドキュメント管理用のビュー
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Feb. 01, 1998
 * Last update:	Feb. 01, 1998
 ************************************************************************
*/

#ifndef _DOCUMENTS_VIEW_H_
#define _DOCUMENTS_VIEW_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "RetrieverView.h"


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/* 関連クラス・構造体 */
class	BFilePanel;

/* 公開メッセージのコード */
enum {
	/* Fileメニュー */
	ADD_DOCUMENT		= 'adoc',	/* ドキュメントを追加する */
	RMV_DOCUMENT		= 'rdoc',	/* ドキュメントを削除する */
	/* Sortメニュー */
	SORT_BY_NAME		= 'snam',	/* 名前順で並べ替え */
	SORT_BY_MODIFIED	= 'smod',	/* 修正日時順で並べ替え */
	SORT_BY_SIZE		= 'ssiz'	/* サイズ順で並べ替え */
};


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * DocumentsViewクラスの定義
 */
class DocumentsView : public RetrieverView {
// メソッド
public:
	// 初期化と解放
	DocumentsView(BRect frame, uint32 resizeMask, const char* inCategory);
	~DocumentsView(void);
	
	// メッセージ処理
	void	MessageReceived(BMessage* message);
private:
	void	MessageDropped(BMessage* message);
	void	RefsReceived(BMessage* message);
	void	AddDocument(void);
	void	RemoveSelected(void);
	void	ChangeSortKey(BMessage* message);
	
	// ドキュメント管理
	void	StartSearch(void);
	void	MakeupQuery(BQuery* outQuery);
	
	// サブビュー管理
	void	AllAttached(void);
	void	DetachedFromWindow(void);
	void	MakeContent(void);
	void	AdjustContent(void);
	
	// ドキュメントのソート用比較関数 [static]
	static int	CompareByName(const void* inItemA, const void* inItemB);
	static int	CompareByModifiedTime(
					const void* inItemA, const void* inItemB);
	static int	CompareBySize(const void* inItmeA, const void* inItemB);

// データメンバ
private:
	char*	fCategory;	/* カテゴリ */
	
	// クラスデータ
	static BFilePanel*	sOpenPanel;	/* オープンパネル */
};


#endif  /* _DOCUMENTS_VIEW_H_ */

/*
 * End of File
 */
