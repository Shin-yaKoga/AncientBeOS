/************************************************************************
 * $Id$
 * Copyright (c) 1998 Fort Gunnison, Inc.
 *
 * RetrieverView.cp: RetrieverViewクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 29, 1998
 * Last update:	May. 17, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "RetrieverView.h"
#include "SearchEngine.h"
#include "GUI/EntryListView.h"
#include "GUI/MenuUtil.h"
#include "KGUtility/kgDebug.h"

#include <storage/NodeMonitor.h>
#include <storage/VolumeRoster.h>
#include <support/Debug.h>


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
const char	kToDirArg[]		= "to directory";
const char	kNodeArg[]		= "node";
const char	kNameArg[]		= "name";
const char	kNewDeviceArg[]	= "new device";
const char	kRefsArg[]		= "refs";


/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/* RetrieverViewクラスの非公開メソッド */
/*
 * 検索結果の表示動作制御; RetrieverView
 */
void
RetrieverView::SetEntryListView (EntryListView* inListView)
{
	fListView = inListView;
	return;
}

EntryListView*
RetrieverView::GetEntryListView (void)
{
	return fListView;
}

/*
 * 検索動作の制御; RetrieverView
 */
void
RetrieverView::StartSearch (
	const BList& inDevices, bool doLiveQuery, uint32 theFlavor)
{
	BQuery	theQuery;

	/* 過去に行った検索情報を解放 */
	this->AbortSearch();
	fListView->DeleteAll();  /* リスト表示をクリア */

	/* 新しい検索処理を開始 */
	this->MakeupQuery(&theQuery);
	fDoLiveQuery = doLiveQuery;
	fNodeFlavor  = theFlavor;
	for (int32 i = 0, n = inDevices.CountItems(); i < n; ++i) {
		dev_t	theDevice = (dev_t)inDevices.ItemAt(i);
		
		this->StartSearchOn(theDevice, &theQuery);
	}
	
	return;
}

void
RetrieverView::AbortSearch (void)
{
	BMessageQueue*	msgQueue;
	BMessage*		aMessage;

	/* 全ての検索処理を中断して検索オブジェクトを解放 */
	for (int32 i = 0, n = fSearchers.CountItems(); i < n; ++i) {
		SearchEngine*	aSearcher;
		
		aSearcher = (SearchEngine*)fSearchers.RemoveItem((int32)0);
		(void)aSearcher->AbortSearch();
		delete aSearcher;
	}
	
	/* 不要になったメッセージを解放 */
	msgQueue = this->Looper()->MessageQueue();
	while ((aMessage = msgQueue->FindMessage(KGSE_ENTRY_FOUND, 0)) != NULL)
		msgQueue->RemoveMessage(aMessage);
//	if ((aMessage = msgQueue->FindMessage(KGSE_COMPLETED, 0)) != NULL)
	while ((aMessage = msgQueue->FindMessage(KGSE_COMPLETED, 0)) != NULL)
		msgQueue->RemoveMessage(aMessage);
	
	return;
/*
 * 注意：RetrieverViewが所属するウィンドウのスレッドと、SearchEngine
 *		オブジェクト内の検索スレッドは、非同期のメッセージ通信を使って
 *		並行動作している。したがって、SearchEngineを解放して検索スレッ
 *		ドを終了しても、その前に送られてきたメッセージが残っている可能
 *		性がある。上でメッセージキューの「掃除」を行っているのは、それ
 *		に対処するためである。しかし、それだけでは完全でない。
 *		このメソッドが実行されている間、ウィンドウスレッドによるポート
 *		からのメッセージ読み出し処理は停止しているため、その間に送られ
 *		たメッセージはキューに入っていないのである。したがって、不要な
 *		メッセージが届けられてしまい、「新しい」検索条件には該当しない
 *		ファイルやフォルダが表示されてしまう場合がある。
 *
 *		この問題を完全に解決するには、毎回の検索ごとに「トランザクショ
 *		ンID」を発行し、それを使って不要なメッセージを判定するようにし
 *		なければならない。('98. 5/1, koga@ftgun.co.jp)
 */
}

/*
 * 検索オブジェクトの管理; RetrieverView
 */
SearchEngine*
RetrieverView::FindSearcher (dev_t theDevice)
{
	SearchEngine*	theSearcher = NULL;
	
	for (int32 i = 0, n = fSearchers.CountItems(); i < n; ++i) {
		SearchEngine*	aSearcher = (SearchEngine*)fSearchers.ItemAt(i);
		
		if (aSearcher->TargetDevice() == theDevice) {
			theSearcher = aSearcher;
			break;
		}
	}
	
	return theSearcher;
}

/*
 * メッセージ処理; RetrieverView
 */
void
RetrieverView::VolumeMounted (BMessage* message)
{
	dev_t	theDevice;
	BQuery	theQuery;
	
	/* 検索中でなければ何もしない */
	if (fSearchers.CountItems() == 0)
		return;

	/* ボリューム情報を取得 */
	(void)message->FindInt32(kNewDeviceArg, &theDevice);

	/* 検索動作を開始 */
	this->MakeupQuery(&theQuery);
	this->StartSearchOn(theDevice, &theQuery);
	
	return;
/*
 * AbortSearchによって検索を中断しているかどうかの判定をfSearchersの要
 * 素数が0かどうかで行っているが、これは起動ボリュームは必ず検索可能で
 * あるという前提があるからである。('97. 1/29, koga@ftgun.co.jp)
 */
}

void
RetrieverView::VolumeUnmounted (BMessage* message)
{
	dev_t			theDevice;
	SearchEngine*	theSearcher;

	/* ボリューム情報を取得 */
	(void)message->FindInt32(kDeviceArg, &theDevice);

	/* そのボリュームに対する検索オブジェクトを解放 */
	theSearcher = this->FindSearcher(theDevice);
	if (theSearcher != NULL) {
		fSearchers.RemoveItem(theSearcher);
		if (theSearcher->IsSearching())
			(void)theSearcher->AbortSearch();
		delete theSearcher;
	}
	fListView->DeleteItemsOf(theDevice);
	
	return;
/*
 * 注意：検索オブジェクトが存在している間、つまり検索処理を行っている間
 *		は、インデックスファイルを開いているためそのボリュームはbusy状
 *		態である。したがって、このメソッドが呼ばれた時に検索オブジェクト
 *		を解放する必要はないと考えられる。ここで行っているのは、あくまで
 *		も念のための措置である。('98. 5/4, koga@ftgun.co.jp)
 */
}

void
RetrieverView::HandleNodeMessage (BMessage* message)
{
	status_t	sts;
	int32		theOpCode;

	/* オペコードを取得 */
	sts = message->FindInt32(kOpCodeArg, &theOpCode);
	if (sts != B_OK)
		goto err;
	
	/* オペコードに応じて分岐処理 */
	switch (theOpCode) {
	case B_DEVICE_MOUNTED:
		this->VolumeMounted(message);	break;
	case B_DEVICE_UNMOUNTED:
		this->VolumeUnmounted(message);	break;
	case B_ENTRY_MOVED:
		this->EntriesChanged(message);	break;  /* '98. 5/7 */
	default:
		/* 無視 */;
	}
	
	return;
err:
	::Error("RetrieverView::HandleNodeMessage", sts);
	return;
}

void
RetrieverView::SearchCompleted (BMessage* message)
{
	SearchEngine*	theSearcher;
	
	(void)message->FindPointer(kSenderArg, &theSearcher);
	if (! fDoLiveQuery) {
		if (fSearchers.RemoveItem(theSearcher) != NULL)
			delete theSearcher;
	}

	return;
}

void
RetrieverView::EntriesFound (BMessage* message)
{
	entry_ref	aRef;
	int32		theIndex = 0;

	/* 全てのエントリをリスト表示ビューに追加 */
	while (message->FindRef(kRefsArg, theIndex++, &aRef) == B_OK)
		fListView->CreateItem(aRef);
	
	return;
}

void
RetrieverView::EntriesChanged (BMessage* message)
{
	status_t	sts;
	int32		theOpCode;
	entry_ref	theRef;
	node_ref	theNodeRef;
	const char*	theName;
	
	/* オペコードを取得 */
	sts = message->FindInt32(kOpCodeArg, &theOpCode);
	if (sts != B_OK)
		goto err;
	
	/* エントリ情報を取得 */
	(void)message->FindInt32(kDeviceArg, &theRef.device);
	(void)message->FindInt64(kDirectoryArg, &theRef.directory);
	if (theOpCode == B_ENTRY_CREATED) {
		(void)message->FindString(kNameArg, &theName);
		(void)theRef.set_name(theName);
	} else {
		theNodeRef.device = theRef.device;
		(void)message->FindInt64(kNodeArg, &theNodeRef.node);
		
		/* 移動した場合は特別 */
		if (theOpCode == B_ENTRY_MOVED) {
			(void)message->FindInt64(kToDirArg, &theRef.directory);
			(void)message->FindString(kNameArg, &theName);
			(void)theRef.set_name(theName);
		}
	}

	/* オペコードに応じて分岐処理 */
	switch (theOpCode) {
	case B_ENTRY_CREATED:
		fListView->CreateItem(theRef);				break;
	case B_ENTRY_REMOVED:
		fListView->DeleteItem(theNodeRef);			break;
	case B_ENTRY_MOVED:
		fListView->ItemMoved(theNodeRef, theRef);	break;
	default:
		/* 無視 */;
	}
	
	return;
err:
	::Error("RetrieverView::EntriesChanged", sts);
	return;
/*
 * 注意：現在の実装では、B_ENTRY_MOVEDメッセージが送られて来ることは
 *		ない。このメッセージは、NodeMonitorのwatch_node()を使って
 *		個々のノードを監視した場合にのみ送られてくるものだからである。
 *
 *		リストに表示する全てのノードに対する監視を行うように修正する
 *		には、以下の改変が必要になる：
 *		1.)リスト表示にノード(エントリ)を追加する際、それぞれのノード
 *		　を引数としてwatch_node()を呼び出し、監視を開始する。
 *		2.)リスト表示からノード(エントリ)を削除するときは、それぞれの
 *		　ノードに対してstop_watching()を呼び出し、監視を終了する。
 *		なお、watch_node()を呼び出して監視する場合、そのための「ス
 *		ロット」がシステム側に作られる。このスロットは4096個までしか
 *		作ることができないので、必ずstop_watching()を呼び出してスロッ
 *		トが解放されるようにしなければならない。
 *		('98. 4/30, koga@ftgun.co.jp)
 */
}

/*
 * 検索処理の実行; RetrieverView
 */
void
RetrieverView::StartSearchOn (dev_t theDevice, BQuery* theQuery)
{
	status_t		sts;
	SearchInfo		searchInfo;
	SearchEngine*	theSearcher;
	size_t			theLength = theQuery->PredicateLength();
	
	/* 既に同じディレクトリの検索を行っていたら無視 */
	if (this->FindSearcher(theDevice) != NULL)
		return;

	/* 検索指定情報を設定 */
	(void)searchInfo.volume.SetTo(theDevice);
	searchInfo.client = this;
	searchInfo.predicate = new char[theLength];
	sts = theQuery->GetPredicate(searchInfo.predicate, theLength);
	if (sts != B_OK)
		goto err;

	/* 検索処理を開始 */
	theSearcher = new SearchEngine(fNodeFlavor);
	sts = theSearcher->StartSearch(searchInfo, fDoLiveQuery);
	if (sts != B_OK)
		goto err_release;
	fSearchers.AddItem(theSearcher);  /* リストに追加 */
	
	return;
err_release:
	delete theSearcher;
err:
	::Error("RetrieverView::StartSearchOn", sts);
	return;
/*
 * 注意：SearchInfoは、BRectなどと同様構造体的な性格を持つクラスだが、
 *		デストラクタではpredicateメンバを正しく解放するので、呼び出し
 *		側で解放の心配をする必要はない。
 *		('98. 1/29, koga@ftgun.co.jp)
 */
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/* RetrieverViewクラスの公開メソッド */
/*
 * 検索用のユーティリティ; RetrieverView
 */
status_t
RetrieverView::GetQueryAwareVolumes (BList* outDevices)
{
	BVolumeRoster	volRoster;
	BVolume			aVolume;
	
	/* 問い合わせ可能な全てのボリュームを取得 */
	while (volRoster.GetNextVolume(&aVolume) == B_OK) {
		if (aVolume.KnowsQuery())
			outDevices->AddItem((void*)aVolume.Device());
	}
	
	return B_OK;	
}

/*
 * 初期化と解放; RetrieverView
 */
RetrieverView::RetrieverView (BRect frame, const char* title,
		uint32 resizeMask, uint32 flags)
	: BView(frame, title, resizeMask, flags)
{
	fListView = NULL;
}

RetrieverView::~RetrieverView (void)
{
	/* 全ての検索処理を停止 */
	if (fSearchers.CountItems() != 0)
		this->AbortSearch();
}

/*
 * メニュー調節; RetrieverView
 */
void
RetrieverView::AddAdjustableMenuItem (uint32 theCommand)
{
	ASSERT(fListView != NULL);

	fMItemList.AddItem((void*)theCommand);
	return;
}

void
RetrieverView::AdjustMenuItems (BMenuBar* ioMenuBar)
{
	bool	doEnable;

	/* リスト表示ビューの選択状態を取得 */
	doEnable = (fListView->CurrentSelection() >= 0);
	
	/* 設定されている全項目についてセット */
	for (int32 i = 0, n = fMItemList.CountItems(); i < n; ++i) {
		uint32	theCommand = (uint32)fMItemList.ItemAt(i);
	
		MenuUtil::SetEnabledOf(ioMenuBar, theCommand, doEnable);
	}
	
	return;
}

/*
 * xx
 */
void
RetrieverView::SetDragEnabled (bool allowDrag)
{
	ASSERT(fListView != NULL);
	
	fListView->SetDragEnabled(allowDrag);
	return;
}

/*
 * メッセージ処理; RetrieverView
 */
void
RetrieverView::MessageReceived (BMessage* message)
{
	ASSERT(fListView != NULL);

	switch (message->what) {
	case B_NODE_MONITOR:
		this->HandleNodeMessage(message);		break;
	case B_QUERY_UPDATE:
		this->EntriesChanged(message);			break;
	case KGSE_ENTRY_FOUND:
		this->EntriesFound(message);			break;
	case KGSE_COMPLETED:
		this->SearchCompleted(message);			break;
	case OPEN_DOCUMENT:
	case OPEN_PARENT:
		fListView->MessageReceived(message);	break;
	default:
		BView::MessageReceived(message);
	}
	
	return;
}


/*
 * End of File
 */
