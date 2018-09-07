/*
#     ___  _ _      ___
#    |    | | |    |
# ___|    |   | ___|    PS2DEV Open Source Project.
#----------------------------------------------------------
# Based on ffmpeg project (no copyright notes in the original source code)
# (c) 2005-2007 Eugene Plotnikov <e-plotnikov@operamail.com>
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
*/
#ifndef __SMS_Codec_H
#define __SMS_Codec_H

#ifndef __SMS_H
#include "SMS.h"
#endif  /* __SMS_H */

#ifndef __SMS_RingBuffer_H
#include "SMS_RingBuffer.h"
#endif  /* __SMS_RingBuffer_H */

#define SMS_CODEC_FLAG_LOW_DELAY 0x00080000
#define SMS_CODEC_FLAG_NOCSC     0x00000001
#define SMS_CODEC_FLAG_UNCACHED  0x20000000
#define SMS_CODEC_FLAG_IPU       0x00000002

typedef enum SMS_CodecType {
 SMS_CodecTypeUnknown = -1, SMS_CodecTypeVideo, SMS_CodecTypeAudio
} SMS_CodecType;

typedef enum SMS_CodecID {
 SMS_CodecID_NULL,   SMS_CodecID_MPEG4,   SMS_CodecID_MSMPEG4V3,
 SMS_CodecID_MPEG1,  SMS_CodecID_MPEG2,
 SMS_CodecID_MP2,    SMS_CodecID_MP3,     SMS_CodecID_AC3,
 SMS_CodecID_DTS,    SMS_CodecID_OGGV,    SMS_CodecID_WMA_V1,
 SMS_CodecID_WMA_V2, SMS_CodecID_PCM16LE, SMS_CodecID_PCM16BE,
 SMS_CodecID_AAC,    SMS_CodecID_FLAC,    SMS_CodecID_DXSB
} SMS_CodecID;

typedef enum SMS_CodecHWCtl {
 SMS_HWC_Init, SMS_HWC_Destroy, SMS_HWC_Suspend, SMS_HWC_Resume, SMS_HWC_Reset
} SMS_CodecHWCtl;

typedef struct SMS_CodecTag {

 SMS_CodecID m_ID;
 uint32_t    m_Tag;

} SMS_CodecTag;

struct SMS_CodecContext;
struct SMS_Frame;

typedef struct SMS_Codec {

 const char* m_pName;
 void*       m_pCtx;
 int32_t     ( *Init    ) ( struct SMS_CodecContext*                                   );
 int32_t     ( *Decode  ) ( struct SMS_CodecContext*, SMS_RingBuffer*, SMS_RingBuffer* );
 void        ( *Destroy ) ( struct SMS_CodecContext*                                   );

} SMS_Codec;

typedef struct SMS_CodecContext {

 uint32_t      m_FrameRate;
 uint32_t      m_FrameRateBase;
 uint32_t      m_FrameSize;
 int32_t       m_Width;
 int32_t       m_Height;
 int32_t       m_SampleRate;
 int32_t       m_BitRate;
 SMS_CodecType m_Type;
 SMS_CodecID   m_ID;
 uint32_t      m_Tag;
 uint32_t      m_Flags;
 void*         m_pIntBuf;
 SMS_Codec*    m_pCodec;
 void*         m_pUserData;
 short         m_UserDataLen;
 short         m_Channels;
 short         m_BitsPerSample;
 short         m_BlockAlign;
 uint8_t       m_fWS;

 void ( *HWCtl ) ( struct SMS_CodecContext*, SMS_CodecHWCtl );
 void*         m_pHWData;

} SMS_CodecContext;

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

SMS_CodecID SMS_CodecGetID         ( SMS_CodecType, uint32_t              );
int         SMS_CodecOpen          ( SMS_CodecContext*                    );
void        SMS_CodecClose         ( SMS_CodecContext*                    );
void        SMS_CodecGetBuffer     ( SMS_CodecContext*, struct SMS_Frame* );
void        SMS_CodecReleaseBuffer ( SMS_CodecContext*, struct SMS_Frame* );
void        SMS_CodecDestroy       ( SMS_CodecContext*                    );

#ifdef __cplusplus
};
#endif  /* __cplusplus */
#endif  /* __SMS_Codec_H */
