/***********************************************************************
 * $Id$
 * Copyright (c) 1997 by Fort Gunnison, Inc.
 * 
 * ButtonApp.h: ButtonApp用のアプリケーションクラス
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 12, 1997
 * Last update: Dec. 12, 1997
 ************************************************************************
*/

#ifndef _BUTTON_APP_H_
#define _BUTTON_APP_H_

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
 * ButtonAppクラスの定義
 */
class ButtonApp : public BApplication {
// メソッド
public:
	// 初期化と解放
	ButtonApp(void);
	~ButtonApp(void);

private:
	// 起動と終了
	void	ReadyToRun(void);
};


#endif  /* _BUTTON_APP_H_ */

/*
 * End of File
 */
