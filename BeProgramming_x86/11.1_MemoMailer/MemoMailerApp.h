/***********************************************************************
 * $Id$
 * Copyright (c) 1998 by Fort Gunnison, Inc.
 * 
 * MemoMailerApp.h: MemoMailerのアプリケーションクラス
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		May. 04, 1998
 * Last update:	May. 04, 1998
 ************************************************************************
*/

#ifndef _MEMO_APP_H_
#define _MEMO_APP_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include <app/Application.h>


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/* 関連クラス・構造体 */
class	BAlert;
class	BFilePanel;
class	MemoWindow;
class	BRefFilter;


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * MemoAppクラスの定義
 */
class MemoApp : public BApplication {
// メソッド
public:
	// 初期化と解放
	MemoApp(void);
	~MemoApp(void);

private:
	// 起動・終了時処理
	void	ReadyToRun(void);
	
	// メッセージ応答
	void	MessageReceived(BMessage* message);
	void	RefsReceived(BMessage* message);
	void	AboutRequested(void);
	
	// ドキュメントウィンドウの管理
	MemoWindow*	NewDocument(bool doShow = true);
	void	OpenDocument(const entry_ref& inRef);
	void	ShowOpenPanel(void);
	void	DocumentClosed(void);
	
	// xx
	void	ConfigureFontMenu(void);

// データメンバ
private:
	BAlert*		fAboutBox;			/* アバウトダイアログ */
	int32		fCreatedDocs;		/* 生成したドキュメントウィンドウの数 */
	int32		fNumDocWindow;		/* 開いているドキュメントウィンドウの数 */
	BMessage*	fFontMenuArchive;	/* フォントメニュー */
	BFilePanel*	fOpenPanel;			/* オープンダイアログ */
	BRefFilter*	fRefFilter;			/* テキストファイル用のフィルタ */
};


#endif  /* _MEMO_APP_H_ */

/*
 * End of File
 */
