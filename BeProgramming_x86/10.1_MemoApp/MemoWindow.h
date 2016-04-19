/***********************************************************************
 * $Id$
 * Copyright (c) 1998 by Fort Gunnison, Inc.
 * 
 * MemoWindow.h: テキスト編集ウィンドウ
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		May. 04, 1998
 * Last update:	May. 16, 1998
 ************************************************************************
*/

#ifndef _MEMO_WINDOW_H_
#define _MEMO_WINDOW_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include <interface/Window.h>


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/* 関連クラス・構造体 */
class	BFilePanel;
class	MyTextView;

/* xx */
extern const char	kMyAppSig[];


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * MemoWindowクラスの定義
 */
class MemoWindow : public BWindow {
// メソッド
public:
	// 初期化と解放
	MemoWindow(BRect frame, const char* inTitle, uint32 quitMsg);
	virtual ~MemoWindow(void);
	void	InitContent(BMenuBar* inMenuBar, bool doShow);
	
	// テキストファイルの読み込みと保存
	void	LoadFile(const entry_ref& inRef);
	bool	GetEntryRef(entry_ref* outRef) const;

protected:
	// テキストビューの取得
	MyTextView*	GetTextView(void);

	// メッセージ応答
	virtual void	MessageReceived(BMessage* message);
	
private:
	// テキストファイルの読み込みと保存
	static status_t	CreateDocument(const entry_ref& inRef);
	void	SaveDocumentTo(const entry_ref& inNewRef);
	void	ShowSavePanel(void);
	
	// メッセージ応答
	void	SaveRequested(BMessage* message);
	
	// 終了時処理
	bool	QuitRequested(void);
	void	Quit(void);
	
	// メニュー応答
	void	DoSave(void);
	void	DoClear(void);
	void	SetFontFamily(BMessage* message);
	void	SetFontStyle(BMessage* message);
	void	SetFontSize(BMessage* message);
	void	ToggleLineAdjust(BMessage* message);
	
	// メニュー調節
	void	MenusBeginning(void);
	void	AdjustFileMenu(BMenuBar* inMenuBar);
	void	AdjustEditMenu(BMenuBar* inMenuBar);
	void	AdjustFontMenu(BMenuBar* inMenuBar);
	
// データメンバ
private:
	entry_ref	fFileRef;		/* データファイル */
	bool		fHasFile;		/* fFileRefが有効か */
	bool		fInQuitting;	/* 終了処理の途中か */
	uint32		fQuitMsg;		/* 終了通知メッセージ */
	BFilePanel*	fSavePanel;		/* セーブダイアログ */
	
	// クラスデータ
	static BPoint	sNextLeftTop;	/* 次にウィンドウを開く位置 */
};


#endif  /* _MEMO_WINDOW_H_ */

/*
 * End of File
 */
