/***********************************************************************
 * $Id$
 * Copyright (c) 1998 by Fort Gunnison, Inc.
 * 
 * FileSearchApp.h: FileSearchのアプリケーションクラス
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 15, 1998
 * Last update:	Jan. 30, 1998
 ************************************************************************
*/

#ifndef _FILE_SEARCH_H_
#define _FILE_SEARCH_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include <app/Application.h>


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/* 関連クラス・構造体 */
class	BAlert;
class	BVolumeRoster;


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * FileSearchAppクラスの定義
 */
class FileSearchApp : public BApplication {
// メソッド
public:
	// 初期化と解放
	FileSearchApp(void);
	~FileSearchApp(void);

private:
	// 起動時処理
	void	ReadyToRun(void);
	
	// メッセージ処理
	void	MessageReceived(BMessage* message);
	void	AboutRequested(void);

// データメンバ
private:
	BAlert*			fAboutBox;		/* アバウトボックス */
	BWindow*		fMainWindow;	/* 検索ウィンドウ */
	BVolumeRoster*	fVolRoster;		/* ボリュームの監視用 */
};


#endif  /* _FILE_SEARCH_H_ */

/*
 * End of File
 */
