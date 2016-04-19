/************************************************************************
 * $Id$
 * Copyright (c) 1998 Fort Gunnison, Inc.
 *
 * EntryListItem.cp: EntryListItemクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 30, 1998
 * Last update:	May. 01, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "EntryListItem.h"
#include "KGUtility/kgDebug.h"

#include <interface/Bitmap.h>
#include <storage/NodeInfo.h>

#include <algobase.h>  /* for max() */
#include <string.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/* 各種定数 */
const float	kNameBoxWidth = 120;	/* ファイル名の表示幅 */
const float	kIconBoxWidth = 16;		/* アイコンの表示幅 */


/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/*
 * EntryListItemクラスの非公開メソッド
 */
void
EntryListItem::DrawItem (BView* owner, BRect frame, bool /* complete */)
{
	float			baseLine;
	font_height		theFontHeight;
	rgb_color		orgColor = owner->HighColor();
	BRect			destRect = frame;
	drawing_mode	orgMode;
	char			strBuf[63+1];
	
	/* 背景を描画 */
	if (this->IsSelected())
		owner->SetHighColor(0xD0, 0xD0, 0xD0);
	else
		owner->SetHighColor(255, 255, 255);
	owner->FillRect(frame);
	owner->SetHighColor(orgColor);
	
	/* アイコンを描画 */
	destRect.left   += 1;
	destRect.top    += 1;
	destRect.bottom = destRect.top  + 15;
	destRect.right  = destRect.left + destRect.Height();
	orgMode = owner->DrawingMode();
	owner->SetDrawingMode(B_OP_OVER);
	owner->DrawBitmap(fIcon, destRect);
	owner->SetDrawingMode(orgMode);
	
	/* テキスト描画用のベースラインを計算 */
	owner->GetFontHeight(&theFontHeight);
	baseLine = theFontHeight.leading + theFontHeight.descent;

	/* ファイル名を描画 */
	owner->MovePenTo(
		frame.left + kIconBoxWidth + 5, frame.bottom - baseLine
	);
	owner->DrawString(fNameStr);
	
	/* 最終更新日時を描画 */
	owner->MovePenTo(
		frame.left + kIconBoxWidth + kNameBoxWidth + 10,
		frame.bottom - baseLine
	);
	strncpy(strBuf, ctime(&fModTime), sizeof(strBuf));
	strBuf[sizeof(strBuf) - 1] = '\0';  /* 念のため */
	strBuf[strlen(strBuf) - 1] = '\0';  /* 改行文字の削除 */
	owner->DrawString(strBuf);
	
	return;
}

void
EntryListItem::Update (BView* owner, const BFont* font)
{
	font_height	theFontHeight;
	float		textHeight;
	
	/* 親クラスのメソッドを実行 */
	BListItem::Update(owner, font);
	
	/* 自身の高さを調節 */
	font->GetHeight(&theFontHeight);
	textHeight = theFontHeight.ascent
		+ theFontHeight.descent + theFontHeight.leading;
	this->SetHeight(max(kIconBoxWidth + 1, textHeight));
	
	/* 表示用のファイル文字列を再計算 */
	delete [] fNameStr;
	fNameStr = new char [strlen(fEntryRef.name) + 3];
	font->GetTruncatedStrings(
		&fEntryRef.name, 1, B_TRUNCATE_END, kNameBoxWidth, &fNameStr
	);
	
	return;
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/* EntryListItemクラスの公開メソッド */
/*
 * 初期化と解放; EntryListItem
 */
EntryListItem::EntryListItem (
		const entry_ref& inEntryRef, uint32 outlineLevel, bool expanded)
	: BListItem(outlineLevel, expanded)
{
	fNameStr  = NULL;
	fIcon     = NULL;
	this->SetEntryRef(inEntryRef);
}

EntryListItem::~EntryListItem (void)
{
	delete [] fNameStr;
	delete fIcon;
}

/*
 * 表示ファイルの設定と取得; EntryListItem
 */
void
EntryListItem::SetEntryRef (const entry_ref& inEntryRef)
{
	status_t	sts;
	BEntry		theEntry;

	/* エントリ情報を初期化 */
	fEntryRef = inEntryRef;
	sts = theEntry.SetTo(&fEntryRef);
	if (sts != B_OK)
		goto err;
	sts = theEntry.GetNodeRef(&fNodeRef);
	if (sts != B_OK)
		goto err;
	sts = theEntry.GetModificationTime(&fModTime);
	if (sts != B_OK)
		goto err;
	
	/* アイコンを取得 */
	delete fIcon;
	fIcon = new BBitmap(BRect(0, 0, 15, 15), B_COLOR_8_BIT);
	(void)BNodeInfo::GetTrackerIcon(
		(entry_ref*)&inEntryRef, fIcon, B_MINI_ICON
	);

	return;
err:
	::Error("EntryListItem::SetEntryRef", sts);
	return;
/*
 * 注意：本当は、ここでfNameStrも一緒に更新する必要がある。しかし、通常
 *		は一旦表示してしまった後でこのメソッドを呼び出すことはないと前
 *		提しているので、ここでは省略している。
 *		('98. 1/30, koga@ftgun.co.jp)
 */
}

const entry_ref*
EntryListItem::EntryRef (void) const
{
	return &fEntryRef;
}

const node_ref*
EntryListItem::NodeRef (void) const
{
	return &fNodeRef;
}

/*
 * 描画処理; EntryListItem
 */
float
EntryListItem::PreferredWidth (void)
{
	return (kIconBoxWidth + 5 + kNameBoxWidth + 135);
}


/*
 * End of File
 */
