/***********************************************************************
 * $Id$
 * Copyright (c) 1997 by Fort Gunnison, Inc.
 * 
 * ClockworkWaveApp.h: ClockworkWave用のアプリケーションクラス
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 21, 1997
 * Last update:	Dec. 22, 1997
 ************************************************************************
*/

#ifndef _CLOCKWORK_WAVE_APP_H_
#define _CLOCKWORK_WAVE_APP_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include <app/Application.h>


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/* 関連クラス・構造体 */
class	RegularWindow;
class	TimeKeeper;
class	BMenuBar;


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * ClockworkWaveAppクラスの定義
 */
class ClockworkWaveApp : public BApplication {
// メソッド
public:
	// 初期化と解放
	ClockworkWaveApp(void);
	~ClockworkWaveApp(void);

private:
	// 起動と終了
	void		ReadyToRun(void);
	void		Quit(void);

	// ビュー部品の生成
	void		MakeClockWin(void);
	void		MakeWaveWin(void);
	BMenuBar*	MakeMenuBar(bool forClock);

	// メニューの応答
	void		MessageReceived(BMessage* message);
	void		AboutRequested(void);
	void		CheckWindow(BMessage* message);
	void		ShowClock(void);
	void		ShowWave(void);
	
	// ウィンドウ操作
	void		ShowWindow(BWindow* inWindow);

// データメンバ
private:
	RegularWindow*	fClockWin;		/* 時計表示のウィンドウ */
	RegularWindow*	fWaveWin;		/* 水面表示のウィンドウ */
	TimeKeeper*		fTimeKeeper;	/* タイミング動作用 */
};


#endif  /* _CLOCKWORK_WAVE_APP_H_ */

/*
 * End of File
 */
