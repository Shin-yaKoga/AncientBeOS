/***********************************************************************
 * $Id$
 * Copyright (c) 1998 by Fort Gunnison, Inc.
 * 
 * MakeResAp.h: MakeRes用のアプリケーションクラス
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Apr. 22, 1998
 * Last update:	Apr. 22, 1998
 ************************************************************************
*/

#ifndef _MAKE_RES_APP_H_
#define _MAKE_RES_APP_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include <app/Application.h>


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * MakeResAppクラスの定義
 */
class MakeResApp : public BApplication {
// メソッド
public:
	// 初期化と解放
	MakeResApp(void);
	~MakeResApp(void);

private:
	// 起動と終了
	void		ArgvReceived(int32 argc, char** argv);
	void		ReadyToRun(void);
	
	// メッセージ応答
	void		MessageReceived(BMessage* message);
	void		SaveRequested(BMessage* message);
	
	// リソースファイルの作成
	status_t	MakeResFile(const entry_ref& inRef, bool clobber);
	void		OpenSavePanel(void);

// データメンバ
private:
	bool	fArgReceived;	/* コマンド引数を受け取って処理したか */
};


#endif  /* _MAKE_RES_APP_H_ */

/*
 * End of File
 */
