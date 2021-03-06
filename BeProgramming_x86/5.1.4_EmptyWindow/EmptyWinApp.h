/***********************************************************************
 * $Id$
 * Copyright (c) 1997 by Fort Gunnison, Inc.
 * 
 * EmptyWinApp.h: 空のウィンドウを表示するアプリケーションクラス
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 10, 1997
 * Last update: Dec. 10, 1997
 ************************************************************************
*/

#ifndef _EMPTY_WIN_APP_H_
#define _EMPTY_WIN_APP_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include <app/Application.h>


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * EmptyWinAppクラスの定義
 */
class EmptyWinApp : public BApplication {
// メソッド
public:
	// 初期化と解放
	EmptyWinApp(void);
	~EmptyWinApp(void);

private:
	// 起動と終了
	void	ReadyToRun(void);
};


#endif  /* _EMPTY_WIN_APP_H_ */

/*
 * End of File
 */
