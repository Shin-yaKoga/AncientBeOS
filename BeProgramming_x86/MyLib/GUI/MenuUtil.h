/***********************************************************************
 * $Id$
 * Copyright (c) 1998 by Fort Gunnison, Inc.
 * 
 * MenuUtil.h: メニューの生成・操作用のユーティリティ
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 28, 1998
 * Last update:	May. 04, 1998
 ************************************************************************
*/

#ifndef _MENU_UTIL_H_
#define _MENU_UTIL_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include <interface/MenuBar.h>
#include <interface/MenuItem.h>


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/* メニュー項目の記述情報 */
struct MItemInfo {
	const char*	label;		/* 項目のラベル */
	uint32		command;	/* メッセージコード */
	char		shortcut;	/* xx */
};

/* メニューの記述情報 */
struct MInfo {
	const char*		label;	/* メニューのラベル */
	MItemInfo*		items;	/* 項目の配列 */
};


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * MenuUtilクラスの定義
 */
class MenuUtil {
// メソッド
public:
	// メニュー実体の生成
	static BMenuBar*	MakeMenuBar(const MInfo inMInfos[]);
	static BMenu*		MakeMenu(
		const char* inLabel, const MItemInfo inMItemInfos[]);
	
	// メニュー項目の操作
	static void	SetEnabledOf(
					BMenu* inMenu, uint32 theCommand, bool theFlag);
	static void	SetTargetOf(BMenu* inMenu, uint32 theCommand,
					BHandler* inTarget, BLooper* inLooper = NULL);
	static void	SetTargetOf(BMenu* inMenu, const char* theLabel,
					BHandler* inTarget, BLooper* inLooper = NULL);
	static void	SetTargetOf(BMenu* inMenu, uint32 theCommand,
					const BMessenger& inTarget);
};


#endif  /* _MENU_UTIL_H_ */

/*
 * End of File
 */
