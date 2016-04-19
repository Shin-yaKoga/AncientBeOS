/***********************************************************************
 * $Id$
 * Copyright (c) 1997 by Fort Gunnison, Inc.
 * 
 * kgDebug.h: デバッグ用のユーティリティ
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 16, 1997
 * Last update: Dec. 16, 1997
 ************************************************************************
*/

#ifndef _KG_DEBUG_H_
#define _KG_DEBUG_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include <support/SupportDefs.h>


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/* デバッグ用のメッセージ出力 */
extern void	UnSupport(const char theProc[]);
extern void	BadArg(const char theProc[]);
extern void	Error(const char theProc[], int32 sts);


#endif  /* _KG_DEBUG_H_ */

/*
 * End of File
 */
