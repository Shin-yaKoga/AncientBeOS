/***********************************************************************
 * $Id$
 * Copyright (c) 1997-1998 by Fort Gunnison, Inc.
 *
 * MyScrollView.h: レプリカント化可能なスクロールビュー
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 16, 1997
 * Last update:	May. 04, 1998
 ************************************************************************
 */

#ifndef _MY_SCROLL_VIEW_H_
#define _MY_SCROLL_VIEW_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include <interface/ScrollView.h>

#ifndef _PR2_COMPATIBLE_
#define _PR2_COMPATIBLE_ 1
#endif


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/* 関連クラス・構造体 */
class	BDragger;


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

#if _PR2_COMPATIBLE_
#else
class __declspec(dllexport) MyScrollView;
#endif

/*
 * MyScrollViewクラスの定義
 */
class MyScrollView : public BScrollView {
// メソッド
public:
	// 初期化と解放
	MyScrollView(
		const char* name, BView* target, uint32 resizeMask, uint32 flags);
	MyScrollView(BMessage* message);
	~MyScrollView(void);

	/* アーカイビング */	
#if _PR2_COMPATIBLE_
#pragma export on
#endif
	static BArchivable*	Instantiate(BMessage* message);
#if _PR2_COMPATIBLE_
#pragma export reset
#endif
	status_t	Archive(BMessage* data, bool deep = true) const;

private:
	void	MessageReceived(BMessage* message);
	void	AboutRequested(void);
	// マウス応答
	void	MouseDown(BPoint where);
	static int32	DoDrag(void* data);
	
// データメンバ
private:
	BDragger*	fDragger;
	BPoint		fStartPos;
};


#endif  /* _MY_SCROLL_VIEW_H_ */

/*
 * End of File
 */
