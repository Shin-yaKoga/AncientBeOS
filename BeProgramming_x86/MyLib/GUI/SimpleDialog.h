/***********************************************************************
 * $Id$
 * Copyright (c) 1998 by Fort Gunnison, Inc.
 * 
 * SimpleDialog.h: 入力フィールドを一つだけ持つダイアログ用
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 27, 1998
 * Last update:	May. 04, 1998
 ************************************************************************
*/

#ifndef _SIMPLE_DIALOG_H_
#define _SIMPLE_DIALOG_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include <interface/Window.h>


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/* 関連クラス・構造体 */
class	BTextControl;

/* メッセージコード */
enum {
	KGSD_OK		= 'ok  ',	/* OK */
	KGSD_CANCEL	= 'cncl'	/* キャンセル */
};

/* 文字列定数 */
extern const char	kValueArg[];


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * SimpleDialogクラスの定義
 */
class SimpleDialog : public BWindow {
// メソッド
public:
	// 初期化と解放
	SimpleDialog(BRect frame, const char* title,
		window_type type, uint32 flags, uint32 completionMsg);
	~SimpleDialog(void);
	void		SetContent(BView* initContent, const char* inFieldName);
	
	// ダイアログの表示
	status_t	Go(const BMessenger& inTarget);

private:
	// 終了時処理
	void		Quit(void);

	// メッセージ処理
	void		DispatchMessage(BMessage* message, BHandler* handler);
	void		MessageReceived(BMessage* message);
	void		SendCompletionMessage(void);
	void		CheckEscape(BMessage* message);

// データメンバ
private:
	uint32			fCompletionMsg;	/* OKが押されたら送るメッセージ */
	BTextControl*	fInputField;	/* 入力フィールド */
	BMessenger		fTarget;		/* メッセージの送付先 */
};


#endif  /* _SIMPLE_DIALOG_H_ */

/*
 * End of File
 */
