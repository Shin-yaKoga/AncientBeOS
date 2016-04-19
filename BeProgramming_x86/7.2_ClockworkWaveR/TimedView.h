/***********************************************************************
 * $Id$
 * Copyright (c) 1997 by Fort Gunnison, Inc.
 * 
 * TimedView.h: 決まったスケジュールで動作するビュー
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 22, 1997
 * Last update:	Dec. 22, 1997
 ************************************************************************
*/

#ifndef _TIMED_VIEW_H_
#define _TIMED_VIEW_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include <interface/View.h>


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/* 関連クラス・構造体 */
class	TimeKeeper;
struct	ScheduleInfo;


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * TimedViewクラスの定義
 */
class TimedView : public BView {
// メソッド
public:
	// 初期化と解放
	TimedView(BRect frame, const char* title,
		uint32 resizedMask, uint32 flags, TimeKeeper* inTimeKeeper);
	virtual ~TimedView(void);

protected:
	// スケジュール情報の管理
	status_t	AddSchedule(const ScheduleInfo& inSchedule);
	status_t	RmvSchedule(uint32 theCode);

// データメンバ
private:
	TimeKeeper*	fTimeKeeper;	/* タイミング動作用のスケジューラ */
};


#endif  /* _TIMED_VIEW_H_ */

/*
 * End of File
 */
