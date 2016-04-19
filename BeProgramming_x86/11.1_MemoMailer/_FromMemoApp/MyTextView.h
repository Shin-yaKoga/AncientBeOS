/***********************************************************************
 * $Id$
 * Copyright (c) 1998 by Fort Gunnison, Inc.
 *
 * MyTextView.h: MY_TEXT_VIEW
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		May. 04, 1998
 * Last update:	May. 04, 1998
 ************************************************************************
 */

#ifndef _MY_TEXT_VIEW_H_
#define _MY_TEXT_VIEW_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include <interface/TextView.h>


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

class MyTextView : public BTextView {
public:
	MyTextView(BRect frame, const char* name, BRect textRect,
		uint32 resizingMode, uint32 flags);
	MyTextView(BMessage* message);
	~MyTextView(void);
	
	bool	IsDirty(void) const;
	void	MakeClean(void);
	void	SetLineAdjust(bool doit);
	
private:
	void	FrameResized(float newWidth, float newHeight);
	void	InsertText(const char* text, int32 length,
		int32 offset, const text_run_array* runs);
	void	DeleteText(int32 start, int32 finish);

private:
	bool	fIsDirty;
	bool	fDoLineAdjust;
};


#endif  /* _MY_TEXT_VIEW_H_ */

/*
 * End of File
 */
