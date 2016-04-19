/************************************************************************
 * $Id$
 * Copyright (c) 1997-1998 Fort Gunnison, Inc.
 *
 * MyScrollView.cp: MyScrollViewクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 16, 1997
 * Last update:	May. 05, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "MyScrollView.h"
#include "MyDragger.h"

#include <interface/Alert.h>
#include <support/Debug.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

const char	kAboutMsg[]	= "MyScrollView(from StickyMemo app)\n\n"
	"Copyright " B_UTF8_COPYRIGHT " 1998 Fort Gunnison, Inc.\n"
	"Author: Shin'ya Koga (koga@ftgun.co.jp)";


/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/* MyScrollViewクラスの非公開メソッド */
void
MyScrollView::MessageReceived (BMessage* message)
{
	switch (message->what) {
	case B_ABOUT_REQUESTED:
		this->AboutRequested();	break;
	default:
		BScrollView::MessageReceived(message);
	}
	
	return;
}

void
MyScrollView::AboutRequested (void)
{
	BAlert*	alertPanel;
	
	alertPanel = new BAlert("about box", kAboutMsg, "OK");
	(void)alertPanel->Go(NULL);

	return;
}

/*
 * マウス応答; MyScrollView
 */
void
MyScrollView::MouseDown (BPoint where)
{
	BPoint		prevPos;
	uint32		buttons;
	int32		theInt;
	thread_id	dragThread;


	/* どのマウスボタンが押されたかチェック */
	this->Window()->CurrentMessage()->FindInt32("buttons", &theInt);
	buttons = theInt;
	if (buttons != B_PRIMARY_MOUSE_BUTTON)
		return;

	/* マウスの追跡スレッドを生成して起動 */
	fStartPos = where;
	dragThread = ::spawn_thread(
		DoDrag, "drag_thread", B_NORMAL_PRIORITY, this
	);
	(void)::resume_thread(dragThread);
	::snooze(20 * 1000);
	
	return;
}

int32
MyScrollView::DoDrag (void* data)
{
	MyScrollView*	theObj = (MyScrollView*)data;
	BWindow*		theWin = theObj->Window();
	BPoint			prevPos, where;
	uint32			buttons;

	/* マウスボタンが離されるまで追跡 */
	prevPos = theObj->fStartPos;
	do {
		::snooze(20 * 1000);
		theWin->Lock();  /* ウィンドウをロック */
		theObj->GetMouse(&where, &buttons, true);
		theObj->ResizeBy(where.x - prevPos.x, where.y - prevPos.y);
		theWin->Unlock();  /* ロックを解除 */
		prevPos = where;
	} while (buttons & B_PRIMARY_MOUSE_BUTTON);
	
	return B_OK;
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/* MyScrollViewクラスの公開メソッド */
/*
 * 初期化と解放; MyScrollView
 */
MyScrollView::MyScrollView (const char* name, BView* target,
		uint32 resizeMask, uint32 flags)
	: BScrollView(name, target, resizeMask, flags, true, true)
{
	BScrollBar*	theScrollBar = this->ScrollBar(B_HORIZONTAL);
	BRect		draggerRect;
	
	ASSERT(theScrollBar != NULL);

	/* 水平方向のスクロールバーを縮めて左端を空ける */
	theScrollBar->ResizeBy(-15, 0);
	theScrollBar->MoveBy(15, 0);
	
	/* 空けたところにドラッガーを貼りつける */
	draggerRect = theScrollBar->Frame();
	draggerRect.right = draggerRect.left;
	draggerRect.left -= 15;
	draggerRect.right  -= 1;
	fDragger = new MyDragger(
		draggerRect, this, B_FOLLOW_LEFT | B_FOLLOW_BOTTOM);
	this->AddChild(fDragger);
}

MyScrollView::MyScrollView (BMessage* message)
	: BScrollView(message)
{  /* do nothing */  }

MyScrollView::~MyScrollView (void)
{  /* do nothing */  }

/*
 * アーカイビング; MyScrollView
 */
BArchivable*
MyScrollView::Instantiate (BMessage* message)
{
	return (new MyScrollView(message));
}

status_t
MyScrollView::Archive (BMessage* data, bool deep) const
{
	/* 親クラスのメソッドを実行 */
	BScrollView::Archive(data, deep);
	
	/* リサイズフラグの値を置き換え */
	data->ReplaceInt32(
		"_resize_mode", B_FOLLOW_LEFT|B_FOLLOW_TOP
	);
	
	/* add-on情報を追加 */
	data->AddString("add_on", "application/x-vnd.FtGUN-StickyMemo");
	
	return B_OK;
}


/*
 * End of File
 */
