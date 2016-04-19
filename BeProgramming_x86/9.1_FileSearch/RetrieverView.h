/***********************************************************************
 * $Id$
 * Copyright (c) 1998 by Fort Gunnison, Inc.
 * 
 * RetrieverView.h: 検索動作の制御に使うビューの基底クラス
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 29, 1998
 * Last update:	May. 01, 1998
 ************************************************************************
*/

#ifndef _RETRIEVER_VIEW_H_
#define _RETRIEVER_VIEW_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include <interface/View.h>
#include <support/List.h>


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/* 関連クラス・構造体 */
class	EntryListView;
class	SearchEngine;
class	BMenuBar;
class	BQuery;


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * RetrieverViewクラスの定義
 */
class RetrieverView : public BView {
// メソッド
public:
	// 検索用のユーティリティ
	static status_t	GetQueryAwareVolumes(BList* outDevices);
	
	// 初期化と解放
	RetrieverView(
		BRect frame, const char* title, uint32 resizeMask, uint32 flags);
	virtual ~RetrieverView(void);

	// メニュー調節
	void	AddAdjustableMenuItem(uint32 theCommand);
	void	AdjustMenuItems(BMenuBar* ioMenuBar);
	
	// xx
	void	SetDragEnabled(bool allowDrag);

	// メッセージ処理
	virtual void	MessageReceived(BMessage* message);

protected:
	// 検索結果の表示動作制御
	void	SetEntryListView(EntryListView* inListView);
	EntryListView*	GetEntryListView(void);
	
	// 検索動作の制御
	void	StartSearch(
		const BList& inDevices, bool doLiveQuery, uint32 theFlavor);
	void	AbortSearch(void);
	virtual void	MakeupQuery(BQuery* outQuery) = 0;
	
	// 検索オブジェクトの管理
	SearchEngine*	FindSearcher(dev_t theDevice);
	
	// メッセージ処理
	virtual void	VolumeMounted(BMessage* message);
	virtual void	VolumeUnmounted(BMessage* message);
private:
	void	HandleNodeMessage(BMessage* message);
	void	SearchCompleted(BMessage* message);
	void	EntriesFound(BMessage* message);
	void	EntriesChanged(BMessage* message);
	
	// 検索処理の実行
	void	StartSearchOn(dev_t theDevice, BQuery* theQuery);

// データメンバ
private:
	EntryListView*	fListView;		/* エントリのリスト表示ビュー */
	BList			fMItemList;		/* 状態調節を行うメニュー項目 */
	BList			fSearchers;		/* 検索オブジェクトのリスト */
	bool			fDoLiveQuery;	/* live queryを行うかどうか */
	uint32			fNodeFlavor;	/* 検索対象範囲 */
};


#endif  /* _RETRIEVER_VIEW_H_ */

/*
 * End of File
 */
