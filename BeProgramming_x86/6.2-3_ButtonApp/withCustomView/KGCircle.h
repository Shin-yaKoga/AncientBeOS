/***********************************************************************
 * $Id$
 * Copyright (c) 1997 by Fort Gunnison, Inc.
 *
 * KGCircle.h: 円形の描画用クラス
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		Dec. 13, 1997
 * Last update: Dec. 13, 1997
 ************************************************************************
 */

#ifndef _KG_CIRCLE_H_
#define _KG_CIRCLE_H_

/*      #########################################################       */
/*      #               I N C L U D E   F I L E S               #       */
/*      #########################################################       */

#include <interface/View.h>


/*      #########################################################       */
/*      #               T Y P E   D E F I N E S                 #       */
/*      #########################################################       */

/*      #########################################################       */
/*      #             P U B L I C   F U N C T I O N S           #       */
/*      #########################################################       */

/*
 * KGCircleクラスの定義
 */
class KGCircle {
// メソッド
public:
	// 初期化と解放
	KGCircle(BPoint center, float radius);
	~KGCircle(void) {  /* do nothing */ };
	
	// 描画処理用
	bool	Contains(BPoint thePoint) const;
	void	DrawOn(BView* inView) const;

// データメンバ
private:
	BPoint	fCenter;	/* 中心点の座標 */
	float	fRadius;	/* 半径 */
};


#endif  /* _KG_CIRCLE_H_ */

/*
 * End of File
 */
