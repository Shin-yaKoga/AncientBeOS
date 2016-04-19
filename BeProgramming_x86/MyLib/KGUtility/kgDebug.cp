/************************************************************************
 * $Id$
 * Copyright (c) 1997, Fort Gunnison, Inc.
 *
 * kgDebug.cp: デバッグ用ユーティリティ手続きの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 16, 1997
 * Last update:	Dec. 16, 1997
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "kgDebug.h"
#include "kgAlert.h"
#include <stdio.h>


/*      #########################################################       */
/*      #               L O C A L   D E F I N E S               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #               L O C A L   S T O R A G E               #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #            P R I V A T E   F U N C T I O N S          #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################		*/

/*
 * デバッグ用のメッセージ出力
 */
void
UnSupport(const char theProc[])
{
	char	mesg[256];
	
	sprintf(mesg, "Sorry, but %s() is not implemented yet.", theProc);
	KGAlert(mesg);
	
	return;
}

void
BadArg (const char theProc[])
{
	char	mesg[256];
	
	sprintf(mesg, "Bad argument for %s().", theProc);
	KGAlert(mesg);
	
	return;
}

void
Error (const char theProc[], int32 sts)
{
	char	mesg[256];
	
	sprintf(mesg, "Error at %s(), sts= %d.", theProc, sts);
	KGAlert(mesg);
	
	return;
}


/*
 * End of File
 */
