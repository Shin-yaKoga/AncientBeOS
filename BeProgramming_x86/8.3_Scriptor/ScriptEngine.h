/***********************************************************************
 * $Id$
 * Copyright (c) 1998 by Fort Gunnison, Inc.
 * 
 * ScriptEngine.h: スクリプト実行用のクラス
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 05, 1998
 * Last update:	Apr. 22, 1998
 ************************************************************************
*/

#ifndef _SCRIPT_ENGINE_H_
#define _SCRIPT_ENGINE_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include <kernel/OS.h>


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/* 関連クラス・構造体 */
class	BMessage;
class	BMessenger;
class	BPoint;


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * ScriptEngineクラスの定義
 */
class ScriptEngine {
// メソッド
public:
	// 初期化と解放
	ScriptEngine(void);
	~ScriptEngine(void);
	
	// アプリケーション操作の開始と終了
	status_t	TellApplication(const char inAppSig[]);
	void		EndTell(void);
	
	// メッセージ送信オブジェクトの生成
	status_t	GetRmtApplication(BMessenger* outMessenger);
	status_t	GetRmtWindow(
					const char inWindowInfo[], BMessenger* outMessenger);
	status_t	GetRmtControl(
					const char inWindowInfo[],
					const char inControlInfo[], BMessenger* outMessenger);
	
	// ユーティリティ
	static status_t	AddSpecifierTo(
						const char inPropertyName[],
						const char inPropertyInfo[], BMessage* ioMessage);
	static status_t	MoveWindow(
						const BMessenger& inRmtWindow, BPoint inNewPoint);
	static status_t	PushButton(const BMessenger& inRmtControl);

// データメンバ
private:
	team_id	fTargetApp;	/* 操作対象のアプリケーション */
};


#endif  /* _SCRIPT_ENGINE_H_ */

/*
 * End of File
 */
