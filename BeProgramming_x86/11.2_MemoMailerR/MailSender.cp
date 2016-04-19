/************************************************************************
 * $Id$
 * Copyright (c) 1998 Fort Gunnison, Inc.
 *
 * MailSender.cp: MailSenderクラスの実装(改訂版)
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		May. 10, 1998
 * Last update:	May. 17, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "MailSender.h"
#include "KGUtility/kgAlert.h"
#include "KGUtility/kgDebug.h"

#include <app/Roster.h>
#include <mail/E-mail.h>
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
const char	kNoDaemonMsg[]		= "mail_daemon is not running.\n"
	"Would you like to launch it?";
const char	kMailDaemonSig[]	= "application/x-vnd.Be-POST";


/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/*
 * MailSenderクラスの非公開メソッド
 */
int32
MailSender::ExecSend (void* data)
{
	status_t	sts;
	MailSender*	theObj = (MailSender*)data;
	
	/* mail_daemonが動いているかチェック */
	if (! CheckMailDaemon()) {
		theObj->fThread = -1;
		return B_OK;  /* 起動できない */
	}
	
	/* メールの送信を実行 */
	sts = theObj->fMailObj->Send(true, false);  /* すぐに送信 */
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

bool
MailSender::CheckMailDaemon (void)
{
	bool		isRunning = false;
	status_t	sts;
	app_info	theAppInfo;
	
	/* mail_daemonの動作状態を取得 */
	sts = be_roster->GetAppInfo(kMailDaemonSig, &theAppInfo);
	if (sts == B_OK)
		isRunning = true;
	
	/* 動いていなければユーザに確認して起動 */
	if (!isRunning && ::KGConfirm(kNoDaemonMsg)) {
		sts = be_roster->Launch(kMailDaemonSig);
		if (sts != B_OK)
			goto err;
		isRunning = true;
	}
	
	return isRunning;
err:
	::Error("MailSender::CheckMailDaemon", sts);
	return false;
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
	fMailObj = NULL;
	fThread  = -1;
	this->Init();
}

MailSender::~MailSender (void)
{
	/* スレッドが生きていたら強制終了 */
	if (fThread != -1)
		(void)::kill_thread(fThread);
	delete fMailObj;
}

void
MailSender::Init (void)
{
	ASSERT(! this->IsSending());

	/* メールオブジェクトを生成し直す */
	delete fMailObj;
	fMailObj = new BMailMessage();
	
	return;
}

/*
 * メール送信; MailSender
 */
void
MailSender::SetHeader (const char* inToAddr,
	const char* /* inFromAddr */, const char* inSubject)
{
	ASSERT(inToAddr  != NULL);
	ASSERT(inSubject != NULL);
	ASSERT(! this->IsSending());

	status_t	sts;

	/* Toアドレスをセット*/
	sts = fMailObj->AddHeaderField(B_MAIL_TO, inToAddr);
	if (sts != B_OK)
		goto err;
	
	/* Subjectをセット */
	sts = fMailObj->AddHeaderField(B_MAIL_SUBJECT, inSubject);
	if (sts != B_OK)
		goto err;
	
	return;
err:
	::Error("MailSender::SetHeader", sts);
	return;
}

void
MailSender::SetContent (const char* inBodyText)
{
	ASSERT(inBodyText != NULL);
	ASSERT(! this->IsSending());

	status_t	sts;
	
	/* メール本文をセット */
	sts = fMailObj->AddContent(inBodyText, strlen(inBodyText));
	if (sts != B_OK)
		goto err;
	
	return;
err:
	::Error("MailSender::SetContent", sts);
	return;
}

void
MailSender::AddEnclosure (const entry_ref& inRef)
{
	ASSERT(! this->IsSending());

	status_t	sts;
	entry_ref	copiedRef = inRef;
	
	/* 添付書類をセット */
	sts = fMailObj->AddEnclosure(&copiedRef);
	if (sts != B_OK)
		goto err;
	
	return;
err:
	::Error("MailSender::AddEnclosure", sts);
	return;
}

void
MailSender::Send (void)
{
	ASSERT(! this->IsSending());

	status_t	sts;

	/* 送信処理用のスレッドを生成して起動 */
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
 *		べきではない。Mail Kitの呼び出しを強制終了することによって、
 *		mail_daemonや、さらにはnet_serverをラッシュさせてしまう可能性
 *		すらあるからである。('98. 5/16, koga@ftgun.co.jp)
 */
}


/*
 * End of File
 */
