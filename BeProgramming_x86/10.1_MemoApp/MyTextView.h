/***********************************************************************
 * $Id$
 * Copyright (c) 1998 by Fort Gunnison, Inc.
 *
 * MyTextView.h: MemoWindow用のテキストビュー
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

/*
 * MyTextViewクラスの定義
 */
class MyTextView : public BTextView {
// メソッド
public:
	// 初期化と解放
	MyTextView(BRect frame, const char* name, BRect textRect,
		uint32 resizingMode, uint32 flags);
	~MyTextView(void);

	// 属性の取得と設定
	bool	IsDirty(void) const;
	void	MakeClean(void);
	void	SetLineAdjust(bool doit);

private:
	void	FrameResized(float newWidth, float newHeight);
	void	InsertText(const char* text, int32 length,
		int32 offset, const text_run_array* runs);
	void	DeleteText(int32 start, int32 finish);

private:
	bool	fIsDirty;		/* テキストが変更されたか */
	bool	fDoLineAdjust;	/* テキスト表示幅を調節するか */
};


#endif  /* _MY_TEXT_VIEW_H_ */

/*
 * End of File
 */
