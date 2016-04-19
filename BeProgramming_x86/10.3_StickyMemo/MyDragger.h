/***********************************************************************
 * $Id$
 * Copyright (c) 1998 by Fort Gunnison, Inc.
 *
 * MyDragger.h: 背景色の描画機能を持つDragger
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		May. 04, 1998
 * Last update:	May. 04, 1998
 ************************************************************************
 */

#ifndef _MY_DRAGGER_
#define _MY_DRAGGER_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include <interface/Dragger.h>

#ifndef _PR2_COMPATIBLE_
#define _PR2_COMPATIBLE_ 1
#endif

#if _PR2_COMPATIBLE_
#else
class __declspec(dllexport) MyDragger;
#endif


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * MyDraggerクラスの定義
 */
class MyDragger : public BDragger {
public:
	MyDragger(BRect bounds, BView* target,
		uint32 rmask, uint32 flags = B_WILL_DRAW);
	MyDragger(BMessage* data);
	~MyDragger(void);

#if _PR2_COMPATIBLE_	
#pragma export on
#endif
	static BArchivable*	Instantiate(BMessage* data);
#if _PR2_COMPATIBLE_
#pragma export reset
#endif

private:
	void	Draw(BRect updateRect);
};


#endif  /* _MY_DRAGGER_ */