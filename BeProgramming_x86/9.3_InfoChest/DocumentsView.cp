/************************************************************************
 * $Id$
 * Copyright (c) 1998 Fort Gunnison, Inc.
 *
 * DocumentsView.cp: DocumentsViewクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Feb. 01, 1998
 * Last update:	May. 11, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "DocumentsView.h"
#include "Categorizer.h"
#include "GUI/EntryListItem.h"
#include "GUI/EntryListView.h"
#include "KGUtility/kgAlert.h"
#include "KGUtility/kgDebug.h"

#include <interface/ScrollBar.h>
#include <interface/ScrollView.h>
#include <storage/FilePanel.h>
#include <support/Beep.h>
#include <support/Debug.h>

#include <string.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/* 文字列定数 */
const char	kOpCodeArg[]	= "opcode";
const char	kDeviceArg[]	= "device";
const char	kDirectoryArg[]	= "directory";
const char	kNameArg[]		= "name";
const char	kRefsArg[]		= "refs";

const char	kListView[]	= "Found List";
const char	kScroller[]	= "Scroller";

const char	kUnsupportedVolMsg[]	= "The volume has not query aware.";

/* DocumentsViewのクラスデータ */
BFilePanel*	DocumentsView::sOpenPanel = NULL;


/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/* DocumentsViewクラスの非公開メソッド */
/*
 * メッセージ処理; DocumentsView
 */
void
DocumentsView::MessageDropped (BMessage* message)
{
	BPoint		theDropPoint = message->DropPoint();
	BListView*	theListView  = this->GetEntryListView();

	/* スクリーン座標からローカル座標に変換 */	
	theListView->ConvertFromScreen(&theDropPoint);
	
	/* リストビューの中なら受け付け */
	if (theListView->Bounds().Contains(theDropPoint))
		this->RefsReceived(message);
	
	return;
}

void
DocumentsView::RefsReceived (BMessage* message)
{
	status_t		sts;
	entry_ref		aRef;
	node_ref		aNodeRef;
	BEntry			anEntry;
	int32			theIndex    = 0;
	Categorizer*	categorizer = Categorizer::GetInstance();
	
	/* 全てのファイルシステムエントリに対してカテゴリを設定	*/
	while (message->FindRef(kRefsArg, theIndex++, &aRef) == B_OK) {
		/* エントリの有効性を確認 */
		if (anEntry.SetTo(&aRef) != B_OK
				|| anEntry.GetNodeRef(&aNodeRef) != B_OK)
			continue;  /* 無効なエントリ */
		/* カテゴリ設定を試行 */		
		sts = categorizer->AttachCategory(&aRef, fCategory);
		if (sts == B_CANCELED)
			break;  /* ユーザがキャンセルした */
		else if (sts == B_UNSUPPORTED) {
			KGAlert(kUnsupportedVolMsg);
			break;  /* インデックス付けできないボリューム所属 */
		} else if (sts != B_OK)
			goto err;
	}
	
	return;
err:
	::Error("DocumentsView::RefsReceived", sts);
	return;
/*
 * 注意：カテゴリ(キーワード)を設定したエントリをリストビューに登録して
 *		いないのを不思議に思うかも知れないが、その必要はない。ライブク
 *		エリー機構により、そのエントリ情報がB_QUERY_UPDATEメッセージを
 *		使って通知されるからである。
 *		('98. 5/1, koga@ftgun.co.jp)
 */
}

void
DocumentsView::AddDocument (void)
{
	BMessenger	theTarget(this);

	/* オープンパネルがなければ生成 */
	if (! sOpenPanel) {
		sOpenPanel = new BFilePanel(
			B_OPEN_PANEL, NULL, NULL, B_ANY_NODE
		);
	}
	
	/* xx */
	sOpenPanel->SetTarget(theTarget);
	
	/* オープンパネルを表示 */
	sOpenPanel->Show();
	
	return;
}

void
DocumentsView::RemoveSelected (void)
{
	status_t		sts;
	int32			theIndex, baseIndex = 0;
	EntryListView*	theListView = this->GetEntryListView();
	Categorizer*	categorizer = Categorizer::GetInstance();
	BEntry			anEntry;
	node_ref		aNodeRef;

	/* 選択されている全てのドキュメントについて繰り返し */
	while ((theIndex = theListView->CurrentSelection(baseIndex)) >= 0) {
		EntryListItem*		anItem
			= (EntryListItem*)theListView->ItemAt(theIndex);
		const entry_ref*	aRefP = anItem->EntryRef();
		
		/* エントリの有効性を確認 */
		if (anEntry.SetTo(aRefP) != B_OK
				|| anEntry.GetNodeRef(&aNodeRef) != B_OK) {
			::beep();
			baseIndex += 1;
			continue;  /* 無効なエントリ */
		}
		
		/* カテゴリ設定を解除 */
		sts = categorizer->DetachCategory(aRefP);
		if (sts != B_OK)
			goto err;
	
		/* 一覧表示リストから削除 */
		theListView->RemoveItem(theIndex);
		delete anItem;
	}
	
	return;
err:
	::Error("DocumentsView::RemoveSelected", sts);
	return;
/*
 * 注意：RefsReceived()と同様に、このメソッドでもリストビューにアクセス
 *		して項目を削除する必要は本来ない。カテゴリ(キーワード)属性を削
 *		除すれば、それによってライブクエリーの通知メッセージが届けられ
 *		るからである。しかし、ここではリストの全選択要素について繰り返
 *		し処理を行っており、そのアルゴリズム上、自分で削除しないと繰り
 *		返しが正しく行われない。なお、リストビュー(EntryListView)側では
 *		項目の削除を要求された場合、その項目が存在するかどうかのチェッ
 *		クを行っており、ここで削除した項目に関する削除通知メッセージを
 *		受け取っても正しく処理される。
 *
 *		実は、項目の追加を行うEntryListView::CreateItem()でも「重複呼び
 *		出し」のチェックを行っている。そのチェックによる実行効率が問題
 *		になるようであれば、繰り返しアルゴリズムを改善して自分で削除し
 *		なくても良い実装にすること。また、項目の追加処理においても、不
 *		要なチェック処理を削除してしまって構わない。
 *		('98. 5/1, koga@ftgun.co.jp)
 */
}

void
DocumentsView::ChangeSortKey (BMessage* message)
{
	BListView*	theListView = this->GetEntryListView();

	switch (message->what) {
	case SORT_BY_NAME:
		theListView->SortItems(CompareByName);			break;
	case SORT_BY_MODIFIED:
		theListView->SortItems(CompareByModifiedTime);	break;
	case SORT_BY_SIZE:
		theListView->SortItems(CompareBySize);			break;
	}
	
	return;
}

/*
 * ドキュメント管理; DocumentsView
 */
void
DocumentsView::StartSearch (void)
{
	BList	targetVols;
	
	/* 検索対象ボリュームを取得 */
	(void)GetQueryAwareVolumes(&targetVols);
	
	/* 検索処理を開始 */
	RetrieverView::StartSearch(targetVols, true, B_ANY_NODE);
	
	return;
}

void
DocumentsView::MakeupQuery (BQuery* outQuery)
{
	status_t		sts;
	Categorizer*	categorizer = Categorizer::GetInstance();
	
	sts = categorizer->MakeQueryFor(fCategory, outQuery);
	if (sts != B_OK)
		goto err;
	
	return;
err:
	::Error("DocumentsView::MakeupQuery", sts);
	return;
}

/*
 * サブビュー管理; DocumentsView
 */
void
DocumentsView::AllAttached (void)
{
	/* サブビューをレイアウト */
	this->AdjustContent();
	
	/* 検索処理を開始 */
	this->StartSearch();
	
	return;
}

void
DocumentsView::DetachedFromWindow (void)
{
	/* 検索処理を強制終了 */
	this->AbortSearch();
	return;
}

void
DocumentsView::MakeContent (void)
{  /* do nothing */  }

void
DocumentsView::AdjustContent (void)
{
	ASSERT(this->CountChildren() == 0);
	
	EntryListView*	theListView;
	BScrollView*	theScrollView;
	BRect			theBounds = this->Bounds();
	BRect			theFrame;
	
	/* 一覧表示ビューとスクロールビューを生成 */
	theBounds.right  -= B_V_SCROLL_BAR_WIDTH;
	theBounds.bottom -= B_H_SCROLL_BAR_HEIGHT;
	theListView = new EntryListView(
		theBounds,
		kListView,
		B_MULTIPLE_SELECTION_LIST,
		B_FOLLOW_ALL_SIDES
	);
	theScrollView = new BScrollView(
		kScroller,
		theListView,
		B_FOLLOW_ALL_SIDES,
		0,
		true, true, B_NO_BORDER	
	);

	/* 自身に登録 */
	this->AddChild(theScrollView);
	this->SetEntryListView(theListView);
	
	return;
/*
 * 注意：B_DOCUMENT_WINDOWタイプのウィンドウに対し、枠一杯の大きさのス
 *		クロールビューを表示する場合、それ以外の場合とは若干違う手順が
 *		必要なようである。
 *		　上のようにして、スクロールビューのターゲットにするビューのサ
 *		イズを、ウィンドウのバウンズ矩型サイズから各スクロールバーのサ
 *		イズを引いて計算したものに設定すると素直にサイズ調節される。し
 *		かし、他のサンプルでやっているように、いったんウィンドウに貼り
 *		つけた後でリサイズしようとすると、微妙にずれてしまう。そのた
 *		め、ウィンドウに貼りつける前にターゲットビューのサイズ調節を行
 *		うようにした。('98. 5/4, koga@ftgun.co.jp)
 */
}

/*
 * ドキュメントのソート用比較関数 [static]
 */
int
DocumentsView::CompareByName (const void* firstArg, const void* secondArg)
{
	EntryListItem*	itemA = *((EntryListItem**)firstArg);
	EntryListItem*	itemB = *((EntryListItem**)secondArg);

	return strcmp(itemA->EntryRef()->name, itemB->EntryRef()->name);
}

int
DocumentsView::CompareByModifiedTime (
	const void* firstArg, const void* secondArg)
{
	int				retVal = 0;
	EntryListItem*	itemA  = *((EntryListItem**)firstArg);
	EntryListItem*	itemB  = *((EntryListItem**)secondArg);
	BEntry			entryA(itemA->EntryRef());
	BEntry			entryB(itemB->EntryRef());
	time_t			mTimeA, mTimeB;
	
	(void)entryA.GetModificationTime(&mTimeA);
	(void)entryB.GetModificationTime(&mTimeB);
	if (mTimeA < mTimeB)
		retVal = -1;
	else if (mTimeA > mTimeB)
		retVal = 1;
	
	return retVal;
}

int
DocumentsView::CompareBySize (const void* firstArg, const void* secondArg)
{
	int				retVal = 0;
	EntryListItem*	itemA  = *((EntryListItem**)firstArg);
	EntryListItem*	itemB  = *((EntryListItem**)secondArg);
	BEntry			entryA(itemA->EntryRef());
	BEntry			entryB(itemB->EntryRef());
	off_t			sizeA, sizeB;
	
	(void)entryA.GetSize(&sizeA);
	(void)entryB.GetSize(&sizeB);
	if (sizeA < sizeB)
		retVal = -1;
	else if (sizeA > sizeB)
		retVal = 1;
	
	return retVal;
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/* DocumentsViewクラスの公開メソッド */
/*
 * 初期化と解放; DocumentsView
 */
DocumentsView::DocumentsView (
		BRect frame, uint32 resizeMask, const char* inCategory)
	: RetrieverView(frame, "DocumentsView", resizeMask, 0)
{
	ASSERT(inCategory != NULL);

	fCategory = new char[strlen(inCategory) + 1];
	strcpy(fCategory, inCategory);
//	this->MakeContent();  /* '98. 5/4 (koga@ftgun.co.jp) */
}

DocumentsView::~DocumentsView (void)
{
	delete [] fCategory;
}

/*
 * メッセージ処理; DocumentsView
 */
void
DocumentsView::MessageReceived (BMessage* message)
{
	/* ドラッグ＆ドロップ対応 */
	if (message->WasDropped()) {
		this->MessageDropped(message);
		return;
	}

	switch (message->what) {
	case ADD_DOCUMENT:
		this->AddDocument();			break;
	case RMV_DOCUMENT:
		this->RemoveSelected();			break;
	case SORT_BY_NAME:
	case SORT_BY_MODIFIED:
	case SORT_BY_SIZE:
		this->ChangeSortKey(message);	break;
	case B_REFS_RECEIVED:
		this->RefsReceived(message);	break;
	default:
		RetrieverView::MessageReceived(message);
	}
	
	return;
}


/*
 * End of File
 */
