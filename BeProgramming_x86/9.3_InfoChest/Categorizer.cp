/************************************************************************
 * $Id$
 * Copyright (c) 1998 Fort Gunnison, Inc.
 *
 * Categorizer.cp: Categorizerクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 26, 1998
 * Last update:	May. 01, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "Categorizer.h"
#include "KGUtility/kgAlert.h"
#include "KGUtility/kgDebug.h"

#include <storage/FindDirectory.h>
#include <storage/Query.h>
#include <storage/Volume.h>
#include <storage/VolumeRoster.h>
#include <support/Autolock.h>
#include <kernel/fs_index.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/* 各種定数 */
const int32	kMaxCategoryLen	= 255;


/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/* Categorizerのクラスデータ */
Categorizer*	Categorizer::sSingleton = NULL;

/* 文字列定数 */
const char	kPrefFile[]			= "InfoChest";
const char	kCategoryIndex[]	= "FTGUN:CATEGORY";
const char	kCreateIndexMsg[]	= "The volume has not been "
	"indexed for category.\n"
	"Would you like to create it?";


/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/* Categorizerクラスの非公開メソッド */
/*
 * 初期化と解放; Categorizer
 */
Categorizer::Categorizer (void)
{  /* do nothing */  }

Categorizer::~Categorizer (void)
{
	/* 不要領域を解放 */
	for (int32 i = 0, n = fCategories.CountItems(); i < n; ++i) {
		char*	anItem = (char*)fCategories.ItemAt(i);
		
		delete anItem;
	}
}

status_t
Categorizer::LoadCategories (void)
{
	status_t	sts;
	BPath		thePath;
	FILE*		thePrefFile;
	char		strBuf[kMaxCategoryLen+1];

	/* カテゴリ保存ファイルのパスを初期化 */
	sts = ::find_directory(B_COMMON_SETTINGS_DIRECTORY, &thePath, true);
//	sts = ::find_directory(B_USER_SETTINGS_DIRECTORY, &thePath, true);
	if (sts != B_OK)
		goto err;
	sts = thePath.Append(kPrefFile);
	if (sts != B_OK)
		goto err;
	fPrefFilePath = thePath;

	/* カテゴリ一覧をロード */
#ifdef Pending  /* '98. 1/27 (koga@ftgun.co.jp) */
	thePrefFile = fopen(fPrefFilePath.Path(), "a");
	if (! thePrefFile) {
		sts = KGCTG_BAD_PREF_FILE;  /* ファイルを作れない */
		goto err;
	}
	if (fseek(thePrefFile, 0, SEEK_SET) != 0) {
		sts = KGCTG_FAIL_PREF_READ;  /* ファイルを読めない */
		goto err;
	}
#else  /* alternative src */
	thePrefFile = fopen(fPrefFilePath.Path(), "r");
	if (! thePrefFile) {
		thePrefFile = fopen(fPrefFilePath.Path(), "w");
		if (! thePrefFile) {
			sts = KGCTG_BAD_PREF_FILE;
			goto err;
		}
	}
/*
 * 注意：PR2付属のmwccでは、"a"モードでfopen()を実行すると、fseek()を
 *		実行しても先頭にシークできないようである。本来は、"a+"によって
 *		バイナリファイルとしてオープンしない限りは先頭にシークできる筈
 *		なのだが、仕方ないので上のようにしている。
 *		('98. 1/27, koga@ftgun.co.jp)
 */
#endif
	while (fgets(strBuf, sizeof(strBuf), thePrefFile) != NULL) {
		char*	newItem;
		int32	theLength = strlen(strBuf);
	
		if (strBuf[theLength-1] == '\n')  /* 改行文字を削除 */
			strBuf[--theLength] = '\0';
		newItem = new char[theLength + 1];
		strcpy(newItem, strBuf);
		fCategories.AddItem(newItem);
	}
	fclose(thePrefFile);
	
	return B_OK;
err:
	::Error("Categorizer::LoadCategories", sts);
	return sts;
}

/*
 * カテゴリの管理; Categorizer
 */
int32
Categorizer::FindCategory (const char* inCategory)
{
	int32	theIndex = -1;
	
	for (int32 i = 0, n = fCategories.CountItems(); i < n; ++i) {
		char*	aCategory = (char*)fCategories.ItemAt(i);
		
		if (strcmp(aCategory, inCategory) == 0) {
			theIndex = i;
			break;
		}
	}
	
	return theIndex;
}

status_t
Categorizer::SaveCategories (const BList& inCategories)
{
	status_t	sts;
	FILE*		thePrefFile;
	char		strBuf[kMaxCategoryLen+1];

	/* カテゴリ保存ファイルを開く */
	thePrefFile = fopen(fPrefFilePath.Path(), "w");
	if (! thePrefFile) {
		sts = KGCTG_BAD_PREF_FILE;
		goto err;
	}
	
	/* 全てのカテゴリを書き込む */
	for (int32 i = 0, n = inCategories.CountItems(); i < n; ++i) {
		char*	aCategory = (char*)inCategories.ItemAt(i);
		
		strncpy(strBuf, aCategory, kMaxCategoryLen);
		strBuf[kMaxCategoryLen] = '\0';
		if (fputs(strBuf, thePrefFile) == EOF)
			goto err_write;
		if (fputc('\n', thePrefFile) == EOF)  /* 改行文字を付加 */
			goto err_write;
	}
	
	/* カテゴリ保存ファイルを閉じる */
	fclose(thePrefFile);
	
	return B_OK;
err_write:
	fclose(thePrefFile);
	sts = KGCTG_FAIL_WRITE_PREF;
err:
	::Error("Categorizer::SaveCategories", sts);
	return sts;
/*
 * 注意：本当は、現在のシステムフォントなどから得られる言語圏情報を使い、
 *		適切なバイト境界で文字列の切り捨てを行うべきである。
 *		('98. 1/27, koga@ftgun.co.jp)
 */
}

/*
 * ドキュメントの管理; Categorizer
 */
status_t
Categorizer::CheckIndex (const BVolume& inVolume)
{
	status_t	sts = B_OK;
	int			retVal;
	index_info	theInfo;
	
	if (! inVolume.KnowsQuery())
		return B_UNSUPPORTED;
	retVal = ::fs_stat_index(
		inVolume.Device(), kCategoryIndex, &theInfo
	);
	if (retVal != 0)
		sts = errno;
	if (sts == B_ENTRY_NOT_FOUND) {
		if (! ::KGConfirm(kCreateIndexMsg))
			return B_CANCELED;
		else {
			retVal = ::fs_create_index(
				inVolume.Device(), kCategoryIndex, B_STRING_TYPE, 0
			);
			if (retVal != 0)
				sts = errno;
			else
				sts = 0;
		}
	}
	if (sts != B_OK)
		goto err;
	
	return B_OK;
err:
	::Error("Categorizer::CheckIndex", sts);
	return B_ERROR;
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/* Categorizerクラスの公開メソッド */
/*
 * 単一インスタンスの管理; Categorizer
 */
status_t
Categorizer::Startup (void)
{
	status_t		sts;
	Categorizer*	theInstance;

	if (sSingleton != NULL)
		return KGCTG_ALREADY_EXIST;
	
	theInstance = new Categorizer();
	sts = theInstance->LoadCategories();
	if (sts != B_OK) {
		delete theInstance;
		goto err;
	}
	sSingleton = theInstance;
	
	return B_OK;
err:
	::Error("Categorizer::Startup", sts);
	return sts;
}

status_t
Categorizer::Cleanup (void)
{
	if (sSingleton == NULL)
		return KGCTG_NO_INSTANCE;
	
	delete sSingleton;
	sSingleton = NULL;
	
	return B_OK;
}

Categorizer*
Categorizer::GetInstance (void)
{
	return sSingleton;
}

/*
 * インデックスファイルの管理; Categorizer
 */
status_t
Categorizer::DeleteIndexes (void)
{
	status_t		sts;
	BVolume			aVolume;
	BVolumeRoster	volRoster;
	
	/* 問い合わせ可能な全ボリュームに対して繰り返し */
	while (volRoster.GetNextVolume(&aVolume) == B_OK) {
		if (aVolume.KnowsQuery()) {
			sts = ::fs_remove_index(aVolume.Device(), kCategoryIndex);
			if (sts != B_OK && sts != B_ENTRY_NOT_FOUND)
				goto err;
		}
	}
	
	return B_OK;
err:
	::Error("Categorizer::DeleteIndexes", sts);
	return B_ERROR;
}

/*
 * カテゴリの管理; Categorizer
 */
void
Categorizer::GetCategories (BList* outCategories)
{
	BAutolock	lock(this);
	
	*outCategories = fCategories;
	
	return;
}

status_t
Categorizer::AddCategory (const char* inCategory)
{
	BAutolock	lock(this);
	status_t	sts;
	int32		theIndex;
	BList		tmpList = fCategories;
	char*		newCategory;

	/* カテゴリが存在するかどうかをチェック */
	theIndex = this->FindCategory(inCategory);
	if (theIndex >= 0)
		return B_OK;  /* 既に存在している */
	
	/* 一時リストを作成してカテゴリを追加 */
	(void)tmpList.AddItem((void*)inCategory);

	/* カテゴリファイルの内容を変更 */
	sts = this->SaveCategories(tmpList);
	if (sts != B_OK)
		goto err;

	/* キャッシュしているリスト内容を変更 */
	newCategory = new char[strlen(inCategory) + 1];
	strcpy(newCategory, inCategory);
	(void)fCategories.AddItem(newCategory);
	
	return B_OK;
err:
	::Error("Categorizer::AddCategory", sts);
	return sts;
}

status_t
Categorizer::RemoveCategory (const char* inCategory)
{
	BAutolock	lock(this);
	status_t	sts;
	int32		theIndex;
	BList		tmpList = fCategories;

	/* カテゴリが存在するかどうかをチェック */
	theIndex = this->FindCategory(inCategory);
	if (theIndex < 0)
		return B_OK;  /* 存在していない */
	
	/* 一時リストを作成してカテゴリを削除 */
	(void)tmpList.RemoveItem(theIndex);
	
	/* カテゴリファイルの内容を変更 */
	sts = this->SaveCategories(tmpList);
	if (sts != B_OK)
		goto err;
	
	/* キャッシュしているリスト内容を変更 */
	(void)fCategories.RemoveItem(theIndex);
	
	return B_OK;
err:
	::Error("Categorizer::RemoveCategory", sts);
	return sts;
/*
 * 注意：本当は、指定されたカテゴリ(キーワード)が設定されているファイル
 *		やフォルダを全て検索し、それらからカテゴリ属性を削除すべきであ
 *		る。しかし、ここではその処理を省略している。
 *		('98. 5/1, koga@ftgun.co.jp)
 */
}

/*
 * ドキュメントの管理; Categorizer
 */
status_t
Categorizer::MakeQueryFor (const char* inCategory, BQuery* outQuery)
{
	BAutolock	lock(this);
	status_t	sts;
	
	/* 念のため */
	sts = outQuery->Clear();
	if (sts != B_OK)
		goto err;
	
	/* カテゴリ検索用の問い合わせ文を作成 */
	outQuery->PushAttr(kCategoryIndex);
	outQuery->PushString(inCategory, true);
	outQuery->PushOp(B_EQ);
	
	return B_OK;
err:
	::Error("Categorizer::MakeQuery", sts);
	return sts;
}

status_t
Categorizer::AttachCategory (
	const entry_ref* inDocument, const char* inCategory)
{
	BAutolock	lock(this);
	status_t	sts;
	BVolume		theVolume;
	BNode		theNode;
	
	/* ボリューム情報を取得 */
	sts = theVolume.SetTo(inDocument->device);
	if (sts != B_OK)
		goto err;
	
	/* 必要なインデックスがあるかチェック */
	sts = this->CheckIndex(theVolume);
	if (sts == B_UNSUPPORTED || sts == B_CANCELED)
		return sts;
	else if (sts != B_OK)
		goto err;

	/* カテゴリ属性を設定 */
	sts = theNode.SetTo(inDocument);
	if (sts != B_OK)
		goto err;
	(void)theNode.WriteAttr(
		kCategoryIndex, B_STRING_TYPE,
		0, inCategory, strlen(inCategory) + 1
	);
	
	return B_OK;
err:
	::Error("Categorizer::AttachCategory", sts);
	return sts;
}

status_t
Categorizer::DetachCategory (const entry_ref* inDocument)
{
	status_t	sts;
	BAutolock	lock(this);
	BNode		theNode(inDocument);
	
	/* 正しく初期化できたかチェック */
	if ((sts = theNode.InitCheck()) != B_OK)
		goto err;
	
	/* カテゴリ属性を削除 */
	sts = theNode.RemoveAttr(kCategoryIndex);
	if (sts != B_OK)
		goto err;
	
	return B_OK;
err:
	::Error("Categorizer::DetachCategory", sts);
	return sts;
}


/*
 * End of File
 */
