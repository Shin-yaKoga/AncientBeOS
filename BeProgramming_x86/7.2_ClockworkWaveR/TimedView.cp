/************************************************************************
 * $Id$
 * Copyright (c) 1997-1998 Fort Gunnison, Inc.
 *
 * TimedView.cp: TimedViewクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 22, 1997
 * Last update:	Mar. 04, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "TimedView.h"
#include "TimeKeeper.h"


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
 * TimedViewクラスの非公開メソッド
 */
status_t
TimedView::AddSchedule (const ScheduleInfo& inSchedule)
{
	return fTimeKeeper->InsertSchedule(inSchedule);
}

status_t
TimedView::RmvSchedule (uint32 theCode)
{
	return fTimeKeeper->RemoveSchedule(this, theCode);
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * TimedViewクラスの公開メソッド
 */
TimedView::TimedView (BRect frame, const char* title,
		uint32 resizeMask, uint32 flags, TimeKeeper* inTimeKeeper)
	: BView(frame, title, resizeMask, flags)
{
	fTimeKeeper = inTimeKeeper;
}

TimedView::~TimedView (void)
{  /* do nothing */  }


/*
 * End of File
 */
