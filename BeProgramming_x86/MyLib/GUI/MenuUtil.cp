/************************************************************************
 * $Id$
 * Copyright (c) 1998 Fort Gunnison, Inc.
 *
 * MenuUtil.cp: MenuUtilクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 28, 1998
 * Last update:	May. 04, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "MenuUtil.h"
#include "KGUtility/kgDebug.h"  /* デバッグ用 */


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/* MenuUtilクラスの公開メソッド */
/*
 * メニュー実体の生成; MenuUtil
 */
BMenuBar*
MenuUtil::MakeMenuBar (const MInfo inMInfos[])
{
	BMenuBar*	theMenuBar;
	
	/* メニューバーを生成 */
	theMenuBar = new BMenuBar(BRect(0, 0, 0, 0), B_EMPTY_STRING);
	
	/* 記述情報に従ってメニューを生成 */
	for (const MInfo* anInfo = inMInfos;
			anInfo->label != NULL; ++anInfo) {
		BMenu*	aMenu = MakeMenu(anInfo->label, anInfo->items);
		
		theMenuBar->AddItem(aMenu);
	}
	
	return theMenuBar;
}

BMenu*
MenuUtil::MakeMenu (const char* inLabel, const MItemInfo inMItemInfos[])
{
	/* メニューを生成 */
	BMenu*	theMenu = new BMenu(inLabel);
	
	/* 記述情報に従ってメニュー項目を生成 */
	for (const MItemInfo* anInfo = inMItemInfos;
			anInfo->label != NULL; ++anInfo) {
		BMenuItem*	aMenuItem;
		
		if (anInfo->label == B_EMPTY_STRING)
			theMenu->AddSeparatorItem();
		else {
			aMenuItem = new BMenuItem(
				anInfo->label,
				new BMessage(anInfo->command),
				anInfo->shortcut
			);
			theMenu->AddItem(aMenuItem);
		}
	}
	
	return theMenu;
}

/*
 * メニュー項目の操作; MenuUtil
 */
void
MenuUtil::SetEnabledOf (BMenu* inMenu, uint32 theCommand, bool theFlag)
{
	BMenuItem*	theMenuItem = inMenu->FindItem(theCommand);
	
	theMenuItem->SetEnabled(theFlag);
	
	return;
}

void
MenuUtil::SetTargetOf (BMenu* inMenu, uint32 theCommand,
	BHandler* inTarget, BLooper* inLooper)
{
	BMenuItem*	theMenuItem = inMenu->FindItem(theCommand);
	
	theMenuItem->SetTarget(inTarget, inLooper);
	
	return;
}

void
MenuUtil::SetTargetOf (BMenu* inMenu, const char* theLabel,
	BHandler* inTarget, BLooper* inLooper)
{
	BMenuItem*	theMenuItem = inMenu->FindItem(theLabel);
	
	theMenuItem->SetTarget(inTarget, inLooper);
	
	return;
}

void
MenuUtil::SetTargetOf (
	BMenu* inMenu, uint32 theCommand, const BMessenger& inTarget)
{
	BMenuItem*	theMenuItem = inMenu->FindItem(theCommand);
	
	theMenuItem->SetTarget(inTarget);
	
	return;
}


/*
 * End of File
 */
