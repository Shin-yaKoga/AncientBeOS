/***********************************************************************
 * $Id$
 * Copyright (c) 1998 by Fort Gunnison, Inc.
 * 
 * telnet_and_ftp.h: telnetおよびFTP準拠プロトコル用の定数
 *
 * Author:		Shin'ya Koga (koga@ftgun.co.jp)
 * Created:		May. 21, 1998
 * Last update:	May. 21, 1998
 ************************************************************************
*/

#ifndef _TELNET_AND_FTP_H_
#define _TELNET_AND_FTP_H_

/*
 * 以下の定数定義は、BSD系のftpコマンドのソースに書かれているものと
 * 同じである。BSDヘッダファイルをそのまま収録するのは再配布上問題が
 * あるため、内容だけを拝借した。('98. 5/21, koga@ftgun.co.jp)
 */
/* FTPの返答コード */
#define PRELIM		1	/* positive preliminary */
#define COMPLETE	2	/* positive completion */
#define CONTINUE	3	/* positive intermediate */
#define TRANSIENT	4	/* transient negative completion */
#define ERROR		5	/* permanent negative completion */

/* telnetの制御コード */
#define	IAC		255		/* interpret as command: */
#define	DONT	254		/* you are not to use option */
#define	DO		253		/* please, you use option */
#define	WONT	252		/* I won't use option */
#define	WILL	251		/* I will use option */
#define	SB		250		/* interpret as subnegotiation */
#define	GA		249		/* you may reverse the line */
#define	EL		248		/* erase the current line */
#define	EC		247		/* erase the current character */
#define	AYT		246		/* are you there */
#define	AO		245		/* abort output--but let prog finish */
#define	IP		244		/* interrupt process--permanently */
#define	BREAK	243		/* break */
#define	DM		242		/* data mark--for connect. cleaning */
#define	NOP		241		/* nop */
#define	SE		240		/* end sub negotiation */
#define EOR     239		/* end of record (transparent mode) */


#endif  /* _TELNET_AND_FTP_H_ */

/*
 * End of File
 */
