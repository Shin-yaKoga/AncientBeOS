/***********************************************************************
 * $Id$
 * Copyright (c) 1998 by Fort Gunnison, Inc.
 * 
 * SearchEngine.h: ファイルシステムの検索処理実行オブジェクト
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 28, 1998
 * Last update:	May. 04, 1998
 ************************************************************************
*/

#ifndef _SEARCH_ENGINE_H_
#define _SEARCH_ENGINE_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include <storage/Query.h>
#include <storage/Volume.h>
#include <support/Locker.h>


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/* 関連クラス・構造体 */
class	BHandler;

/* 検索指定情報 */
struct SearchInfo {
	BVolume		volume;		/* 対象ボリューム */
	char*		predicate;	/* 検索条件 */
	BHandler*	client;		/* 検索結果の通知先 */

	SearchInfo(void);
	SearchInfo(const SearchInfo& from);
	~SearchInfo(void);
	SearchInfo&	operator=(const SearchInfo& from);
};

/* 検索結果の通知メッセージ */
enum {
	KGSE_ENTRY_FOUND	= 'efnd',	/* ファイル通知 */
	KGSE_COMPLETED		= 'cmpl'	/* 検索終了通知 */
};

/* 文字列定数 */
extern const char	kSenderArg[];


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * SearchEngineクラスの定義
 */
class SearchEngine : public BLocker {
// メソッド
public:
	// 初期化と解放
	SearchEngine(uint32 targetFlavor);
	~SearchEngine(void);
	
	// 検索処理の開始
	status_t	StartSearch(const SearchInfo& inSearchInfo,
		bool doLiveQuery, int32 inBlockSize = 10);
	status_t	AbortSearch(void);
	bool		IsSearching(void);
	
	// 属性の取得
	dev_t		TargetDevice(void);

private:
	// 検索処理の実行
	status_t	SearchCompleted(BHandler* inTarget);
	status_t	StartWatching(void);
	static int32	ExecSearch(void* data);
	static bool		MatchTo(uint32 inFlavor, const BEntry& inEntry);

// データメンバ
private:
	uint32		fFlavor;		/* 検索対象範囲 */
	thread_id	fThread;		/* 検索スレッド */
	int32		fBlockSize;		/* 一括通知の単位 */
	SearchInfo	fSearchInfo;	/* 検索指定情報 */
	BQuery		fQuery;			/* 問い合わせ文 */
	bool		fQuitting;		/* 強制終了を指示されたか */
};


#endif  /* _SEARCH_ENGINE_H_ */

/*
 * End of File
 */
