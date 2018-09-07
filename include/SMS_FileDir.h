/*
#     ___  _ _      ___
#    |    | | |    |
# ___|    |   | ___|    PS2DEV Open Source Project.
#----------------------------------------------------------
# (c) 2006/7 Eugene Plotnikov <e-plotnikov@operamail.com>
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
*/
#ifndef __SMS_FileDir_H
#define __SMS_FileDir_H

#ifndef __SMS_List_H
#include "SMS_List.h"
#endif  /* __SMS_List_H */

struct CDDAContext;

extern unsigned char g_pUSB  [] __attribute__(   (  aligned( 4 ), section( ".data" )  )   );
extern unsigned char g_pCDROM[] __attribute__(   (  aligned( 4 ), section( ".data" )  )   );
extern unsigned char g_pCDDA [] __attribute__(   (  aligned( 4 ), section( ".data" )  )   );
extern unsigned char g_pHOST [] __attribute__(   (  aligned( 4 ), section( ".data" )  )   );
extern unsigned char g_pDVD  [] __attribute__(   (  aligned( 4 ), section( ".data" )  )   );
extern unsigned char g_pSMB  [] __attribute__(   (  aligned( 4 ), section( ".data" )  )   );
extern unsigned char g_pSMBS [] __attribute__(   (  aligned( 4 ), section( ".data" )  )   );
extern unsigned char g_HDDWD [] __attribute__(   (  aligned( 1 ), section( ".bss"  )  )   );

extern unsigned char* g_pDevName[ 7 ];

extern SMS_List*           g_pFileList;
extern int                 g_CMedia;
extern int                 g_CUnit;
extern int                 g_PD;
extern int                 g_SMBU;
extern struct CDDAContext* g_pCDDACtx;

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

void SMS_FileDirInit ( unsigned char* );

#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __SMS_FileDir_H */
