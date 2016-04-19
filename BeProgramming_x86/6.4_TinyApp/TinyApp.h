/***********************************************************************
 * $Id$
 * Copyright (c) 1997 by Fort Gunnison, Inc.
 * 
 * TinyApp.h: TinyApp用のアプリケーションクラス
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 14, 1997
 * Last update: Dec. 14, 1997
 ************************************************************************
*/

#ifndef _TINY_APP_H_
#define _TINY_APP_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include <app/Application.h>


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/* 関連クラス・構造体 */
class	BMenuBar;


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * TinyAppクラスの定義
 */
class TinyApp : public BApplication {
// メソッド
public:
	// 初期化と解放
	TinyApp(void);
	~TinyApp(void);

private:
	// 起動と終了
	void		ReadyToRun(void);
	
	// メニュー応答
	BMenuBar*	MakeMenuBar(void);
	void		AboutRequested(void);
};



#endif  /* _TINY_APP_H_ */

/*
 * End of File
 */
