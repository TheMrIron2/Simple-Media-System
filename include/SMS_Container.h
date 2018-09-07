/*
#     ___  _ _      ___
#    |    | | |    |
# ___|    |   | ___|    PS2DEV Open Source Project.
#----------------------------------------------------------
# (c) 2005-2007 Eugene Plotnikov <e-plotnikov@operamail.com>
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
*/
#ifndef __SMS_Container_H
#define __SMS_Container_H

#ifndef __SMS_H
#include "SMS.h"
#endif  /* __SMS_H */

#ifndef __SMS_FileContext_H
#include "SMS_FileContext.h"
#endif  /* __SMS_FileContext_H */

#ifndef __SMS_Codec_H
#include "SMS_Codec.h"
#endif  /* __SMS_Codec_H */

#ifndef __SMS_RingBuffer_H
#include "SMS_RingBuffer.h"
#endif  /* __SMS_RingBuffer_H */

struct SMS_List;
struct SMS_ListNode;

#define SMS_CONTAINER_AVI     0
#define SMS_CONTAINER_ASF     1
#define SMS_CONTAINER_OGG     2
#define SMS_CONTAINER_M4A     3
#define SMS_CONTAINER_FLAC    4
#define SMS_CONTAINER_AAC     5
#define SMS_CONTAINER_MP3     6
#define SMS_CONTAINER_AC3     7
#define SMS_CONTAINER_MPEG_PS 8
#define SMS_CONTAINER_JPG     9
#define SMS_CONTAINER_M3U    10

#define SMS_CONT_FLAGS_SEEKABLE 0x00000001

#define SMS_STRM_FLAGS_AUDIO 0x00000001
#define SMS_STRM_FLAGS_VIDEO 0x00000002
#define SMS_STRM_FLAGS_SUBTL 0x00000004

#define SMS_MAX_STREAMS 8

typedef struct SMS_Stream {

 SMS_Rational      m_TimeBase;
 int64_t           m_CurDTS;
 int64_t           m_Duration;
 int32_t           m_SampleRate;
 uint32_t          m_RealFrameRate;
 uint32_t          m_RealFrameRateBase;
 uint32_t          m_Flags;
 SMS_CodecContext* m_pCodec;
 char*             m_pName;
 uint32_t          m_ID;
 void*             m_pCtx;
 SMS_RingBuffer*   m_pPktBuf;

 void ( *Destroy ) ( struct SMS_Stream* );

} SMS_Stream;

typedef struct SMS_Container {

 SMS_Stream*          m_pStm[ SMS_MAX_STREAMS ];
 int64_t              m_Duration;
 int64_t              m_StartTime;
 uint32_t             m_nStm;
 uint32_t             m_Flags;
 FileContext*         m_pFileCtx;
 char*                m_pName;
 void*                m_pCtx;
 struct SMS_List*     m_pPlayList;
 struct SMS_ListNode* m_pPlayItem;
 int                  m_DefPackSize;
 int                  m_DefPackIdx;

 SMS_AVPacket* ( *AllocPacket ) ( SMS_RingBuffer*, int                      );
 int           ( *ReadPacket  ) ( struct SMS_Container*, int*               );
 void          ( *Destroy     ) ( struct SMS_Container*, int                );
 int           ( *Seek        ) ( struct SMS_Container*, int, int, uint32_t );

} SMS_Container;

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

SMS_Container* SMS_GetContainer           ( FileContext*, int                    );
void           SMS_DestroyContainer       ( SMS_Container*, int                  );
void           SMSContainer_SetPTSInfo    ( SMS_Stream*, int, int                );
void           SMSContainer_CalcPktFields ( SMS_Stream*, SMS_AVPacket*           );
int            SMSContainer_SetName       ( SMS_Container*, FileContext*         );
int            SMSContainer_DefReadPacket ( SMS_Container*, int*                 );
void           SMSContainer_GetWAVHeader  ( FileContext*, SMS_CodecContext*, int );
int            SMSContainer_SkipID3       ( FileContext*                         );

#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __SMS_Container_H */
