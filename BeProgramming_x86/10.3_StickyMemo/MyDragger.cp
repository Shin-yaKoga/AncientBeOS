/************************************************************************
 * $Id$
 * Copyright (c) 1998 Fort Gunnison, Inc.
 *
 * MyDragger.cp: MyDraggerクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		May. 04, 1998
 * Last update:	May. 04, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "MyDragger.h"


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/*
 * MyDraggerクラスの非公開メソッド
 */
void
MyDragger::Draw (BRect updateRect)
{
	/* 背景部分を描画 */
	this->SetHighColor(0xD0, 0xD0, 0xD0);
	this->FillRect(updateRect);
	
	/* 親クラスのメソッドを実行 */
	BDragger::Draw(updateRect);
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * MyDraggerクラスの公開メソッド
 */
MyDragger::MyDragger (
		BRect bounds, BView* target, uint32 rmask, uint32 flags)
	: BDragger(bounds, target, rmask, flags)
{  /* do nothing */  }

MyDragger::MyDragger (BMessage* data) : BDragger(data)
{  /* do nothing */  }

MyDragger::~MyDragger (void)
{  /* do nothing */  }

BArchivable*
MyDragger::Instantiate (BMessage* data)
{
	return (new MyDragger(data));
}


/*
 * End of file
 */
