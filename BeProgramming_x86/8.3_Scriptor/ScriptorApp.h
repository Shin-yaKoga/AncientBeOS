/***********************************************************************
 * $Id$
 * Copyright (c) 1998 by Fort Gunnison, Inc.
 * 
 * ScriptorApp.h: Scriptor用のアプリケーションクラス
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 03, 1998 
 * Last update:	Jan. 04, 1998
 ************************************************************************
*/

#ifndef _SCRIPTOR_APP_H_
#define _SCRIPTOR_APP_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include <app/Application.h>


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/* 関連クラス・構造体 */
class	BAlert;


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * ScriptorAppクラスの定義
 */
class ScriptorApp : public BApplication {
// メソッド
public:
	// 初期化と解放
	ScriptorApp(void);
	~ScriptorApp(void);

private:
	// 起動と終了
	void	ReadyToRun(void);
	
	// メニュー応答
	void	MessageReceived(BMessage* message);
	void	AboutRequested(void);

// データメンバ
private:
	BAlert*	fAboutBox;	/* アバウトダイアログ */
};


#endif  /* _SCRIPTOR_APP_H_ */

/*
 * End of File
 */
