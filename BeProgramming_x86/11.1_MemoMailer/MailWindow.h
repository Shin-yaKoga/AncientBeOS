/***********************************************************************
 * $Id$
 * Copyright (c) 1998 by Fort Gunnison, Inc.
 * 
 * MailWindow.h: メール送信ウィンドウ
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		May. 05, 1998
 * Last update:	May. 17, 1998
 ************************************************************************
*/

#ifndef _MAIL_WINDOW_H_
#define _MAIL_WINDOW_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include <interface/Window.h>


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/* 関連クラス・構造体 */
class	MyMemoWindow;
class	MailSender;


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * MailWindowクラスの定義
 */
class MailWindow : public BWindow {
// メソッド
public:
	// 初期化と解放
	MailWindow(MyMemoWindow* inMemoWin);
	~MailWindow(void);
	
private:
	// メッセージ応答
	void	DispatchMessage(BMessage* message, BHandler* handler);
	void	MessageReceived(BMessage* message);
	void	SendMail(void);
	void	Cancel(void);

	// ビュー部品の管理
	void	MakeContent(void);

// データメンバ
private:
	MyMemoWindow*	fMemoWin;
	MailSender*		fMailSender;
};


#endif  /* _MAIL_WINDOW_H_ */

/*
 * End of File
 */
