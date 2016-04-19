/***********************************************************************
 * $Id$
 * Copyright (c) 1998 by Fort Gunnison, Inc.
 * 
 * MemoWindowMessages.h: MemoWindow用のメッセージコード
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		May. 16, 1998
 * Last update:	May. 16, 1998
 ************************************************************************
*/

#ifndef _MEMO_WINDOW_MESSAGES_H_
#define _MEMO_WINDOW_MESSAGES_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/* メニューのメッセージコード */
enum {
	NEW_DOC		= 'ndoc',
	OPEN_DOC	= 'odoc',
	SAVE_DOC	= 'sdoc',
	SAVE_DOC_AS	= 'sdas',
	EDIT_CLEAR	= 'cler',
	SET_FONT_FAMILY	= 'sfnf',
	SET_FONT_STYLE	= 'sfns',
	SET_SIZE		= 'ssiz',
	ADJUST_WIDTH	= 'adjw'
};

/* 文字列定数 */
extern const char	kCloseLabel[];


/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */


#endif  /* _MEMO_WINDOW_MESSAGES_H_ */

/*
 * End of File
 */
