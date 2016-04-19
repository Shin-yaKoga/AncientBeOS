/***********************************************************************
 * $Id$
 * Copyright (c) 1997 by Fort Gunnison, Inc.
 * 
 * MessengerApp.h: Messenger用のアプリケーションクラス
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 24, 1997
 * Last update:	Dec. 24, 1997
 ************************************************************************
*/

#ifndef _MESSENGER_APP_H_
#define _MESSENGER_APP_H_

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
 * MessengerAppクラスの定義
 */
class MessengerApp : public BApplication {
// メソッド
public:
	// 初期化と解放
	MessengerApp(void);
	~MessengerApp(void);

private:
	// 起動と終了
	void	ReadyToRun(void);
	
	// メニュー応答
	void	AboutRequested(void);
};


#endif  /* _MESSENGER_APP_H_ */

/*
 * End of File
 */
