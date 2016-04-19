/***********************************************************************
 * $Id$
 * Copyright (c) 1998 by Fort Gunnison, Inc.
 * 
 * EntryListItem.h: ファイルシステム・エントリの表示用
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 30, 1998
 * Last update:	Apr. 29, 1998
 ************************************************************************
*/

#ifndef _ENTRY_LIST_ITEM_H_
#define _ENTRY_LIST_ITEM_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include <interface/ListItem.h>


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * EntryListItemクラスの定義
 */
class EntryListItem : public BListItem {
// メソッド
public:
	// 初期化と解放
	EntryListItem(const entry_ref& inEntryRef,
		uint32 outlineLevel = 0, bool expanded = true);
	~EntryListItem(void);
	
	// 表示ファイルの設定と取得
	void	SetEntryRef(const entry_ref& inEntryRef);
	const entry_ref*	EntryRef(void) const;
	const node_ref*		NodeRef(void) const;
	
	// 描画処理
	static float	PreferredWidth(void);
private:
	void	DrawItem(BView* owner, BRect frame, bool complete = true);
	void	Update(BView* owner, const BFont* font);

// データメンバ
private:
	entry_ref	fEntryRef;	/* エントリ参照子 */
	node_ref	fNodeRef;	/* ノード参照子 */
	time_t		fModTime;	/* 最終更新日時 */
	char*		fNameStr;	/* 表示用のエントリ名 */
	BBitmap*	fIcon;		/* ノードのアイコン */
};


#endif  /* _ENTRY_LIST_ITEM_H_ */

/*
 * End of File
 */
