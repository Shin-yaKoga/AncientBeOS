/************************************************************************
 * $Id$
 * Copyright (c) 1997, Fort Gunnison, Inc.
 *
 * KGCircle.cp: KGCircleクラスの実装
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 13, 1997
 * Last update:	Dec. 13, 1997
 ************************************************************************
 */
/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include "KGCircle.h"
#include <support/Debug.h>


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

/* KGCircleクラスの公開メソッド */
/*
 * 初期化と解放; KGCircle
 */
KGCircle::KGCircle (BPoint center, float radius)
{
	ASSERT(radius > 0);

	fCenter = center;
	fRadius = radius;
}

/*
 * 描画処理用; KGCircle
 */
bool
KGCircle::Contains (BPoint thePoint) const
{
	float	distance;
	
	distance = sqrt(
		pow(thePoint.x - fCenter.x, 2) + pow(thePoint.y - fCenter.y, 2)
	);

	return (distance <= fRadius);
}

void
KGCircle::DrawOn (BView* inView) const
{
	inView->FillEllipse(fCenter, fRadius, fRadius);
	return;
}


/*
 * End of File
 */
