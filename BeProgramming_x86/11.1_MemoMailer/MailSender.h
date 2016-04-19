/***********************************************************************
 * $Id$
 * Copyright (c) 1998 by Fort Gunnison, Inc.
 * 
 * MailSender.h: メール送信オブジェクト
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		May. 10, 1998
 * Last update:	May. 17, 1998
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
				const char* inFromAddr, const char* inSubject);
	void	SetContent(const char* inBodyText);
	void	Send(void);

	// 送信動作の制御
	bool	IsSending(void);
	bool	Cancel(void);

private:
	static int32	ExecSend(void* data);

// データメンバ
private:
	thread_id	fThread;	/* 送信スレッド */
	char*		fToAddr;	/* Toフィールドの格納用 */
	char*		fSubject;	/* Subjectフィールドの格納用 */
	char*		fBodyText;	/* メール本文の格納用 */
};


#endif  /* _MAIL_SENDER_H_ */

/*
 * End of File
 */
