/************************************************************************
 * $Id$
 * Copyright (c) 1998 Fort Gunnison, Inc.
 *
 * MailSender.cp: MailSenderクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		May. 10, 1998
 * Last update:	May. 16, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "MailSender.h"
#include "MySMTPClient.h"
#include "KGUtility/kgAlert.h"
#include "KGUtility/kgDebug.h"

#include <app/Roster.h>
#include <mail/E-mail.h>
#include <net/netdb.h>
#include <support/Debug.h>

#include <string.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/* 文字列定数 */
const char	kSendingNowMsg[]	= "Do you abort the session?";
const char	kBadPrefMsg[]		= "You must set up the mail preference.";
const char	kMailPrefSig[]		= "application/x-vnd.Be-mail";


/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/*
 * 内部手続き
 */
static bool
CheckMailPref (
	char* outSmtpHost, mail_pop_account* outPopAccount)
{
	bool		isValid = true;
	status_t	sts;

	/* メール関連のプリファレンス情報を取得 */
	sts = ::get_smtp_host(outSmtpHost);
	if (sts != B_OK)
		goto err;
	sts = ::get_pop_account(outPopAccount, 0);
	if (sts != B_OK)
		goto err;
	
	/* 設定されていなければmail preferenceを起動 */
	if (outSmtpHost[0] == '\0' || outPopAccount->reply_to[0] == '\0') {
		team_id	theTeamID;

		::KGAlert(kBadPrefMsg);
		(void)be_roster->Launch(kMailPrefSig, (BMessage*)NULL, &theTeamID);
		(void)be_roster->ActivateApp(theTeamID);
		isValid = false;
	}
	
	return isValid;
err:
	::Error("MailSender::CheckMailPref", sts);
	return false;
/*
 * 注意：ここでは、メールに関するプリファレンス情報をシステムから取得
 *		ている。つまり、mail preferenceの設定内容を流用している。実装
 *		を簡単にするために、自前で初期設定ファイルを用意するのをサボっ
 *		たというのが正直なところだが、このようにすれば、全てのメール
 *		アプリケーションが同じプリファレンスを共有できるので、むしろ
 *		好ましい実装かも知れない。('98. 5/17, koga@ftgun.co.jp)
 */
}


/*
 * MailSenderクラスの非公開メソッド
 */
int32
MailSender::ExecSend (void* data)
{
	status_t			sts;
	MailSender*			theObj = (MailSender*)data;
	char				smtpHost[B_MAX_HOST_NAME_LENGTH];
	mail_pop_account	popAccount;
	MySMTPClient		theProxy;
	
	/* メール関連のプリファレンスがセット済みかチェック */
	if (! CheckMailPref(smtpHost, &popAccount)) {
		theObj->fThread = -1;
		return B_OK;  /* セットできない */
	}
	
	/* メールの送信を実行 */
	sts = theProxy.ConnectTo(smtpHost);
	if (sts != B_OK)
		goto err;
	sts = theProxy.SendMail(theObj->fToAddr,
		popAccount.reply_to, theObj->fSubject, theObj->fBodyText
	);
	(void)theProxy.Close();
	if (sts != B_OK)
		goto err;
	
	/* 完了通知 */
	theObj->Invoke();
	theObj->fThread = -1;  /* 自身の終了を記録 */

	return B_OK;
err:
	::Error("MailSender::ExecSend", sts);
	theObj->fThread = -1;  /* 自身の終了を記録 */
	return sts;
}


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/* MailSenderクラスの公開メソッド */
/*
 * 初期化と解放; MailSender
 */
MailSender::MailSender (BMessage* message, BHandler* handler)
	: BInvoker(message, handler)
{
	fThread = -1;
	fToAddr = fSubject = fBodyText = NULL;
	this->Init();
}

MailSender::~MailSender (void)
{
	/* スレッドが生きていたら強制終了 */
	if (fThread != -1)
		(void)::kill_thread(fThread);
	delete fToAddr;
	delete fSubject;
	delete fBodyText;
}

void
MailSender::Init (void)
{
	ASSERT(! this->IsSending());

	delete [] fToAddr;
	delete [] fSubject;
	delete [] fBodyText;
	fToAddr = fSubject = fBodyText = NULL;
	
	return;
}

/*
 * メール送信; MailSender
 */
void
MailSender::SetHeader (const char* inToAddr,
	const char* /* inFromAddr */, const char* inSubject)
{
	ASSERT(inToAddr   != NULL);
	ASSERT(inSubject  != NULL);
	ASSERT(! this->IsSending());
	
	/* Toフィールドをセット */
	if (fToAddr != NULL)
		delete [] fToAddr;
	fToAddr = new char[strlen(inToAddr) + 1];
	strcpy(fToAddr, inToAddr);
	
	/* Subjectフィールドをセット */
	if (fSubject != NULL)
		delete [] fSubject;
	fSubject = new char[strlen(inSubject) + 1];
	strcpy(fSubject, inSubject);
	
	return;
}

void
MailSender::SetContent (const char* inBodyText)
{
	ASSERT(inBodyText != NULL);
	ASSERT(! this->IsSending());
	
	/* メール本文をセット */
	if (fBodyText != NULL)
		delete [] fBodyText;
	fBodyText = new char[strlen(inBodyText) + 1];
	strcpy(fBodyText, inBodyText);
	
	return;
}

void
MailSender::Send (void)
{
	ASSERT(! this->IsSending());
	
	status_t	sts;
	
	/* 送信用のスレッドを生成して起動 */
	fThread = ::spawn_thread(
		ExecSend, "sender", B_LOW_PRIORITY, this
	);
	sts = ::resume_thread(fThread);
	if (sts != B_OK)
		goto err;
	
	return;
err:
	::Error("MailSender::Send", sts);
	return;
}

/*
 * 送信動作の制御; MailSender
 */
bool
MailSender::IsSending (void)
{
	return (fThread != -1);
}

bool
MailSender::Cancel (void)
{
	bool	isCanceled = true;
	
	/* 送信中なら、ユーザに確認してスレッドを強制終了 */	
	if (this->IsSending()) {
		if (! ::KGConfirm(kSendingNowMsg))
			isCanceled = false;
		else {
			(void)::kill_thread(fThread);
			fThread = -1;
		}
	}
	
	return isCanceled;
/*
 * 注意：ここではkill_thread()を使って強制終了しているが、本当はそうす
 *		べきではない。Network Kitの呼び出しを強制終了することによって、
 *		net_serverをクラッシュさせてしまう可能性がある。
 *		('98. 5/16, koga@ftgun.co.jp)
 */
}


/*
 * End of File
 */
