/************************************************************************
 * $Id$
 * Copyright (c) 1997, Fort Gunnison, Inc.
 *
 * SimpleView.cp: SimpleViewクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 11, 1997
 * Last update:	Dec. 11, 1997
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "SimpleView.h"


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
 * SimpleViewクラスの非公開メソッド
 */
void
SimpleView::Draw (BRect /* updateRect */)
{
	rgb_color	currColor = this->HighColor();

	/* 文字列と図形を描画 */
	this->SetHighColor(0xD0, 0xD0, 0xD0);
	this->FillRect(this->Frame());
	this->SetHighColor(currColor);
	this->DrawString("This is the my first app.", BPoint(10, 10));
	this->SetHighColor(255, 0, 0);
	this->FillRect(BRect(110, 30, 140, 60));

	return;
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * SimpleViewクラスの公開メソッド
 */
SimpleView::SimpleView (BRect frame, const char* name,
		uint32 resizeMask, uint32 flags)
	: BView(frame, name, resizeMask, flags)
{  /* do nothing */  }

SimpleView::~SimpleView (void)
{  /* do nothing */  }


/*
 * End of File
 */
