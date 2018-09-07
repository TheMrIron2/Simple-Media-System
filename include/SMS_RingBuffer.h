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
#ifndef __SMS_RingBuffer_H
#define __SMS_RingBuffer_H

#include <kernel.h>

typedef struct SMS_RingBuffer {

 unsigned char* m_pInp;
 unsigned char* m_pOut;
 unsigned char* m_pBeg;
 unsigned char* m_pEnd;
 unsigned char* m_pJmp;
 void*          m_pPtr;
 int            m_Size;
 int            m_Capacity;
 int            m_fWait;
 void ( *WaitCB ) ( struct SMS_RingBuffer* );
 int            m_Sema[ 3 ];
 int            m_nRef;
 void*          m_pBlockCBParam;
 void*          m_pUserCBParam;
 void*          m_pWaitCBParam;

 void ( *BlockCB ) ( struct SMS_RingBuffer* );
 void ( *UserCB  ) ( struct SMS_RingBuffer* );

} SMS_RingBuffer;

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

SMS_RingBuffer* SMS_RingBufferInit    ( void*, int           );
SMS_RingBuffer* SMS_RingBufferAddRef  ( SMS_RingBuffer*      );
void*           SMS_RingBufferAlloc   ( SMS_RingBuffer*, int );
void            SMS_RingBufferUnalloc ( SMS_RingBuffer*, int );
void            SMS_RingBufferFree    ( SMS_RingBuffer*, int );
void            SMS_RingBufferReset   ( SMS_RingBuffer*      );
void            SMS_RingBufferDestroy ( SMS_RingBuffer*      );
void*           SMS_RingBufferWait    ( SMS_RingBuffer*      );
int             SMS_RingBufferCount   ( SMS_RingBuffer*      );

static void inline SMS_RingBufferPost ( SMS_RingBuffer* apRB ) {
 SignalSema ( apRB -> m_Sema[ 1 ] );
}
#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __SMS_RingBuffer_H */
