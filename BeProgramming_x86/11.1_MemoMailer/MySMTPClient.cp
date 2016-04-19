/************************************************************************
 * $Id$
 * Copyright (c) 1998 Fort Gunnison, Inc.
 *
 * MySMTPClient.cp: MySMTPClientクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		May. 10, 1998
 * Last update:	May. 21, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "MySMTPClient.h"
#include "KGUtility/kgDebug.h"

#include "Arpa/telnet_and_ftp.h"  /* '98. 5/21 */

#include <storage/Entry.h>
#include <mail/E-mail.h>
#include <net/netdb.h>
#include <support/Debug.h>

#include <errno.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/* 各種定数 */
const uint16	kSmtpPort	= 25;  /* SMTPのポート番号 */


/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/*
 * MySMTPClientクラスの非公開メソッド
 */
int16
MySMTPClient::ExeCommand (const char* cmdStr)
{
	int16	theCode;
	bool	isPending = false;
	bool	expectEOF = (strcmp(cmdStr, "QUIT") == 0);

	/* コマンドを送信 */	
	if (::send(fSocket, cmdStr, strlen(cmdStr), 0) < 0)
		goto err;
	
	/* 改行文字を送信 */
	if (::send(fSocket, "\r\n", 2, 0) < 0)
		goto err;

	/* サーバからの応答を受信 */	
	return this->GetReply(&theCode, &isPending, expectEOF);
err:
	::Error("MySMTPClient::ExeCommand", errno);
	this->LostPeer();
	return ERROR;
}

int16
MySMTPClient::GetReply (
	int16* outCode, bool* ioPending, bool inExpectEOF)
{
	char	strBuf[80];
	char	replyStr[1024], *cursor;
	uint8	theChar;
	int16	codeHead, dig;
	int16	origCode = 0;
	bool	isContinue = false;
	
	for (;;) {
		dig = codeHead = *outCode = 0;
		cursor = replyStr;
		
		/* 改行文字を受信するまで繰り返し */
		while ((theChar = GetChar(fSocket)) != '\n') {
			if (theChar == IAC) {  /* telnetコマンドを処理 */
				switch (theChar = GetChar(fSocket)) {
				case WILL:
				case WONT:
					theChar = GetChar(fSocket);
					sprintf(strBuf, "%c%c%c", IAC, DONT, theChar);
					if (::send(fSocket, strBuf, strlen(strBuf), 0) < 0)
						goto err;
					break;
				case DO:
				case DONT:
					theChar = GetChar(fSocket);
					sprintf(strBuf, "%c%c%c", IAC, WONT, theChar);
					if (::send(fSocket, strBuf, strlen(strBuf), 0) < 0)
						goto err;
					break;
				default:
					break;
				}
				continue;
			}
			dig++;
			if (theChar == EOF) {  /* コネクションが切断された */
				if (inExpectEOF) {
					*outCode = 221;
					return 0;
				}
				this->LostPeer();  /* 接続を解除 */
				*outCode = 421;
				return TRANSIENT;
			} else if (dig < 4 && isdigit(theChar)) {
				*outCode = *outCode * 10 + (theChar - '0');
			}
			if (dig == 4 && theChar == '-') {
				if (isContinue)
					*outCode = 0;
				isContinue = true;
			}
			if (codeHead == 0)
				codeHead = theChar;
			if (cursor < &(replyStr[sizeof(replyStr) - 1]))
				*cursor++ = theChar;
		}
		
		/* 他のメッセージ行が残っていなければ終了 */
		if (!isContinue || *outCode == origCode)
			break;
		if (origCode == 0)
			origCode = *outCode;
	}
	
	/* SMTPサーバの状態をチェック */
	*cursor = '\0';
	if (codeHead != '1')
		*ioPending = false;
	if (*outCode == 421 || origCode == 421)
		this->LostPeer();
	
	return (codeHead - '0');
err:
	::Error("MySMTPClient::GetReply", *outCode);
	return TRANSIENT;
}

void
MySMTPClient::LostPeer (void)
{
	(void)::closesocket(fSocket);
	fSocket = -1;

	return;
}

/*
 * 一文字読み出し
 */
uint8
MySMTPClient::GetChar (int theSocket)
{
	uint8	theChar;
	ssize_t	theSize;
	
	theSize = ::recv(theSocket, &theChar, sizeof(theChar), 0);
	
	return (theSize == sizeof(theChar) ? theChar : EOF);
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/* MySMTPClientクラスの公開メソッド */
/*
 * 初期化と解放; MySMTPClient
 */
MySMTPClient::MySMTPClient (void)
{
	fSocket = -1;
}

MySMTPClient::~MySMTPClient (void)
{
	ASSERT(fSocket < 0);

	/* ソケットが解放されていなければ解放 */
	if (fSocket >= 0)
		this->LostPeer();
}

/*
 * セッション管理; MySMTPClient
 */
status_t
MySMTPClient::ConnectTo (const char smtpHost[])
{
	status_t			sts;
	ulong				ipAddr;
	struct sockaddr_in	rmtSockAddr;

	/* ソケットを生成 */
	fSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fSocket < 0) {
		sts = errno;
		goto err;
	}
	
	/* SMTPサーバのIPアドレスを取得 */
	ipAddr = ::inet_addr(smtpHost);
	if (ipAddr == -1) {
		struct hostent*	theHostEnt;
		
		theHostEnt = ::gethostbyname(smtpHost);
		if (theHostEnt == NULL) {
			sts = h_errno;
			goto err_release;
		}
		ipAddr = *(ulong*)theHostEnt->h_addr_list[0];
	}
	
	/* 接続先のアドレス情報を作成 */
	bzero(&rmtSockAddr, sizeof(rmtSockAddr));
	rmtSockAddr.sin_family      = AF_INET;
	rmtSockAddr.sin_port        = htons(kSmtpPort);
	rmtSockAddr.sin_addr.s_addr = ipAddr;
	
	/* SMTPサーバと接続 */
	if (::connect(fSocket,
			(struct sockaddr*)&rmtSockAddr, sizeof(rmtSockAddr)) < 0) {
		sts = errno;
		goto err_release;
	} else {
		int16	theCode;
		bool	isPending = false;
	
		if (this->GetReply(&theCode, &isPending, false) > COMPLETE) {
			this->Close();
			sts = theCode;
			goto err;
		}
	}
	
	return B_OK;
err_release:
	(void)::closesocket(fSocket);
	fSocket = -1;
err:	
	::Error("MySMTPClient::ConnectTo", sts);
	return B_ERROR;
}

status_t
MySMTPClient::Close (void)
{
	/* 既に接続を解除しているなら何もしない */
	if (fSocket < 0)
		return B_OK;

	/* SMTPサーバに終了通知して接続を解除 */	
	(void)this->ExeCommand("QUIT");
	this->LostPeer();
	
	return B_OK;
}

status_t
MySMTPClient::SendMail (const char inToAddr[], const char inFromAddr[],
	const char inSubject[], const char inBodyText[])
{
	status_t	sts;
	int16		theCode;
	char		strBuf[1024];
	char		myHostName[B_MAX_HOST_NAME_LENGTH];
	char		timeStr[256];
	time_t		curTime;

	/* HELOコマンドを送付 */
	(void)::gethostname(myHostName, B_MAX_HOST_NAME_LENGTH);
	sprintf(strBuf, "HELO %s", myHostName);
	theCode = this->ExeCommand(strBuf);
	if (theCode != COMPLETE)
		goto err_code;
	
	/* MAIL FROMコマンドを送付 */
	sprintf(strBuf, "MAIL FROM:<%s>", inFromAddr);
	theCode = this->ExeCommand(strBuf);
	if (theCode != COMPLETE)
		goto err_code;
	
	/* RCPT TOコマンドを送付 */
	sprintf(strBuf, "RCPT TO:<%s>", inToAddr);
	theCode = this->ExeCommand(strBuf);
	if (theCode != COMPLETE)
		goto err_code;
	
	/* DATAコマンドを送付 */
	sprintf(strBuf, "DATA");
	theCode = this->ExeCommand(strBuf);
	if (theCode != CONTINUE)
		goto err_code;
	
	/* ヘッダ情報を送信 */
	time(&curTime);
	strcpy(timeStr, ctime(&curTime));
	timeStr[strlen(timeStr) - 1] = '\0';  /* 改行文字を削除 */
	sprintf(strBuf, "Date: %s\r\nSubject: %s\r\n\r\n", timeStr, inSubject);
	if (::send(fSocket, strBuf, strlen(strBuf), 0) < 0) {
		sts = errno;
		this->LostPeer();
		goto err;
	}
	
	/* メール本文を送信 */
	if (inBodyText[0] != '\0') {
		if (::send(fSocket, inBodyText, strlen(inBodyText), 0) < 0) {
			sts = errno;
			this->LostPeer();
			goto err;
		}
	}
	
	/* メール転送のトランザクションを終了 */
	theCode = this->ExeCommand("\r\n.");
	if (theCode != COMPLETE)
		goto err_code;
	
	return B_OK;
err_code:
	sts = theCode;
err:
	::Error("MySMTPClient::SendMail", sts);
	return sts;
}


/*
 * End of File
 */
