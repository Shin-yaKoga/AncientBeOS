/***********************************************************************
 * $Id$
 * Copyright (c) 1998 by Fort Gunnison, Inc.
 * 
 * MailSender.h: メール送信オブジェクト(改訂版)
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		May. 10, 1998
 * Last update:	May. 16, 1998
 ************************************************************************
*/

#ifndef _MAIL_SENDER_H_
#define _MAIL_SENDER_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include <app/Invoker.h>


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/* 関連クラス・構造体 */
class	BMailMessage;


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * MailSenderクラスの定義
 */
class MailSender : public BInvoker {
// メソッド
public:
	// 初期化と解放
	MailSender(BMessage* message, BHandler* handler);
	~MailSender(void);
	void	Init(void);
	
	// メール送信
	void	SetHeader(const char* inToAddr,
				const char* /* inFromAddr */, const char* inSubject);
	void	SetContent(const char* inBodyText);
	void	AddEnclosure(const entry_ref& inRef);
	void	Send(void);

	// 送信動作の制御
	bool	IsSending(void);
	bool	Cancel(void);

private:
	static int32	ExecSend(void* data);
	static bool		CheckMailDaemon(void);

// データメンバ
private:
	BMailMessage*	fMailObj;	/* メールオブジェクト */
	thread_id		fThread;	/* 送信スレッド */
};


#endif  /* _MAIL_SENDER_H_ */

/*
 * End of File
 */
