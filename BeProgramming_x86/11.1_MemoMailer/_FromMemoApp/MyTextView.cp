/************************************************************************
 * $Id$
 * Copyright (c) 1998 Fort Gunnison, Inc.
 *
 * MyTextView.cp: xxx
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		May. 04, 1998
 * Last update:	May. 04, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "MyTextView.h"


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

void
MyTextView::FrameResized (float newWidth, float newHeight)
{
	BTextView::FrameResized(newWidth, newHeight);
	if (fDoLineAdjust && this->DoesWordWrap()) {
		BRect	textRect = this->Bounds();
		
		textRect.OffsetTo(B_ORIGIN);
		textRect.InsetBy(3.0, 3.0);
		this->SetTextRect(textRect);
	}
	
	return;
}

void
MyTextView::InsertText (const char* text, int32 length,
	int32 offset, const text_run_array* runs)
{
	fIsDirty = true;
	BTextView::InsertText(text, length, offset, runs);
	
	return;
}

void
MyTextView::DeleteText (int32 start, int32 finish)
{
	fIsDirty = true;
	BTextView::DeleteText(start, finish);
	
	return;
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

MyTextView::MyTextView (BRect frame, const char* name, BRect textRect,
		uint32 resizingMask, uint32 flags)
	: BTextView(frame, name, textRect, resizingMask, flags)
{
	fIsDirty = false;
	fDoLineAdjust = false;
}

MyTextView::MyTextView (BMessage* message) : BTextView(message)
{
	fIsDirty = false;
	fDoLineAdjust = false;
}

MyTextView::~MyTextView (void)
{  /* do nothing */  }

bool
MyTextView::IsDirty (void) const
{
	return fIsDirty;
}

void
MyTextView::MakeClean (void)
{
	fIsDirty = false;
	return;
}

void
MyTextView::SetLineAdjust (bool doit)
{
	fDoLineAdjust = doit;
	if (fDoLineAdjust) {
		BRect	myBounds = this->Bounds();
		this->FrameResized(myBounds.Width(), myBounds.Height());
	}
	
	return;
}


/*
 * End of File
 */
