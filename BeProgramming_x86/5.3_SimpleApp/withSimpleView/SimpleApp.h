/***********************************************************************
 * $Id$
 * Copyright (c) 1997 by Fort Gunnison, Inc.
 * 
 * SimpleApp.h: SimpleWindowを表示するアプリケーションクラス
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 10, 1997
 * Last update: Dec. 10, 1997
 ************************************************************************
*/

#ifndef _SIMPLE_APP_H_
#define _SIMPLE_APP_H_

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
 * SimpleAppクラスの定義
 */
class SimpleApp : public BApplication {
// メソッド
public:
	// 初期化と解放
	SimpleApp(void);
	~SimpleApp(void);

private:
	// 起動と終了
	void	ReadyToRun(void);
};


#endif  /* _SIMPLE_APP_H_ */

/*
 * End of File
 */
