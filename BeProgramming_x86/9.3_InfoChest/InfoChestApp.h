/***********************************************************************
 * $Id$
 * Copyright (c) 1998 by Fort Gunnison, Inc.
 * 
 * InfoChestApp.h: InfoChest用のアプリケーションクラス
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 24, 1998
 * Last update:	May. 01, 1998
 ************************************************************************
*/

#ifndef _INFO_CHEST_APP_H_
#define _INFO_CHEST_APP_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include <app/Application.h>


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/* 関連クラス・構造体 */
class	DocumentsWin;
class	BAlert;
class	BVolumeRoster;


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * InfoChestAppクラスの定義
 */
class InfoChestApp : public BApplication {
// メソッド
public:
	// 初期化と解放
	InfoChestApp(void);
	~InfoChestApp(void);

private:
	// 起動と終了
	void	ReadyToRun(void);
	void	Quit(void);
	
	// メッセージ処理
	void	MessageReceived(BMessage* message);
	void	AboutRequested(void);
	void	ShowDocumentsOf(BMessage* message);
	void	ShowSearchPanel(void);
	void	ShowMainWindow(void);
	void	DocWinClosed(BMessage* message);
	void	CategoryRemoved(BMessage* message);
	void	VolumeChanged(BMessage* message);
	
	// ビュー部品の管理
	DocumentsWin*	FindDocumentsWindow(const char* inCategory);

// データメンバ
private:
	BAlert*			fAboutBox;		/* アバウトダイアログ */
	BList*			fDocWindows;	/* カテゴリウィンドウのリスト */
	BWindow*		fSearchWindow;	/* 検索ウィンドウ */
	BWindow*		fMainWindow;	/* カテゴリ管理ウィンドウ */
	BVolumeRoster*	fVolRoster;		/* ボリューム監視オブジェクト */
};


#endif  /* _INFO_CHEST_APP_H_ */

/*
 * End of File
 */
