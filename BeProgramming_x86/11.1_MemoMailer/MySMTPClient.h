/***********************************************************************
 * $Id$
 * Copyright (c) 1998 by Fort Gunnison, Inc.
 * 
 * MySMTPClient.h: SMTPサーバとの通信用
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		May. 10, 1998
 * Last update:	May. 16, 1998
 ************************************************************************
*/

#ifndef _MY_SMTP_CLIENT_H_
#define _MY_SMTP_CLIENT_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include <support/SupportDefs.h>


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * MySMTPClientクラスの定義
 */
class MySMTPClient {
// メソッド
public:
	// 初期化と解放
	MySMTPClient(void);
	~MySMTPClient(void);
	
	// セッション管理
	status_t	ConnectTo(const char smtpHost[]);
	status_t	Close(void);
	status_t	SendMail(const char inToAddr[], const char inFromAddr[],
					const char inSubject[], const char inBodyText[]);

private:
	// SMTP通信の実行
	int16	ExeCommand(const char* cmdStr);
	int16	GetReply(int16* outCode,
				bool* ioPending, bool inExpectEOF = true);
	void	LostPeer(void);
	
	// 一文字読み出し
	static uint8	GetChar(int theSocket);
	
// データメンバ
private:
	int		fSocket;	/* サーバとの通信用 */
};


#endif  /* _MY_SMTP_CLIENT_H_ */

/*
 * End of File
 */
