/***********************************************************************
 * $Id$
 * Copyright (c) 1998 by Fort Gunnison, Inc.
 * 
 * EntryListView.h: ファイルシステム・エントリの一覧表示用
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 30, 1998
 * Last update:	May. 06, 1998
 ************************************************************************
*/

#ifndef _ENTRY_LIST_VIEW_H_
#define _ENTRY_LIST_VIEW_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include <interface/ListView.h>
#include <storage/Entry.h>


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/* 公開メッセージのコード */
enum {
	OPEN_DOCUMENT	= 'odoc',	/* ファイルを開く */
	OPEN_PARENT		= 'opar'	/* 親フォルダを開く */
};


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * EntryListViewクラスの定義
 */
class EntryListView : public BListView {
// メソッド
public:
	// 初期化と解放
	EntryListView(BRect frame, const char* name,
		list_view_type type = B_SINGLE_SELECTION_LIST,
		uint32 resizeMask = B_FOLLOW_LEFT|B_FOLLOW_TOP,
		uint32 flags = B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE);
	~EntryListView(void);
	
	// リスト表示要素の管理
	int32		IndexOf(const entry_ref& inEntryRef);
	int32		IndexOf(const node_ref& inNodeRef);
	bool		CreateItem(const entry_ref& inEntryRef);
	bool		DeleteItem(const entry_ref& inEntryRef);
	bool		DeleteItem(const node_ref& inNodeRef);
	void		DeleteItemsOf(dev_t theDevice);
	void		DeleteAll(void);
	bool		ItemMoved(
		const node_ref& inNodeRef, const entry_ref& inNewEntryRef);
	
	// ドラッグ＆ドロップ制御
	void		SetDragEnabled(bool allowDrag);

	// メッセージ処理
	void		MessageReceived(BMessage* message);
private:
	status_t	Invoke(BMessage* message = NULL);
	bool		InitiateDrag(BPoint point, int32 index, bool wasSelected);
	void		FrameResized(float newWidth, float newHeight);
	void		OpenEntryOrParent(void);
	void		OpenSelected(void);
	void		OpenParentForSelected(void);
	
	// スクロールバーの調節
	void		AttachedToWindow(void);
	void		UpdateScrollbars(void);
	
	// 警告通知
	void		AlertForTrash(const char* inName);

// データメンバ
private:
	bool	fCanDrag;	/* ドラッグできるか */
};


#endif  /* _ENTRY_LIST_VIEW_H_ */

/*
 * End of File
 */
