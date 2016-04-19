/***********************************************************************
 * $Id$
 * Copyright (c) 1998 by Fort Gunnison, Inc.
 * 
 * ScriptorView.h: スクリプトの入力用ビュー
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 04, 1998
 * Last update:	Jan. 05, 1998
 ************************************************************************
*/

#ifndef _SCRIPTOR_VIEW_H_
#define _SCRIPTOR_VIEW_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include <interface/View.h>


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/* 関連クラス・構造体 */
class	ScriptEngine;
class	BBox;


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * ScriptorViewクラスの定義
 */
class ScriptorView : public BView {
// メソッド
public:
	// 初期化と解放
	ScriptorView(BRect frame, uint32 resizeMask);
	~ScriptorView(void);

private:
	// 描画処理
	void	AttachedToWindow(void);
	void	MakeContent(void);
	void	MakeActionBox(BBox* inBox);
	void	AdjustContent(void);
	void	AdjustActionBox(BBox* inBox);

	// メッセージ処理
	void	MessageReceived(BMessage* message);
	void	MouseDown(BPoint where);
	void	ExecScript(void);
	
	// スクリプト実行
	void	DoShowAbout(void);
	void	DoQuit(void);
	void	DoMoveWindow(void);
	void	DoPushButton(void);
	void	SendCommonMessage(uint32 theCommand);
	const char*	GetWindowInfo(void);

// データメンバ
private:
	uint32			fCurrAction;	/* 現在選択中のアクション */
	ScriptEngine*	fEngine;		/* スクリプトの実行用 */
};


#endif  /* _SCRIPTOR_VIEW_H_ */

/*
 * End of File
 */
