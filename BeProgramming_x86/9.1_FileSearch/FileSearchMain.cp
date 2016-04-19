/************************************************************************
 * $Id$
 * Copyright (c) 1998 Fort Gunnison, Inc.
 *
 * FileSearchMain.cp: FileSearchのメインルーチン
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Jan. 15, 1998
 * Last update:	Jan. 15, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "FileSearchApp.h"


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
/*      #########################################################       */

int
main (void)
{
	FileSearchApp	appObj;
	
	appObj.Run();
	
	return 0;
}


/*
 * End of File
 */
