/***********************************************************************
 * $Id$
 * Copyright (c) 1997-1998 by Fort Gunnison, Inc.
 * 
 * MessengerView.h: 動作中のアプリケーションの一覧表示とメッセージ送付
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 24, 1997
 * Last update:	Jan. 03, 1998
 ************************************************************************
*/

#ifndef _MESSENGER_VIEW_H_
#define _MESSENGER_VIEW_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include <interface/View.h>


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * MessengerViewクラスの定義
 */
class MessengerView : public BView {
// メソッド
public:
	// 初期化と解放
	MessengerView(BRect frame, uint32 resizeMask);
	~MessengerView(void);

private:
	// 描画処理
	void	AttachedToWindow(void);
	void	MakeContent(void);
	void	AdjustContent(void);

	// メッセージ処理
	void	MessageReceived(BMessage* message);
	void	Pulse(void);
	void	AppSelected(void);
	void	SendMessage(void);
	
	// アプリケーションの一覧表示
	void	CheckApps(void);
	void	UpdateAppList(const BList* newList);

// データメンバ
private:
	BList*	fAppList;	/* 動作中のアプリケーション群 */
};


#endif  /* _MESSENGER_VIEW_H_ */

/*
 * End of File
 */
