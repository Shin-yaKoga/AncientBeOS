/************************************************************************
 * $Id$
 * Copyright (c) 1997-1998 Fort Gunnison, Inc.
 *
 * kgAlert.cp: アラート表示ユーティリティの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 16, 1997
 * Last update:	Jan. 27, 1998
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "kgAlert.h"
#include <interface/Alert.h>
#include <support/Beep.h>


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

/*
 * アラートパネルの表示
 */
void
KGAlert (const char mesg[])
{
	BAlert*	alertPanel;
	
	/* ビープを鳴らす */
	(void)::beep();
	
	/* アラートパネルを表示 */
	alertPanel = new BAlert("", mesg,
		"OK", NULL, NULL, B_WIDTH_FROM_WIDEST, B_WARNING_ALERT);
	(void)alertPanel->Go();
	
	return;
}

bool
KGConfirm (const char mesg[])
{
	bool	isOK;
	BAlert*	alertPanel;

	/* アラートパネルを生成 */
	alertPanel = new BAlert("", mesg,
		"Cancel", "OK", NULL,
		B_WIDTH_FROM_WIDEST, B_WARNING_ALERT);
	alertPanel->SetShortcut(0, B_ESCAPE);
	
	/* ユーザの確認を待つ */
	isOK = (alertPanel->Go() == 1);
	
	return isOK;
}


/*
 * End of File
 */
