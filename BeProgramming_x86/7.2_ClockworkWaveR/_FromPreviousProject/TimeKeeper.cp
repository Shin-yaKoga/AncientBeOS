/************************************************************************
 * $Id$
 * Copyright (c) 1997-1998 Fort Gunnison, Inc.
 *
 * TimeKeeper.cp: TimeKeeperクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 20, 1997
 * Last update:	Mar. 04, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "TimeKeeper.h"
#include "KGUtility/kgDebug.h"

#include <support/Autolock.h>
#include <support/Debug.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/* メッセージ送付タスク */
struct TaskInfo {
	uint32			exec_time;	/* 遂行時刻 */
	ScheduleInfo	schedule;	/* スケジュール情報 */
};


/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/* TimeKeeperクラスの非公開メソッド */
/*
 * イベントスケジューリング; TimeKeeper
 */
void
TimeKeeper::CheckTask (void)
{
	BAutolock	lock(fLocker);

	/* 指定時刻に達している全てのタスクを実行 */
	while (! fTaskList->IsEmpty()) {
		TaskInfo*	headTask = (TaskInfo*)fTaskList->FirstItem();
	
		if (headTask->exec_time > ::real_time_clock())
			break;  /* 必要なタスクの実行が終了 */
		(void)this->ExecuteHeadTask();
	}
	
	/* 必要なら監視スレッドを停止 */
	if (fTaskList->IsEmpty())
		(void)::suspend_thread(fWatcher);
	
	return;
}

/*
 * クラスメソッド; TimeKeeper
 */
int32
TimeKeeper::WatchClients (void* data)
{
	TimeKeeper*	theObj = (TimeKeeper*)data;
	
	while (true) {
		/* 必要な時間だけスリープ */
		(void)::snooze(theObj->fWatchPeriod);

		/* タスクをチェック */
		theObj->CheckTask();
	}
	
	return 0;
}

int
TimeKeeper::CompareTasks (const void* inTaskA, const void* inTaskB)
{
	int		retVal = 0;
	uint32	timeA  = (*(TaskInfo**)inTaskA)->exec_time;
	uint32	timeB  = (*(TaskInfo**)inTaskB)->exec_time;
	
	if (timeA < timeB)
		retVal = -1;
	else if (timeA > timeB)
		retVal = 1;
	
	return retVal;
/*
 * 注意：BListの要素間比較手続きには、要素そのもの(ポインタ)ではなく、要
 *		素の配列、すなわちポインタ配列が渡される。このため、渡された引数
 *		に対して参照解除(dereference)する必要があるので注意すること。
 *		('98. 1/7, koga@ftgun.co.jp)
 */
}

/*
 * メッセージ送付タスクの操作; TimeKeeper
 */
status_t
TimeKeeper::ExecuteHeadTask (void)
{
	status_t	sts;
	TaskInfo*	theTask;
	BLooper*	target;

	ASSERT(! fTaskList->IsEmpty());

	/* 先頭のタスク情報を取得 */
	theTask = (TaskInfo*)fTaskList->FirstItem();
	target  = theTask->schedule.client->Looper();
	ASSERT(target != NULL);
	ASSERT(theTask->exec_time <= ::real_time_clock());

	/* 送り先にメッセージを送付 */
	sts = target->PostMessage(
		theTask->schedule.message, theTask->schedule.client);
	if (sts != B_OK)
		goto err;
	(void)fTaskList->RemoveItem(theTask);  /* 先頭要素をリストから削除 */

	/* 繰り返し処理なら再度追加 */
	if (theTask->schedule.period > 0) {
		theTask->exec_time += theTask->schedule.period;
		sts = this->AddTask(theTask);
		if (sts != B_OK)
			goto err;
	}

	return B_OK;
err:
	::Error("TimeKeeper::ExecuteHeadTask", sts);
	return sts;
}

status_t
TimeKeeper::AddTask (TaskInfo* inTask)
{
	/* リストの先頭に要素を追加 */
	(void)fTaskList->AddItem(inTask, 0);

	/* リスト要素を昇順でソート */
	fTaskList->SortItems(CompareTasks);

	return B_OK;
}

int32
TimeKeeper::FindTask (const BHandler* inHandler, uint32 theCode)
{
	int32	theIndex = -1;

	for (int32 i = 0, n = fTaskList->CountItems(); i < n; ++i) {
		TaskInfo*	aTask = (TaskInfo*)fTaskList->ItemAt(i);
		
		if (aTask->schedule.client == inHandler
				&& aTask->schedule.message->what == theCode) {
			theIndex = i;
			break;
		}
	}
	
	return theIndex;
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/* TimeKeeperクラスの公開メソッド */
/*
 * 初期化と解放; TimeKeeper
 */
TimeKeeper::TimeKeeper (void)
{
	fWatcher     = -1;
	fTaskList    = new BList();
	fWatchPeriod = 500 * 1000;  /* 0.5秒 */
}

TimeKeeper::~TimeKeeper (void)
{
	BAutolock	lock(fLocker);

	/* スレッドが生きていたら停止して解放 */
	if (fWatcher >= 0)
		(void)kill_thread(fWatcher);

	/* 不要になったスケジュール情報を解放 */
	while (! fTaskList->IsEmpty()) {
		TaskInfo*	aTask = (TaskInfo*)fTaskList->RemoveItem((int32)0);
		
		delete aTask->schedule.message;
		delete aTask;
	}
	delete fTaskList;
/*
 * 注意：ここでは自身にロックをかけて排他制御を行っているが、これは完全
 *		なものではない。InsertSchedule()やRemoveSchedule()が、デストラ
 *		クタを呼び出したのとは別のスレッドから同時に呼び出された場合、
 *		デストラクタによってfTaskListが解放された後にそれらのメソッドが
 *		実行される可能性があるからである。すなわち、InsertSchedule()や
 *		RemoveSchedule()がfTaskListにアクセスした際に、それが既に解放さ
 *		れて無効な領域になっている可能性のあることを考慮していない。
 *		ここでロックをかけているのは、fWatcherスレッドを kill する際に、
 *		その実行が中途半端な状態で終わらないことを保障するのが目的であ
 *		り、他のメソッドとの間での完全な排他制御を実現するものではない。
 *
 *		　なお、fTaskListを解放したらそれをNULLにセットし、他のメソッド
 *		ではfTaskListがNULLかどうかをチェックするように *改良*したとし
 *		ても、TimeKeeperクラスのインスタンス全体が占める領域はデストラ
 *		クタの解放処理によって無効になるため、問題を解決したことにはな
 *		らない。('98. 3/4, koga@ftgun.co.jp)
 */
}

/*
 * イベントスケジューリング; TimeKeeper
 */
status_t
TimeKeeper::InsertSchedule (const ScheduleInfo& inSchedule)
{
	status_t		sts;
	TaskInfo*		newTask = new TaskInfo();
	int32			theIndex;
	BAutolock		lock(fLocker);

	/* 既に同じスケジュールがあれば削除 */
	theIndex = this->FindTask(
		inSchedule.client, inSchedule.message->what);
	if (theIndex >= 0)
		(void)fTaskList->RemoveItem(theIndex);
	
	/* タスクリストにエントリを追加 */
	newTask->exec_time = inSchedule.first_time;
	newTask->schedule  = inSchedule;
	sts = this->AddTask(newTask);
	if (sts != B_OK)
		goto err;

	/* 必要なら監視スレッドを生成して起動 */
	if (fWatcher < 0) {
		fWatcher = ::spawn_thread(
			WatchClients, "watcher", B_NORMAL_PRIORITY, this
		);
	}
	sts = ::resume_thread(fWatcher);
	if (sts != B_OK)
		goto err;
	
	return B_OK;
err:
	::Error("TimeKeeper::InsertSchedule", sts);
	return sts;
/*
 * 注意：上では無条件に resume_thread() を実行しているが、suspend してい
 *		ないスレッドに対しては、この手続きは何もしない。したがって、これ
 *		で問題はない。('97. 12/20, koga@ftgun.co.jp)
 */
}

status_t
TimeKeeper::RemoveSchedule (const BHandler* inClient, uint32 theCode)
{
	status_t	sts;
	TaskInfo*	theTask;
	BAutolock	lock(fLocker);

	/* タスクリストからエントリを削除 */
	theTask = (TaskInfo*)fTaskList->RemoveItem(
		this->FindTask(inClient, theCode)
	);
	if (! theTask) {
		sts = B_ERROR;
		goto err;  /* 見つからない */
	}
	delete theTask->schedule.message;
	delete theTask;
	
	/* 必要なら監視スレッドを停止 */
	if (fTaskList->IsEmpty())
		(void)::suspend_thread(fWatcher);
	
	return B_OK;
err:
	::Error("TimeKeeper::RemoveSchedule", sts);
	return sts;
}


/*
 * End of File
 */
