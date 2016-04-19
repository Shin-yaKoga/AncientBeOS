/***********************************************************************
 * $Id$
 * Copyright (c) 1998 by Fort Gunnison, Inc.
 * 
 * StickyMemoMenu.h: StickyMemoアプリケーションのメニュー定義
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		May. 04, 1998
 * Last update:	May. 16, 1998
 ************************************************************************
*/

#ifndef _STICKY_MEMO_MENU_
#define _STICKY_MEMO_MENU_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "MemoWindowMessages.h"


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/* 関連クラス・構造体 */
class	BMenuBar;


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * MemoAppMenuクラスの定義
 */
class MemoAppMenu {
// メソッド
public:
	static BMenuBar*	MakeDocMenuBar(void);
};


#endif  /* _STICKY_MEMO_MENU_ */

/*
 * End of File
 */
