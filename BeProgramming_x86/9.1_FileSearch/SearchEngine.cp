/************************************************************************
 * $Id$
 * Copyright (c) 1998 Fort Gunnison, Inc.
 *
 * SearchEngine.cp: SearchEngineクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 28, 1998
 * Last update:	May. 18, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "SearchEngine.h"
#include "KGUtility/kgDebug.h"

#include <support/Autolock.h>
#include <support/Debug.h>

#include <string.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/* 文字列定数 */
const char	kSenderArg[]	= "sender";
const char	kRefsArg[]		= "refs";


/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/*
 * SearchEngineクラスの非公開メソッド
 */
status_t
SearchEngine::SearchCompleted (BHandler* inTarget)
{
	status_t	sts;
	BMessage	theMessage(KGSE_COMPLETED);
	
	sts = theMessage.AddPointer(kSenderArg, this);
	if (sts != B_OK)
		goto err;
	sts = inTarget->Looper()->PostMessage(&theMessage, inTarget);
	if (sts != B_OK)
		goto err;
	
	return B_OK;
err:
	::Error("SearchEngine::SearchCompleted", sts);
	return sts;
/*
 * 注意：このメソッドの中で自身にロックをかけてはいけない。
 *		AbortSearch()メソッドでは、自身にロックをかけたうえで、
 *		wait_for_thread()によって検索スレッドと同期をとっている。した
 *		がって、このメソッドを含め検索スレッドが呼び出すメソッドの中で
 *		ロックをかけた場合、デッドロックが生じてしまうからである。
 *		('98. 5/4, koga@ftgun.co.jp)
 */
}

status_t
SearchEngine::StartWatching (void)
{
	status_t	sts;
	
	ASSERT(this->IsLocked());
	ASSERT(! this->IsSearching());

	if (fSearchInfo.client != NULL) {
		BMessenger	theMessenger(fSearchInfo.client, NULL, &sts);
		
		if (sts != B_OK)
			goto err;
		sts = fQuery.SetTarget(theMessenger);
		if (sts != B_OK)
			goto err;
	}
	
	return B_OK;
err:
	::Error("SearchEngine::StartWatching", sts);
	return sts;
}

int32
SearchEngine::ExecSearch (void* data)
{
	status_t		sts;
	SearchEngine*	theObj = (SearchEngine*)data;
	BEntry			anEntry;
	entry_ref		aRef;
	BMessage		theMessage(KGSE_ENTRY_FOUND);
	BQuery*			theQuery  = &theObj->fQuery;
	BHandler*		theClient = theObj->fSearchInfo.client;
	dev_t			theDevice = theObj->fSearchInfo.volume.Device();
	
	/* メッセージの引数を初期化 */
	(void)theMessage.AddRef(kRefsArg, &aRef);
	
	/* 検索処理を実行 */
	sts = theQuery->Fetch();
	if (sts != B_OK)
		goto err;
	
	/* 検索結果を取得して依頼先に通知 */
	while ((sts = theQuery->GetNextEntry(&anEntry)) == B_OK) {
		if (theObj->fQuitting) {  /* 強制終了を指示されていたら終了 */
			theObj->fThread = -1;
//			::exit_thread(B_OK);
			return B_OK;  /* '98. 5/18 */
		}

		/* 取得したエントリ情報を入れたメッセージを送付 */
		if (theObj->fFlavor != B_ANY_NODE
				&& !MatchTo(theObj->fFlavor, anEntry))
			continue;  /* 検索対象ではない */
		anEntry.GetRef(&aRef);
		theMessage.ReplaceRef(kRefsArg, &aRef);
		sts = theClient->Looper()->PostMessage(&theMessage, theClient);
		if (sts != B_OK)
			goto err;
		
		/* ちょっと休憩 */
		::snooze(20 * 1000);
	}
	
	/* 全部終わったら完了の通知 */
	sts = theObj->SearchCompleted(theClient);
	theObj->fThread = -1;
	
	return sts;
err:
	::Error("SearchEngine::ExecSearch", sts);
	return sts;
/*
 * 注意：本当は、theObj->fBlockSizeを使って、fBlockSize個のエントリを
 *		一つのメッセージにまとめて入れて送る予定だったのだが、ここで
 *		は実装していない。気が向いた人がいたらやってみて下さい:-)
 *		('98. 1/29, koga@ftgun.co.jp)
 */
}

bool
SearchEngine::MatchTo (uint32 inFlavor, const BEntry& inEntry)
{
	bool	isMatch = false;

	if (inEntry.IsFile()) {
		if (inFlavor & B_FILE_NODE)
			isMatch = true;
	} else if (inEntry.IsDirectory()) {
		if (inFlavor & B_DIRECTORY_NODE)
			isMatch = true;
	} else if (inEntry.IsSymLink()) {
		if (inFlavor & B_SYMLINK_NODE)
			isMatch = true;
	}
	
	return isMatch;
/*
 * 注意：BEntry::IsFile()などは、結局BNode::GetStat()で取得したstat構造
 *		体の内容を見ているだけだと思うので、速度が問題になるようであれ
 *		ば直接stat構造体を見て判定するように修正すること。
 *		('98. 1/29, koga@ftgun.co.jp)
 */
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * SearchInfoクラスのメソッド
 */
SearchInfo::SearchInfo (void)
{
	predicate = NULL;
	client    = NULL;
}

SearchInfo::SearchInfo (const SearchInfo& from)
{
	if (from.predicate == NULL)
		predicate = NULL;
	else {
		predicate = new char[strlen(from.predicate) + 1];
		strcpy(predicate, from.predicate);
	}
	volume = from.volume;
	client = from.client;
}

SearchInfo::~SearchInfo (void)
{
	delete [] predicate;
}

SearchInfo&
SearchInfo::operator=(const SearchInfo& from)
{
	if (from.predicate == NULL || from.predicate != predicate) {
		delete [] predicate;
		if (from.predicate == NULL)
			predicate = NULL;
		else {
			predicate = new char[strlen(from.predicate) + 1];
			strcpy(predicate, from.predicate);
		}
		volume = from.volume;
		client = from.client;
	}
	
	return *this;
}


/* SearchEngineクラスの公開メソッド */
/*
 * 初期化と解放; SearchEngine
 */
SearchEngine::SearchEngine (uint32 targetFlavor)
{
	fFlavor = targetFlavor;
	fThread = -1;  /* 無効値 */
}

SearchEngine::~SearchEngine (void)
{
	ASSERT(! this->IsSearching());

	/* 検索中の場合は後始末 */
	if (this->IsSearching())
		(void)this->AbortSearch();
/*
 * 注意：検索中でないことをassertionでチェックしているため、本来は強制
 *		終了処理は必要ない。ここで行っているのは、あくまでの念のための
 *		措置である。('98. 5/4, koga@ftgun.co.jp)
 */
}

/*
 * 検索処理の開始と終了; SearchEngine
 */
status_t
SearchEngine::StartSearch (
	const SearchInfo& inSearchInfo, bool doLiveQuery, int32 inBlockSize)
{
	BAutolock	lock(this);  /* 自身をロック */
	status_t	sts;

	ASSERT(! this->IsSearching());

	/* 検索情報を保存 */
	fSearchInfo = inSearchInfo;
	fBlockSize  = inBlockSize;
	
	/* 検索条件を設定 */
	sts = fQuery.Clear();
	if (sts != B_OK)
		goto err;
	sts = fQuery.SetPredicate(fSearchInfo.predicate);
	if (sts != B_OK)
		goto err;
	sts = fQuery.SetVolume(&fSearchInfo.volume);
	if (sts != B_OK)
		goto err;
	
	/* 必要ならボリュームの監視を始める */
	if (doLiveQuery)
		this->StartWatching();
	
	/* 検索用のスレッドを生成して起動 */
	fQuitting = false;
	fThread = ::spawn_thread(
		ExecSearch, "searcher", B_LOW_PRIORITY, this
	);
	sts = ::resume_thread(fThread);
	if (sts != B_OK)
		goto err;
	
	return B_OK;
err:
	::Error("SearchEngine::StartSearch", sts);
	return sts;
}

status_t
SearchEngine::AbortSearch (void)
{
	/* 自身をロック */
	BAutolock	lock(this);
	
	/* ライブクエリーを行っている場合は停止 */
	if (fQuery.IsLive())
		(void)fQuery.Clear();
	
	/* 検索用のスレッドを破棄 */
	if (this->IsSearching()) {
		status_t	sts;
	
		fQuitting = true;
		(void)::wait_for_thread(fThread, &sts);
	}
	
	return B_OK;
/*
 * 注意：スレッドを終了するのにkill_thread()を使うのは安全が保障されな
 *		いため、終了通知フラグ(fQuitting)を使ってスレッド自身に終了させ
 *		ている。また、スレッドの終了と同期をとるため、wait_for_thread()
 *		を使って待ち合わせを行っている。なお、fThreadの値は検索スレッド
 *		によってリセットされるため、ここで更新する必要はない。
 *		('98. 5/1, koga@ftgun.co.jp)
 */
}

bool
SearchEngine::IsSearching (void)
{
	/* 自身をロック */
	BAutolock	lock(this);
	
	return (fThread >= 0);
}

/*
 * 属性の取得; SearchEngine
 */
dev_t
SearchEngine::TargetDevice (void)
{
	/* 自身をロック */
	BAutolock	lock(this);
	
	return fSearchInfo.volume.Device();
}


/*
 * End of File
 */
