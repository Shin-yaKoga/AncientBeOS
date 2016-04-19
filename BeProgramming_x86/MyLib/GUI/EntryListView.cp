/************************************************************************
 * $Id$
 * Copyright (c) 1998 Fort Gunnison, Inc.
 *
 * EntryListView.cp: EntryListViewクラスの実装(改訂版)
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 30, 1998
 * Last update:	May. 06, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "EntryListView.h"
#include "EntryListItem.h"
#include "KGUtility/kgAlert.h"
#include "KGUtility/kgDebug.h"

#include <interface/ScrollBar.h>
#include <interface/Window.h>

#include <stdio.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/* 文字列定数 */
const char	kRefsArg[]		= "refs";
const char	kTrackerSig[]	= "application/x-vnd.Be-TRAK";


/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/* EntryListViewクラスの非公開メソッド */
/*
 * メッセージ処理; EntryListView
 */
status_t
EntryListView::Invoke (BMessage* message)
{
	int32		theCount;
	
	this->Window()->CurrentMessage()->FindInt32("clicks", &theCount);
	if (theCount == 2)
		this->OpenEntryOrParent();

	return BListView::Invoke(message);
}

bool
EntryListView::InitiateDrag (
	BPoint /* point */, int32 index, bool wasSelected)
{
	if (fCanDrag && wasSelected) {
		BMessage		theMessage(B_SIMPLE_DATA);
		BRect			theRect = this->ItemFrame(index);
		EntryListItem*	theItem = (EntryListItem*)this->ItemAt(index);
		
		theMessage.AddRef(kRefsArg, theItem->EntryRef());
		this->DragMessage(&theMessage, theRect);
	}
	
	return (fCanDrag && wasSelected);
}

void
EntryListView::FrameResized (float newWidth, float newHeight)
{
	/* 親クラスのメソッドを実行 */
	BListView::FrameResized(newWidth, newHeight);
	
	/* 水平方向のスクロールバーがあれば調節 */
	this->UpdateScrollbars();
	
	return;
}

void
EntryListView::OpenEntryOrParent (void)
{
	status_t	sts;
	key_info	theKeyInfo;
	
	sts = ::get_key_info(&theKeyInfo);
	if (sts != B_OK)
		goto err;
	if (theKeyInfo.modifiers & B_OPTION_KEY)
		this->OpenParentForSelected();
	else
		this->OpenSelected();
	
	return;
err:
	::Error("EntryListView::OpenEntryOrParent", sts);
	return;
}

void
EntryListView::OpenSelected (void)
{
	status_t	sts;
	int32		theIndex, i = 0;
	BMessage	theMessage(B_REFS_RECEIVED);
	BMessenger	tracker(kTrackerSig);
	node_ref	aNodeRef;
	BEntry		anEntry;

	/* 選択されている全要素について繰り返し */
	while ((theIndex = this->CurrentSelection(i++)) >= 0) {
		EntryListItem*		anItem = (EntryListItem*)this->ItemAt(theIndex);
		const entry_ref*	aRefP = anItem->EntryRef();

		/* エントリの有効性を確認 */
		if (anEntry.SetTo(aRefP) != B_OK
				|| anEntry.GetNodeRef(&aNodeRef) != B_OK) {
			this->AlertForTrash(aRefP->name);
			continue;
		}
		
		/* 送信メッセージに追加 */		
		sts = theMessage.AddRef(kRefsArg, anItem->EntryRef());
		if (sts != B_OK)
			goto err;
	}

	/* Trackerにファイルオープンを依頼 */
	sts = tracker.SendMessage(&theMessage);
	if (sts != B_OK)
		goto err;
	
	return;
err:
	::Error("EntryListView::OpenSelected", sts);
	return;
/*
 * 注意：ここでは、エントリが無効であることが分かった場合、単純に「Trash
 *		に移動されたかも知れない」という警告を発しているが、それでは不十
 *		分である。Trashに限らず、ファイルやフォルダが別のディレクトリに
 *		移動されたり、またファイルやフォルダの名前を変更された場合にも、
 *		エントリは無効になるからである。
 *		　この問題を本質的に解決するには、それぞれのエントリが指すノード
 *		に対してwatch_node()による監視を行い、エントリとの関係が変わった
 *		ことを検出したらエントリ情報を更新するようにしなければならない。
 *		('98. 4/30, koga@ftgun.co.jp)
 */
}

void
EntryListView::OpenParentForSelected (void)
{
	status_t	sts;
	int32		theIndex, i = 0;
	BEntry		theEntry, theParentEntry;
	entry_ref	theParentRef;
	node_ref	theNodeRef;
	BMessage	theMessage(B_REFS_RECEIVED);
	BMessenger	tracker(kTrackerSig);

	/* 選択されている全要素について繰り返し */
	while ((theIndex = this->CurrentSelection(i++)) >= 0) {
		EntryListItem*		anItem = (EntryListItem*)this->ItemAt(theIndex);
		const entry_ref*	aRefP = anItem->EntryRef();
		
		/* エントリの有効性を確認 */
		if (theEntry.SetTo(aRefP) != B_OK
				|| theEntry.GetNodeRef(&theNodeRef) != B_OK) {
			this->AlertForTrash(aRefP->name);
			continue;
		}
	
		/* 親フォルダを取得して送信メッセージに追加 */
		sts = theEntry.GetParent(&theParentEntry);
		if (sts != B_OK)
			goto err;
		sts = theParentEntry.GetRef(&theParentRef);
		if (sts != B_OK)
			goto err;	
		sts = theMessage.AddRef(kRefsArg, &theParentRef);
		if (sts != B_OK)
			goto err;
	}
	
	/* Trackerにフォルダオープンを依頼 */
	sts = tracker.SendMessage(&theMessage);
	if (sts != B_OK)
		goto err;

	return;
err:
	::Error("EntryListView::OpenParentForSelected", sts);
	return;
/*
 * 注意：ここでは、エントリが無効であることが分かった場合、単純に「Trash
 *		に移動されたかも知れない」という警告を発しているが、それでは不十
 *		分である。Trashに限らず、ファイルやフォルダが別のディレクトリに
 *		移動されたり、またファイルやフォルダの名前を変更された場合にも、
 *		エントリは無効になるからである。
 *		　この問題を本質的に解決するには、それぞれのエントリが指すノード
 *		に対してwatch_node()による監視を行い、エントリとの関係が変わった
 *		ことを検出したらエントリ情報を更新するようにしなければならない。
 *		('98. 4/30, koga@ftgun.co.jp)
 */
}

/*
 * スクロールバーの調節
 */
void
EntryListView::AttachedToWindow (void)
{
	/* 親クラスのメソッドを実行 */
	BListView::AttachedToWindow();

	/* スクロールバーの表示を調節 */
	this->UpdateScrollbars();
	
	return;
}

void
EntryListView::UpdateScrollbars (void)
{
	BScrollBar*	theHorizBar = this->ScrollBar(B_HORIZONTAL);

	if (theHorizBar != NULL) {
		float	scrollWidth;
		float	realWidth = EntryListItem::PreferredWidth();
		
		scrollWidth = realWidth - this->Bounds().Width();
		if (scrollWidth < 0)
			scrollWidth = 0;
		theHorizBar->SetRange(0, scrollWidth);
		theHorizBar->SetProportion((realWidth - scrollWidth) / realWidth);
	}
}

/*
 * 警告通知; EntryListView
 */
void
EntryListView::AlertForTrash (const char* inName)
{
	char	strBuf[1024];
	
	sprintf(strBuf,
		"Cannot handle '%s'. It may be in the trash.", inName);
	::KGAlert(strBuf);
	
	return;
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/* EntryListViewクラスの公開メソッド */
/*
 * 初期化と解放; EntryListView
 */
EntryListView::EntryListView (BRect frame, const char* title,
		list_view_type type, uint32 resizeMask, uint32 flags)
	: BListView(frame, title, type, resizeMask, flags)
{
	fCanDrag = false;
}

EntryListView::~EntryListView (void)
{  /* do nothing */  }

/*
 * リスト表示要素の管理; EntryListView
 */
int32
EntryListView::IndexOf (const entry_ref& inEntryRef)
{
	int32					theIndex = -1;
	const EntryListItem**	items = (EntryListItem**)this->Items();

	/* リスト要素を検索 */
	for (int32 i = 0, n = this->CountItems(); i < n; ++i) {
		const entry_ref*	aRef = items[i]->EntryRef();
		
		if (*aRef == inEntryRef) {
			theIndex = i;
			break;
		}
	}
	
	return theIndex;
}

int32
EntryListView::IndexOf (const node_ref& inNodeRef)
{
	int32					theIndex = -1;
	const EntryListItem**	items = (EntryListItem**)this->Items();
	
	/* リスト要素を検索 */
	for (int32 i = 0, n = this->CountItems(); i < n; ++i) {
		const node_ref*	aNodeRefP = items[i]->NodeRef();
	
		if (*aNodeRefP == inNodeRef) {
			theIndex = i;
			break;
		}
	}
	
	return theIndex;
}

bool
EntryListView::CreateItem (const entry_ref& inEntryRef)
{
	bool			isCreated;

	/* 既に対応するリスト要素がないかチェック */
	if (this->IndexOf(inEntryRef) >= 0)
		return false;
	
	/* 新しいリスト要素を生成して追加 */
	isCreated = this->AddItem(new EntryListItem(inEntryRef));

	return isCreated;
}

bool
EntryListView::DeleteItem (const entry_ref& inEntryRef)
{
	bool	isRemoved = false;
	int32	theIndex;

	/* 対応するリスト要素を検索 */
	theIndex = this->IndexOf(inEntryRef);

	/* 見つかったらリストから削除して解放 */
	if (theIndex >= 0) {
		EntryListItem*	theItem;
		
		theItem = (EntryListItem*)this->RemoveItem(theIndex);
		delete theItem;
		isRemoved = true;
	}
	
	return isRemoved;
}

bool
EntryListView::DeleteItem (const node_ref& inNodeRef)
{
	bool	isRemoved = false;
	int32	theIndex;
	
	/* 対応するリスト要素を検索 */
	theIndex = this->IndexOf(inNodeRef);
	
	/* 見つかったらリストから削除して解放 */
	if (theIndex >= 0) {
		EntryListItem*	theItem;
		
		theItem = (EntryListItem*)this->RemoveItem(theIndex);
		delete theItem;
		isRemoved = true;
	}
	
	return isRemoved;
}

void
EntryListView::DeleteItemsOf (dev_t theDevice)
{
	for (int32 i = 0, n = this->CountItems(); i < n; ++i) {
		EntryListItem*	anItem = (EntryListItem*)this->ItemAt(i);
		
		if (anItem->EntryRef()->device == theDevice) {
			(void)this->RemoveItem(i--);
			delete anItem;
			n -= 1;
		}
	}
	
	return;
}

void
EntryListView::DeleteAll (void)
{
	/* 全要素を削除して解放 */
	for (int32 i = 0, n = this->CountItems(); i < n; ++i) {
		EntryListItem*	anItem;
		
		anItem = (EntryListItem*)this->RemoveItem((int32)0);
		delete anItem;
	}
	
	return;
}

bool
EntryListView::ItemMoved (
	const node_ref& inNodeRef, const entry_ref& inNewEntryRef)
{
	bool	isMoved = false;
	int32	theIndex;
	
	/* 対応するリスト要素を検索 */
	theIndex = this->IndexOf(inNodeRef);
	
	/* 見つかったらエントリ情報を更新 */
	if (theIndex >= 0) {
		EntryListItem*	theItem;
		
		theItem = (EntryListItem*)this->RemoveItem(theIndex);
		theItem->SetEntryRef(inNewEntryRef);
		this->AddItem(theItem, theIndex);
		isMoved = true;
	}
	
	return isMoved;
/*
 * 注意：エントリの表示を更新するために、一旦リストから撮り外して追加し
 *		直している。こうするのではなく、SetEntryRef()でエントリ情報を更
 *		新してBListView::InvalidateItem()を呼び出した場合、
 *		EntryListItem::Update()が呼び出されないので、ファイル名表示の切
 *		り詰め処理が行われない。('98. 4/30, koga@ftgun.co.jp)
 */
}

/*
 * ドラッグ＆ドロップ;
 */
void
EntryListView::SetDragEnabled (bool allowDrag)
{
	fCanDrag = allowDrag;
	return;
}

/*
 * メッセージ処理; EntryListView
 */
void
EntryListView::MessageReceived (BMessage* message)
{
	switch (message->what) {
	case OPEN_DOCUMENT:
		this->OpenSelected();			break;
	case OPEN_PARENT:
		this->OpenParentForSelected();	break;
	default:
		BListView::MessageReceived(message);
	}
	
	return;
}


/*
 * End of File
 */
