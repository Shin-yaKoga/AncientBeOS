/***********************************************************************
 * $Id$
 * Copyright (c) 1997 by Fort Gunnison, Inc.
 * 
 * ClickApp.h: ClickApp用のアプリケーションクラス
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 12, 1997
 * Last update: Dec. 12, 1997
 ************************************************************************
*/

#ifndef _CLICK_APP_H_
#define _CLICK_APP_H_

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
 * ClickAppクラスの定義
 */
class ClickApp : public BApplication {
// メソッド
public:
	// 初期化と解放
	ClickApp(void);
	~ClickApp(void);

private:
	// 起動と終了
	void	ReadyToRun(void);
};


#endif  /* _CLICK_APP_H_ */

/*
 * End of File
 */
