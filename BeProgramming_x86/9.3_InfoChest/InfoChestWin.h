/***********************************************************************
 * $Id$
 * Copyright (c) 1998 by Fort Gunnison, Inc.
 * 
 * InfoChestWin.h: InfoChestのメインウィンドウ
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 24, 1998
 * Last update:	May. 04, 1998
 ************************************************************************
*/

#ifndef _INFO_CHEST_WIN_H_
#define _INFO_CHEST_WIN_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "GUI/RegularWindow.h"


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/* 関連クラス・構造体 */
class	InfoChestView;
class	SimpleDialog;


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * InfoChestWinクラスの定義
 */
class InfoChestWin : public RegularWindow {
// メソッド
public:
	// 初期化と解放
	InfoChestWin(BRect frame, const char* title,
		window_type type, uint32 flags);
	~InfoChestWin(void);
	void	InitContent(BView* inContent);
	void	InitContent(BMenuBar* inMenuBar, BView* inContent);

private:
	// メッセージ処理
	void	MessageReceived(BMessage* message);
	void	CreateCategory(void);
	void	RemoveCategory(void);
	void	AddCategory(BMessage* message);
	
	// メニュー調節
	void	MenusBeginning(void);
	
	// ダイアログ操作
	void	MakeDialogView(SimpleDialog* inDialog);

// データメンバ
private:
	InfoChestView*	fInfoChestView;	/* カテゴリ一覧ビュー */
};


#endif  /* _INFO_CHEST_WIN_H_ */

/*
 * End of File
 */
