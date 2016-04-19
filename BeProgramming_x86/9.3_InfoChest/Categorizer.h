/***********************************************************************
 * $Id$
 * Copyright (c) 1998 by Fort Gunnison, Inc.
 * 
 * Categorizer.h: カテゴリおよびドキュメントの管理用
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 26, 1998
 * Last update:	Feb. 01, 1998
 ************************************************************************
*/

#ifndef _CATEGORIZER_H_
#define _CATEGORIZER_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include <storage/Path.h>
#include <support/List.h>
#include <support/Locker.h>


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/* 関連クラス・構造体 */
class	BQuery;

/* エラーコード */
enum {
	KGCTG_FIRST_ERROR = B_ERRORS_END,
	KGCTG_ALREADY_EXIST,	/* インスタンスが既に存在 */
	KGCTG_NO_INSTANCE,		/* インスタンスが存在しない */
	KGCTG_BAD_PREF_FILE,	/* カテゴリ保存ファイルを開けない */
	KGCTG_FAIL_READ_PREF,	/* カテゴリ保存ファイルを読めない */
	KGCTG_FAIL_WRITE_PREF	/* カテゴリ保存ファイルに書けない */
};


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * Categorizerクラスの定義
 */
class Categorizer : public BLocker {
// メソッド
public:
	// 単一インスタンスの管理 [static]
	static status_t		Startup(void);
	static status_t		Cleanup(void);
	static Categorizer*	GetInstance(void);
	
	// インデックスファイルの管理 [static]
	static status_t		DeleteIndexes(void);
	
	// カテゴリの管理
	void		GetCategories(BList* outCategories);
	status_t	AddCategory(const char* inCategory);
	status_t	RemoveCategory(const char* inCategory);
	
	// ドキュメントの管理
	status_t	MakeQueryFor(const char* inCategory, BQuery* outQuery);
	status_t	AttachCategory(
					const entry_ref* inDocument, const char* inCategory);
	status_t	DetachCategory(const entry_ref* inDocument);

private:
	// 初期化と解放
	Categorizer(void);
	~Categorizer(void);
	status_t	LoadCategories(void);
	
	// カテゴリの管理
	int32		FindCategory(const char* inCategory);
	status_t	SaveCategories(const BList& inCategories);
	
	// ドキュメントの管理
	status_t	CheckIndex(const BVolume& inVolume);

// データメンバ
private:
	BList	fCategories;	/* カテゴリのリスト */
	BPath	fPrefFilePath;	/* カテゴリ保存ファイルのパス */

	// クラスデータ
	static Categorizer*	sSingleton;	/* 単一インスタンス */
};


#endif  /* _CATEGORIZER_H_ */

/*
 * End of File
 */
