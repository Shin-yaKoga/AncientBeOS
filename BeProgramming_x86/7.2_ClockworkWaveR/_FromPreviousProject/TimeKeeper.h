/***********************************************************************
 * $Id$
 * Copyright (c) 1997-1998 by Fort Gunnison, Inc.
 * 
 * TimeKeeper.h: 指定したスケジュールでメッセージ送ってくれるクラス
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 20, 1997
 * Last update:	Mar. 04, 1998
 ************************************************************************
*/

#ifndef _TIME_KEEPER_H_
#define _TIME_KEEPER_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include <support/Locker.h>


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/* 関連クラス・構造体 */
class	BHandler;
class	BList;
class	BMessage;
struct	TaskInfo;


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * メッセージ送付のスケジュール情報(ScheduleInfo)
 */
struct ScheduleInfo {
	BHandler*	client;		/* メッセージを送る相手 */
	BMessage*	message;	/* 送付するメッセージ */
	int32		period;		/* メッセージを送る間隔(秒単位) */
	uint32		first_time;	/* 最初にメッセージを送る時刻(秒単位) */
};

/*
 * TimeKeeperクラスの定義
 */
class TimeKeeper {
// メソッド
public:
	// 初期化と解放
	TimeKeeper(void);
	~TimeKeeper(void);

	// イベントスケジューリング
	status_t	InsertSchedule(const ScheduleInfo& inSchedule);
	status_t	RemoveSchedule(const BHandler* inClient, uint32 theCode);
private:
	void		CheckTask(void);

	// クラスメソッド
	static int32	WatchClients(void* data);
	static int		CompareTasks(const void* inTaskA, const void* inTaskB);
	
	// メッセージ送付タスクの操作
	status_t	ExecuteHeadTask(void);
	status_t	AddTask(TaskInfo* inTask);
	int32		FindTask(const BHandler* inClient, uint32 theCode);

// データメンバ
private:
	thread_id	fWatcher;		/* 監視動作用のスレッド */
	bigtime_t	fWatchPeriod;	/* 監視スレッドのスリープ間隔(u秒単位) */
	BList*		fTaskList;		/* メッセージ送付タスクのリスト */
	BLocker		fLocker;		/* 自身のロック用 */
};


#endif  /* _TIME_KEEPER_H_ */

/*
 * End of File
 */
