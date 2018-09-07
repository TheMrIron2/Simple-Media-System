/*
#     ___  _ _      ___
#    |    | | |    |
# ___|    |   | ___|    PS2DEV Open Source Project.
#----------------------------------------------------------
# Copyright (c) 2001 Fabrice Bellard.
#               2005 - Adopted for SMS by Eugene Plotnikov
# Licensed (like the original ffmpeg source code) under the terms of the
# GNU Lesser General Public License as published by the Free Software Foundation;
# either version 2 of the License, or (at your option) any later version.
# 
*/
#include "SMS_ContainerAVI.h"
#include "SMS_FourCC.h"
#include "SMS_GUI.h"
#include "SMS_GUIClock.h"
#include "SMS_Locale.h"
#include "SMS_Config.h"

#include <malloc.h>
#include <stdio.h>

#define MYCONT( c ) (  ( _AVIContainer* )c -> m_pCtx  )
#define MYSTRM( s ) (  ( _AVIStream*    )s -> m_pCtx  )

#define AVIIF_INDEX        0x10
#define AVIF_ISINTERLEAVED 0x00000100

typedef struct _AVIdxEntry {

 uint32_t m_Pos    __attribute__(  ( packed )  );
 uint32_t m_CumLen __attribute__(  ( packed )  );
 uint32_t m_fKey   __attribute__(  ( packed )  );

} _AVIdxEntry __attribute__(  ( packed )  );

typedef struct _AVIStream {

 int64_t      m_StartTime;
 _AVIdxEntry* m_pIdx;
 uint32_t     m_IdxAllocSize;
 uint32_t     m_nIdx;
 int32_t      m_Idx;
 int32_t      m_Rate;
 int32_t      m_Scale;
 int32_t      m_PTSWrapBits;
 int32_t      m_SampleSize;
 uint32_t     m_CumLen;
 uint32_t     m_FrameOffset;
 uint32_t     m_NewFrameOffset;

} _AVIStream;

typedef struct _AVIContainer {

 int64_t  m_StartTime;
 uint32_t m_BitRate;
 uint32_t m_RiffEnd;
 uint32_t m_MoviEnd;
 uint32_t m_MoviList;
 uint32_t m_nChunks;
 uint32_t m_RiffEndList [ 8 ];
 uint32_t m_MoviEndList [ 8 ];
 uint32_t m_MoviListList[ 8 ];
 char     m_fODML;
 char     m_fOffsetBug;

} _AVIContainer;

static int _ProbeFile ( FileContext* apFileCtx, uint32_t* apRiffEnd ) {

 int      retVal;
 uint32_t lTag = File_GetUInt ( apFileCtx );

 if (  lTag == SMS_MKTAG( 'R', 'I', 'F', 'F' )  ) {

  *apRiffEnd  = File_GetUInt ( apFileCtx );
  *apRiffEnd += apFileCtx -> m_CurPos;
  lTag        = File_GetUInt ( apFileCtx );

  retVal = lTag == SMS_MKTAG( 'A', 'V', 'I', ' ' ) ||
           lTag == SMS_MKTAG( 'A', 'V', 'I', 'X' );

 } else retVal = 0;

 return retVal;

}  /* end _ProbeFile */

static void _DestroyStream ( SMS_Stream* apStm ) {

 if (  MYSTRM( apStm ) -> m_pIdx  ) free (  MYSTRM( apStm ) -> m_pIdx  );

}  /* end _DestroyStream */

static SMS_Stream* _NewStream ( SMS_Container* apCont ) {

 SMS_Stream* retVal = NULL;

 if ( apCont -> m_nStm < SMS_MAX_STREAMS ) {

  retVal = ( SMS_Stream* )calloc (  1, sizeof( SMS_Stream ) + sizeof ( _AVIStream )  );

  if ( retVal ) {

   retVal -> m_pCtx   = ( char* )retVal + sizeof( SMS_Stream );
   retVal -> m_pCodec = ( SMS_CodecContext* )calloc (  1, sizeof ( SMS_CodecContext )  );

   MYSTRM( retVal ) -> m_Idx       = apCont -> m_nStm;
   MYSTRM( retVal ) -> m_StartTime =
   retVal -> m_Duration =
   retVal -> m_CurDTS   = SMS_NOPTS_VALUE;

   retVal -> Destroy = _DestroyStream;

   apCont -> m_pStm[ apCont -> m_nStm++ ] = retVal;

  }  /* end if */

 }  /* end if */

 return retVal;

}  /* end _NewStream */

static int _Load_idx1 ( SMS_Container* apCtx, int aSize ) {

 int          i, lnIdx = aSize / 16;
 uint32_t     lIdx, lTag, lFlags, lPos, lLen, lPerc;
 SMS_Stream*  lpBaseStm;
 _AVIStream*  lpStm;
 _AVIdxEntry* lpIndices;
 _AVIdxEntry* lpIndex;
 FileContext* lpFileCtx    = apCtx -> m_pFileCtx;
 int          lPrevIdx     = -1;
 int          lnSameIdx    =  1;
 int          lnMaxSameIdx = -1;

 if ( lnIdx <= 0 ) return 0;

 lpFileCtx -> Stream ( lpFileCtx, lpFileCtx -> m_CurPos, 64 );

 SMS_GUIClockSuspend ();
 GUI_Progress ( STR_LOADING_INDICES.m_pStr, 0, 0 );

 lPerc = lnIdx / 100 + 1;

 for ( i = 0; i < lnIdx; ++i ) {

  lTag   = File_GetUInt ( lpFileCtx );
  lFlags = File_GetUInt ( lpFileCtx );
  lPos   = File_GetUInt ( lpFileCtx );
  lLen   = File_GetUInt ( lpFileCtx );

  lIdx  = (  ( lTag        & 0xFF  ) - '0'  ) * 10;
  lIdx += (  ( lTag >> 8 ) & 0xFF  ) - '0';

  if (  !( i % lPerc )  ) {

   float lPos = ( float )i / ( float ) lnIdx * 100.0F;

   GUI_Progress (  STR_LOADING_INDICES.m_pStr, ( unsigned int )( lPos + 0.5F ), 0  );

  }  /* end if */

  if ( lIdx >= apCtx -> m_nStm ) continue;

  lpBaseStm = apCtx -> m_pStm[ lIdx ];

  if ( lIdx == lPrevIdx )

   lnSameIdx += 1;

  else {

   if ( lnMaxSameIdx < lnSameIdx && !( apCtx -> m_pStm[ lPrevIdx ] -> m_Flags & SMS_STRM_FLAGS_AUDIO ) ) lnMaxSameIdx = lnSameIdx;

   lnSameIdx = 1;
   lPrevIdx  = lIdx;

  }  /* end else */

  lpStm = MYSTRM( apCtx -> m_pStm[ lIdx ] );

  lpIndices = SMS_Realloc (
   lpStm -> m_pIdx, &lpStm -> m_IdxAllocSize,
   ( lpStm -> m_nIdx + 1 ) * sizeof ( _AVIdxEntry )
  );

  lpStm -> m_pIdx = lpIndices;

  lpIndex = &lpIndices[ lpStm -> m_nIdx++ ];
  lpIndex -> m_fKey   = ( lFlags & AVIIF_INDEX ) != 0;
  lpIndex -> m_Pos    = lPos;
  lpIndex -> m_CumLen = lpStm -> m_CumLen;

  lpStm -> m_CumLen += lLen;

 }  /* end for */

 if ( lnMaxSameIdx < lnSameIdx ) lnMaxSameIdx = lnSameIdx;

 lpFileCtx -> Stream ( lpFileCtx, lpFileCtx -> m_CurPos, 0 );

 GUI_UpdateStatus ();
 SMS_GUIClockResume ();

 return 1;//lnMaxSameIdx > 256 ? -1 : 1;

}  /* end _Load_idx1 */

static int _LoadIndex ( SMS_Container* apCont ) {

 uint32_t     lTag, lSize;
 uint32_t     lPos = apCont -> m_pFileCtx -> m_CurPos;
 FileContext* lpFileCtx = apCont -> m_pFileCtx;
 int          retVal    = 0;

 lpFileCtx -> Seek (  lpFileCtx, MYCONT( apCont ) -> m_MoviEnd  );

 while ( 1 )

  if ( lpFileCtx -> m_CurPos < lpFileCtx -> m_Size ) {

   lTag  = File_GetUInt ( lpFileCtx );
   lSize = File_GetUInt ( lpFileCtx );

   if (   lTag == SMS_MKTAG( 'i', 'd', 'x', '1' ) && (  retVal = _Load_idx1 ( apCont, lSize )  )   ) {
    apCont -> m_Flags |= SMS_CONT_FLAGS_SEEKABLE;
    break;
   }  /* end if */

   lSize += ( lSize & 1 );
   lpFileCtx -> Seek ( lpFileCtx, lpFileCtx -> m_CurPos + lSize );

  } else break;

 lpFileCtx -> Seek ( lpFileCtx, lPos );

 return retVal >= 0 ? 1 : -1;

}  /* end _LoadIndex */

static int _LoadODMLIdx ( SMS_Container* apCont, int* apNum ) {

 FileContext* lpFileCtx = apCont -> m_pFileCtx;
 int          lPreEntry = File_GetUShort( lpFileCtx );
 int          lSubType  = File_GetByte  ( lpFileCtx );
 int          lType     = File_GetByte  ( lpFileCtx );
 int          lnEntries = File_GetUInt  ( lpFileCtx );
 int          i, lCID   = File_GetUInt  ( lpFileCtx );
 int          lStID     = 10 * (  ( lCID & 0xFF ) - '0'  ) + (   (  ( lCID >> 8 ) & 0xFF  ) - '0'   );
 int64_t      lBase     = File_GetULong ( lpFileCtx );
 int64_t      lLastPos  = -1;
 int64_t      lFileSize = lpFileCtx -> m_Size;
 SMS_Stream*  lpStm;
 _AVIStream*  lpAVIStm;
 _AVIdxEntry* lpIndices;
 _AVIdxEntry* lpIndex;

 if (  ( uint32_t )lStID > apCont -> m_nStm || lStID < 0  ) return 0;

 lpStm = apCont -> m_pStm[ lStID ];

 if ( lSubType ) return 0;

 File_GetUInt ( lpFileCtx );

 if ( lType && lPreEntry != 2 ) return 0;
 if ( lType > 1               ) return 0;

 if ( lBase >= lFileSize ) {

  if (   lBase >> 32 == ( lBase & 0xFFFFFFFF ) &&
       ( lBase & 0xFFFFFFFF ) < lFileSize      && lFileSize <= 0xFFFFFFFF
  )
   lBase &= 0xFFFFFFFF;
  else return 0;

 }  /* end if */

 lpAVIStm = MYSTRM( lpStm );

 if ( lType ) {

  char lBuf[ 256 ];
  sprintf ( lBuf, STR_LOADING_ODML.m_pStr, apNum[ 0 ] );
  GUI_Status ( lBuf );

 }  /* end if */

 for ( i = 0; i < lnEntries; ++i ) {

  if ( lType ) {
   int64_t lPos = File_GetUInt ( lpFileCtx ) + lBase - 8;
   int     lLen = File_GetUInt ( lpFileCtx );
   int     lKey = lLen >= 0;
   lLen &= 0x7FFFFFFF;
   lpIndices = SMS_Realloc (
    lpAVIStm -> m_pIdx, &lpAVIStm -> m_IdxAllocSize,
    ( lpAVIStm -> m_nIdx + 1 ) * sizeof ( _AVIdxEntry )
   );
   lpAVIStm -> m_pIdx = lpIndices;
   lpIndex = &lpIndices[ lpAVIStm -> m_nIdx++ ];
   lpIndex -> m_fKey   = lKey;
   lpIndex -> m_Pos    = ( uint32_t )lPos;
   lpIndex -> m_CumLen = lpAVIStm -> m_CumLen;
   lpAVIStm -> m_CumLen  += lLen;
   lLastPos = lPos;
  } else {
   int64_t lOffset, lPos;
   lOffset = File_GetULong ( lpFileCtx );
   File_GetUInt ( lpFileCtx );
   File_GetUInt ( lpFileCtx );
   lPos = lpFileCtx -> m_CurPos;
   lpFileCtx -> Seek (  lpFileCtx, ( uint32_t )lOffset + 8  );
   _LoadODMLIdx ( apCont, apNum );
   ++apNum[ 0 ];
   lpFileCtx -> Seek (  lpFileCtx, ( uint32_t )lPos  );
  }  /* end else */

 }  /* end for */

 MYCONT( apCont ) -> m_fODML = 1;

 return 1;

}  /* end _LoadODMLIdx */

static void _FillChunkList ( SMS_Container* apCont ) {

 _AVIContainer* lpCont    = MYCONT( apCont ); 
 FileContext*   lpFileCtx = apCont -> m_pFileCtx;
 uint32_t       i, lPos   = lpFileCtx -> m_CurPos;

 lpCont -> m_nChunks           = 1;
 lpCont -> m_RiffEndList [ 0 ] = lpCont -> m_RiffEnd;
 lpCont -> m_MoviEndList [ 0 ] = lpCont -> m_MoviEnd;
 lpCont -> m_MoviListList[ 0 ] = lpCont -> m_MoviList;

 for ( i = 1; i < 8; ++i ) {

  uint32_t lTag;
  uint32_t lSize;
  uint32_t lRIFFEnd;

  lpFileCtx -> Seek ( lpFileCtx, lpCont -> m_RiffEndList[ i - 1 ] );

  if (  !_ProbeFile ( lpFileCtx, &lRIFFEnd )  ) break;

  lTag  = File_GetUInt ( lpFileCtx );
  lSize = File_GetUInt ( lpFileCtx );

  if (  lTag != SMS_MKTAG( 'L', 'I', 'S', 'T' )  ) break;

  lTag = File_GetUInt ( lpFileCtx );

  if (  lTag != SMS_MKTAG( 'm', 'o', 'v', 'i' )  ) break;

  lpCont -> m_nChunks          += 1;
  lpCont -> m_RiffEndList [ i ] = lRIFFEnd;
  lpCont -> m_MoviEndList [ i ] = lpFileCtx -> m_CurPos - 4 + lSize;
  lpCont -> m_MoviListList[ i ] = lpFileCtx -> m_CurPos - 4;

 }  /* end for */

 lpFileCtx -> Seek ( lpFileCtx, lPos );

}  /* end _FillChunkList */

static void _CheckOffsetBug ( SMS_Container* apCont ) {

 _AVIContainer* lpCont    = MYCONT( apCont ); 
 FileContext*   lpFileCtx = apCont -> m_pFileCtx;
  _AVIStream*   lpStm     = MYSTRM( apCont -> m_pStm[ 0 ] );

 if ( lpStm -> m_nIdx ) {

  char     lBuf[ 2 ];
  uint32_t lPos  = lpFileCtx -> m_CurPos;

  lpFileCtx -> Seek ( lpFileCtx, lpStm -> m_pIdx[ 0 ].m_Pos );
  lpFileCtx -> Read ( lpFileCtx, lBuf, 2 );
  lpFileCtx -> Seek ( lpFileCtx, lPos );

  if ( lBuf[ 0 ] == '0' || lBuf[ 1 ] == '0' ) lpCont -> m_fOffsetBug = 1;

 }  /* end if */

}  /* end _CheckOffsetBug */

static int _ReadHeader ( SMS_Container* apCtx ) {

 int                retVal;
 uint32_t           lTag, lSubTag, lSize, lnFrames, lLen, lStmIdx, lFlags = 0;
 int32_t            lFramePeriod = 0, lBitRate;
 int32_t            i, lCount, lScale, lRate, lODMLIdx = 1;
 SMS_Stream*        lpStm;
 FileContext*       lpFileCtx  = apCtx -> m_pFileCtx;
 enum SMS_CodecType lCodecType = SMS_CodecTypeUnknown;

 retVal  =  1;
 lStmIdx = -1;

 while ( 1 )

  if ( lpFileCtx -> m_CurPos < lpFileCtx -> m_Size ) {

   lTag  = File_GetUInt ( lpFileCtx );
   lSize = File_GetUInt ( lpFileCtx );

   switch ( lTag ) {

    case SMS_MKTAG( 'L', 'I', 'S', 'T' ):

     lSubTag = File_GetUInt ( lpFileCtx );

     if (  lSubTag == SMS_MKTAG( 'm', 'o', 'v', 'i' )  ) {

      MYCONT( apCtx ) -> m_MoviList = lpFileCtx -> Seek ( lpFileCtx, lpFileCtx -> m_CurPos - 4 );
      MYCONT( apCtx ) -> m_MoviEnd  = MYCONT( apCtx ) -> m_MoviList + lSize;

      goto endOfHeader;

     }  /* end if */

    break;

    case SMS_MKTAG( 'a', 'v', 'i', 'h' ):  /* Main AVI header */

     lFramePeriod = File_GetUInt ( lpFileCtx );      /* dwMicroSecPerFrame */
     lBitRate     = File_GetUInt ( lpFileCtx ) * 8;  /* dwMaxBytesPerSec   */
                    File_GetUInt ( lpFileCtx );      /* dwReserved1        */
     lFlags       = File_GetUInt ( lpFileCtx );      /* dwFlags            */
                    File_GetUInt ( lpFileCtx );      /* dwTotalFrames      */
                    File_GetUInt ( lpFileCtx );      /* dwInitialFrames    */
     lCount       = File_GetUInt ( lpFileCtx );      /* dwStreams          */

     for ( i = 0; i < lCount; ++i )

      if (  !_NewStream ( apCtx )  ) goto error;

     lpFileCtx -> Seek ( lpFileCtx, lpFileCtx -> m_CurPos + lSize - 28 );

    break;

    case SMS_MKTAG( 's', 't', 'r', 'h' ):

     ++lStmIdx;

     lTag    = File_GetUInt ( lpFileCtx );  /* fccType    */
     lSubTag = File_GetUInt ( lpFileCtx );  /* fccHandler */

     switch ( lTag ) {

      case SMS_MKTAG( 'v', 'i', 'd', 's' ):

       lCodecType = SMS_CodecTypeVideo;

       if ( lStmIdx >= apCtx -> m_nStm )

        lpFileCtx -> Seek ( lpFileCtx, lpFileCtx -> m_CurPos + lSize - 8 );

       else {

        lpStm = apCtx -> m_pStm[ lStmIdx ];

        lpStm -> m_Flags |= SMS_STRM_FLAGS_VIDEO;

        File_GetUInt   ( lpFileCtx );  /* dwFlags         */
        File_GetUShort ( lpFileCtx );  /* wPriority       */
        File_GetUShort ( lpFileCtx );  /* wLanguage       */
        File_GetUInt   ( lpFileCtx );  /* dwInitialFrames */

        lScale = File_GetUInt ( lpFileCtx );  /* dwScale */
        lRate  = File_GetUInt ( lpFileCtx );  /* dwRate  */

        if ( !lScale || !lRate ) {

         if ( lFramePeriod ) {

          lRate  = SMS_TIME_BASE;
          lScale = lFramePeriod;

         } else {

          lRate  = 25;
          lScale =  1;

         }  /* end else */

        }  /* end if */

        MYSTRM( lpStm ) -> m_Rate  = lRate;
        MYSTRM( lpStm ) -> m_Scale = lScale;

        SMSContainer_SetPTSInfo ( lpStm, lScale, lRate );

        lpStm -> m_pCodec -> m_FrameRate     = lRate;
        lpStm -> m_pCodec -> m_FrameRateBase = lScale;

                   File_GetUInt ( lpFileCtx );  /* dwStart  */
        lnFrames = File_GetUInt ( lpFileCtx );  /* dwLength */

        MYSTRM( lpStm ) -> m_StartTime = SMS_INT64( 0 );
        lpStm -> m_Duration  = SMS_Rescale (
         lnFrames, ( int64_t )lpStm -> m_pCodec -> m_FrameRateBase *
                   ( int64_t )SMS_TIME_BASE,
         lpStm -> m_pCodec -> m_FrameRate
        );
        lpFileCtx -> Seek ( lpFileCtx, lpFileCtx -> m_CurPos + lSize - 36 );

       }  /* end else */

      break;

      case SMS_MKTAG( 'a', 'u', 'd', 's' ): {

       lCodecType = SMS_CodecTypeAudio;

       if ( lStmIdx >= apCtx -> m_nStm )

        lpFileCtx -> Seek ( lpFileCtx, lpFileCtx -> m_CurPos + lSize - 8 );

       else {

        lpStm = apCtx -> m_pStm[ lStmIdx ];

        lpStm -> m_Flags |= SMS_STRM_FLAGS_AUDIO;

        File_GetUInt   ( lpFileCtx );  /* dwFlags         */
        File_GetUShort ( lpFileCtx );  /* wPriority       */
        File_GetUShort ( lpFileCtx );  /* wLanguage       */
        File_GetUInt   ( lpFileCtx );  /* dwInitialFrames */

        MYSTRM( lpStm ) -> m_Scale = File_GetUInt ( lpFileCtx );  /* dwScale */
        MYSTRM( lpStm ) -> m_Rate  = File_GetUInt ( lpFileCtx );  /* dwRate  */

        SMSContainer_SetPTSInfo (  lpStm, MYSTRM( lpStm ) -> m_Scale, MYSTRM( lpStm ) -> m_Rate  );

               File_GetUInt ( lpFileCtx );  /* dwStart  */
        lLen = File_GetUInt ( lpFileCtx );  /* dwLength */

        File_GetUInt ( lpFileCtx );  /* dwSuggestedBufferSize */
        File_GetUInt ( lpFileCtx );  /* dwQuality             */

        MYSTRM( lpStm ) -> m_SampleSize = File_GetUInt ( lpFileCtx );  /* dwSampleSize */
        MYSTRM( lpStm ) -> m_StartTime  = 0;

        if (  MYSTRM( lpStm ) -> m_Rate ) lpStm -> m_Duration = ( int64_t )lLen * SMS_TIME_BASE / MYSTRM( lpStm ) -> m_Rate;

        lpFileCtx -> Seek ( lpFileCtx, lpFileCtx -> m_CurPos + lSize - 48 );

       }  /* end else */

      } break;

      case SMS_MKTAG( 't', 'x', 't', 's' ):

       lCodecType = SMS_CodecTypeUnknown;
       lpFileCtx -> Seek ( lpFileCtx, lpFileCtx -> m_CurPos + lSize - 8 );

      break;

      default: goto error;

     }  /* end switch */

    break;

    case SMS_MKTAG( 's', 't', 'r', 'f' ):

    if ( lStmIdx >= apCtx -> m_nStm )

     lpFileCtx -> Seek ( lpFileCtx, lpFileCtx -> m_CurPos + lSize );

    else {

     SMS_CodecContext* lpCodecCtx = apCtx -> m_pStm[ lStmIdx ] -> m_pCodec;

     switch ( lCodecType ) {

      case SMS_CodecTypeVideo:

       File_GetUInt ( lpFileCtx );

       lpCodecCtx -> m_Width  = File_GetUInt ( lpFileCtx );
       lpCodecCtx -> m_Height = File_GetUInt ( lpFileCtx );

       File_GetUShort ( lpFileCtx );

       lpCodecCtx -> m_BitsPerSample = File_GetUShort ( lpFileCtx );

       lSubTag = File_GetUInt ( lpFileCtx );

       File_GetUInt ( lpFileCtx );
       File_GetUInt ( lpFileCtx );
       File_GetUInt ( lpFileCtx );
       File_GetUInt ( lpFileCtx );
       File_GetUInt ( lpFileCtx );

       lLen = lSize - 40;

       lpFileCtx -> Seek ( lpFileCtx, lpFileCtx -> m_CurPos + lLen );

       if ( lLen & 1 ) File_GetByte ( lpFileCtx );

       lpCodecCtx -> m_Type = SMS_CodecTypeVideo;
       lpCodecCtx -> m_Tag  = lSubTag;
       lpCodecCtx -> m_ID   = SMS_CodecGetID ( SMS_CodecTypeVideo, lSubTag );

      break;

      case SMS_CodecTypeAudio: {

       SMSContainer_GetWAVHeader ( lpFileCtx, lpCodecCtx, lSize );

       if ( lSize % 2 ) File_Skip ( lpFileCtx, 1 );

       lpStm = apCtx -> m_pStm[ lStmIdx ];

       if (  lpCodecCtx -> m_BlockAlign == 1 && MYSTRM( lpStm ) -> m_SampleSize == 0  ) MYSTRM( lpStm ) -> m_SampleSize = 1;

      } break;

      default:

       lpCodecCtx -> m_Type = SMS_CodecTypeUnknown;
       lpCodecCtx -> m_ID   = SMS_CodecID_NULL;
       lpCodecCtx -> m_Tag  = 0;

       lpFileCtx -> Seek ( lpFileCtx, lpFileCtx -> m_CurPos + lSize );

     }  /* end switch */

    }  /* end else */

    break;

    case SMS_MKTAG( 's', 't', 'r', 'n' ):

     if ( lStmIdx >= apCtx -> m_nStm )

      lpFileCtx -> Seek ( lpFileCtx, lpFileCtx -> m_CurPos + lSize );

     else {

      lpStm = apCtx -> m_pStm[ lStmIdx ];

      if ( lpStm -> m_pName ) free ( lpStm -> m_pName );

      lpStm -> m_pName = ( char* )malloc (  ( lSize += lSize & 1 ) + 1  );
      lpFileCtx -> Read ( lpFileCtx, lpStm -> m_pName, lSize );
      lpStm -> m_pName[ lSize ] = '\x00';

     }  /* end else */

    break;

    case SMS_MKTAG( 'I', 'A', 'S', '1' ):
    case SMS_MKTAG( 'I', 'A', 'S', '2' ):
    case SMS_MKTAG( 'I', 'A', 'S', '3' ):
    case SMS_MKTAG( 'I', 'A', 'S', '4' ):
    case SMS_MKTAG( 'I', 'A', 'S', '5' ):
    case SMS_MKTAG( 'I', 'A', 'S', '6' ):
    case SMS_MKTAG( 'I', 'A', 'S', '7' ):
    case SMS_MKTAG( 'I', 'A', 'S', '8' ):
    case SMS_MKTAG( 'I', 'A', 'S', '9' ): {

     char* lpBuf = ( char* )malloc (  ( lSize += lSize & 1 ) + 1  );
     int   lfSet = 0;

     lpFileCtx -> Read ( lpFileCtx, lpBuf, lSize );
     lpBuf[ lSize ] = '\x00';

     lSubTag = (  ( char* )&lTag  )[ 3 ] - '1';
     lLen    = 0;

     for ( lSize = 0; lSize < apCtx -> m_nStm; ++lSize ) {

      SMS_Stream* lpStm = apCtx -> m_pStm[ lSize ];

      if ( lpStm -> m_Flags & SMS_STRM_FLAGS_AUDIO ) {

       if ( !lSubTag ) {
        if ( lpStm -> m_pName ) free ( lpStm -> m_pName );
        lpStm -> m_pName = lpBuf;
        lfSet            = 1;
        break;

       } else --lSubTag;

      }  /* end if */

     }  /* end for */

     if ( !lfSet ) free ( lpBuf );

    } break;

    case SMS_MKTAG( 'i', 'n', 'd', 'x' ): {

     unsigned int lPos = apCtx -> m_pFileCtx -> m_CurPos;

     _LoadODMLIdx ( apCtx, &lODMLIdx );

     apCtx -> m_pFileCtx -> Seek (  apCtx -> m_pFileCtx, lPos + lSize + ( lSize & 1 )  );

    } break;

    default:

     lSize += ( lSize & 1 );
     lpFileCtx -> Seek ( lpFileCtx, lpFileCtx -> m_CurPos + lSize );

   }  /* end switch */

  } else goto error;
endOfHeader:
 if ( lStmIdx != apCtx -> m_nStm - 1 )
error: retVal = 0;
 if ( retVal ) {

  _FillChunkList ( apCtx );

  if (  MYCONT( apCtx ) -> m_fODML  ) {
   apCtx -> m_Flags |= SMS_CONT_FLAGS_SEEKABLE;
  } else retVal = _LoadIndex ( apCtx );

  _CheckOffsetBug ( apCtx );

 }  /* end if */

 return retVal;

}  /* end _ReadHeader */

static int _ReadPacket ( SMS_Container* apCont, int* apIdx ) {

 uint8_t        lData[ 8 ] __attribute__(   (  aligned( 8 )  )   );
 _AVIContainer* lpAVICont  = MYCONT( apCont );
 FileContext*   lpBuf      = apCont -> m_pFileCtx;
 uint32_t       i          = lpBuf -> m_CurPos;
 int32_t        lCount, lSize;
 _AVIStream*    lpAVIStm;
 uint16_t       lChunkID;
 uint16_t       lCond;

 __asm__(
  ".set noat\n\t"
  "pnor $at, $zero, $zero\n\t"
  "sd   $at, %0\n\t"
  ".set at\n\t"
  : "=m"( lData ) :: "at"
 );

 while ( lpBuf -> m_CurPos < lpBuf -> m_Size ) {

  if ( lpBuf -> m_CurPos >= lpAVICont -> m_MoviEnd ) {

   uint32_t lTag[ 2 ], lSize;

   File_Skip ( lpBuf, lpAVICont -> m_RiffEnd - lpBuf -> m_CurPos );

   if (  !_ProbeFile ( lpBuf, &lpAVICont -> m_RiffEnd )  ) return -1;

   lTag[ 0 ] = File_GetUInt ( lpBuf );
   lSize     = File_GetUInt ( lpBuf );
   lTag[ 1 ] = File_GetUInt ( lpBuf );

   if (  lTag[ 0 ] == SMS_MKTAG( 'L', 'I', 'S', 'T' ) &&
         lTag[ 1 ] == SMS_MKTAG( 'm', 'o', 'v', 'i' )
   )

    lpAVICont -> m_MoviEnd = lpBuf -> m_CurPos + lSize - 4; 

   else return -1;

  }  /* end if */

  lSize = File_GetByte ( lpBuf );

  __asm__(
   ".set noat\n\t"
   "ld      $at, %0\n\t"
   "dsll32  %2, %2, 24\n\t"
   "dsrl    $at, $at, 8\n\t"
   "or      $at, $at, %2\n\t"
   "sd      $at, %0\n\t"
   "dsrl32  %1, $at, 0\n\t"
   ".set at\n\t"
   : "=m"( lData ), "=r"( lSize ) : "r"( lSize ): "at"
  );

  lCount = (  ( int )lData[ 2 ] - '0' ) * 10 + (  ( int )lData[ 3 ] - '0'  );
  lCond  = (  lCount < ( int32_t )apCont -> m_nStm  ) + (  i + lSize <= ( uint32_t )lpAVICont -> m_MoviEnd  );

  if ( lData[ 2 ] >= '0' && lData[ 2 ] <= '9' &&
       lData[ 3 ] >= '0' && lData[ 3 ] <= '9' &&
       *( uint16_t* )&lData[ 0 ] == 0x7869    &&  /* ix */
       lCond
  ) File_Skip ( lpBuf, lSize );
            
  lCount   = (  ( int )lData[ 0 ] - '0'  ) * 10 + (  ( int )lData[ 1 ] - '0'  );
  lChunkID = *( uint16_t* )&lData[ 2 ];

  if ( lData[ 0 ] >= '0' && lData[ 0 ] <= '9' &&
       lData[ 1 ] >= '0' && lData[ 1 ] <= '9' &&
       (  lChunkID == 0x6364 ||  /* dc */
	      lChunkID == 0x6277 ||  /* wb */
          lChunkID == 0x6273 ||  /* sb */
          lChunkID == 0x6264 ||  /* db */
          lChunkID == 0x5F5F     /* __ */
       ) && lCond
  ) {

   SMS_Stream* lpStm = apCont -> m_pStm[ lCount ];

   if ( lpStm -> m_pPktBuf ) {

    SMS_AVPacket* lpPkt = apCont -> AllocPacket ( lpStm -> m_pPktBuf, lSize );

    if (  !lpPkt || lpBuf -> Read ( lpBuf, lpPkt -> m_pData, lSize ) != lSize  ) return 0;

    if ( lSize & 1 ) {

     File_GetByte ( lpBuf );
     ++lSize;

    }  /* end if */

    lpPkt -> m_StmIdx = lCount;

    if (  !( lpStm -> m_Flags & SMS_STRM_FLAGS_SUBTL )  ) {

     lpAVIStm = MYSTRM( lpStm );

     lpPkt -> m_DTS = lpAVIStm -> m_FrameOffset;

     if ( lpAVIStm -> m_SampleSize ) lpPkt -> m_DTS /= lpAVIStm -> m_SampleSize;

     if ( lpStm -> m_pCodec -> m_Type == SMS_CodecTypeVideo ) {

      if (  lpAVIStm -> m_FrameOffset < lpAVIStm -> m_nIdx &&
            lpAVIStm -> m_pIdx[ lpAVIStm -> m_FrameOffset ].m_fKey
      ) lpPkt -> m_Flags |= SMS_PKT_FLAG_KEY;

     } else lpPkt -> m_Flags |= SMS_PKT_FLAG_KEY;

     if ( lpAVIStm -> m_SampleSize )
      lpAVIStm -> m_FrameOffset += lpPkt -> m_Size;
     else ++lpAVIStm -> m_FrameOffset;

     SMSContainer_CalcPktFields ( lpStm, lpPkt );

    } else lpPkt -> m_Flags |= SMS_PKT_FLAG_SUB;

    *apIdx = lCount;

    return lSize;

   } else File_Skip ( lpBuf, lSize );

  }  /* end else */

 }  /* end while */

 return -1;

}  /* end _ReadPacket */

static int _HasTimings ( SMS_Container* apCont ) {

 int         retVal = 0;
 uint32_t    i;
 SMS_Stream* lpStm;

 for ( i = 0; i < apCont -> m_nStm; ++i ) {

  lpStm = apCont -> m_pStm[ i ];

  if (  MYSTRM( lpStm ) -> m_StartTime != SMS_NOPTS_VALUE &&
        lpStm -> m_Duration            != SMS_NOPTS_VALUE
  ) {

   retVal = 1;
   break;

  }  /* end if */

 }  /* end for */

 return retVal;

}  /* end _HasTimings */

static void _UpdateStmTimings ( SMS_Container* apCont ) {

 int64_t     lStartTime, lEndTime, lTmp;
 uint32_t    i;
 SMS_Stream* lpStm;

 lStartTime = SMS_MAXINT64;
 lEndTime   = SMS_MININT64;

 for ( i = 0; i < apCont -> m_nStm; ++i ) {

  lpStm = apCont -> m_pStm[ i ];

  if ( lpStm -> m_pCodec -> m_ID == SMS_CodecID_DXSB ) continue;

  if (  MYSTRM( lpStm ) -> m_StartTime != SMS_NOPTS_VALUE  ) {

   if (  MYSTRM( lpStm ) -> m_StartTime < lStartTime  ) lStartTime = MYSTRM( lpStm ) -> m_StartTime;

   if ( lpStm -> m_Duration != SMS_NOPTS_VALUE ) {

    lTmp = MYSTRM( lpStm ) -> m_StartTime + lpStm -> m_Duration;

    if ( lTmp > lEndTime ) lEndTime = lTmp;

   }  /* end if */

  }  /* end if */

 }  /* end for */

 if ( lStartTime != SMS_MAXINT64 ) {

  MYCONT( apCont ) -> m_StartTime = lStartTime;

  if ( lEndTime != SMS_MAXINT64 ) {

   apCont -> m_Duration = lEndTime - lStartTime;

   if ( apCont -> m_pFileCtx -> m_Size > 0 )

    MYCONT( apCont ) -> m_BitRate = ( uint32_t )(  ( float )apCont -> m_pFileCtx -> m_Size * 8.0F * ( float )SMS_TIME_BASE / ( float )apCont -> m_Duration  );

  }  /* end if */

 }  /* end if */

}  /* end _UpdateStmTimings */

static void _FillAllStmTimings ( SMS_Container* apCont ) {

 uint32_t    i;
 SMS_Stream* lpStm;

 _UpdateStmTimings ( apCont );

 for ( i = 0; i < apCont -> m_nStm; ++i ) {

  lpStm = apCont -> m_pStm[ i ];

  if (  MYSTRM( lpStm ) -> m_StartTime == SMS_NOPTS_VALUE ) {

   MYSTRM( lpStm ) -> m_StartTime = MYCONT( apCont ) -> m_StartTime;
   lpStm -> m_Duration            = apCont -> m_Duration;

  }  /* end if */

 }  /* end for */

}  /* end _FillAllStmTimings */

static void _EstimateTimingsFromBitRate ( SMS_Container* apCont ) {

 int64_t     lDuration;
 uint32_t    i, lBitRate;
 SMS_Stream* lpStm;

 if (  MYCONT( apCont ) -> m_BitRate == 0  ) {

  lBitRate = 0;

  for ( i = 0; i < apCont -> m_nStm; ++i ) {

   lpStm     = apCont -> m_pStm[ i ];
   lBitRate += lpStm -> m_pCodec -> m_BitRate;

  }  /* end for */

  MYCONT( apCont ) -> m_BitRate = lBitRate;

 }  /* end if */

 if (  apCont -> m_Duration == SMS_NOPTS_VALUE && 
       MYCONT( apCont ) -> m_BitRate  != 0     && 
       apCont -> m_pFileCtx -> m_Size  > 0
 )  {

  lDuration = ( int64_t )(   (  8.0F * ( float )SMS_TIME_BASE * ( float )apCont -> m_pFileCtx -> m_Size  ) / ( float )MYCONT( apCont ) -> m_BitRate   );

  for ( i = 0; i < apCont -> m_nStm; ++i ) {

   lpStm = apCont -> m_pStm[ i ];

   if (  MYSTRM( lpStm ) -> m_StartTime == SMS_NOPTS_VALUE ||
         lpStm -> m_Duration            == SMS_NOPTS_VALUE
   ) {

    MYSTRM( lpStm ) -> m_StartTime = 0;
    lpStm           -> m_Duration  = lDuration;

   }  /* end if */

  }  /* end for */

 }  /* end if */

}  /* end _EstimateTimingsFromBitRate */

static void _EstimateTimings ( SMS_Container* apCont ) {

 if (  _HasTimings ( apCont )  )

  _FillAllStmTimings ( apCont );

 else _EstimateTimingsFromBitRate ( apCont );

 _UpdateStmTimings ( apCont );

}  /* end _EstimateTimings */

static void _CalcFrameRate ( SMS_Container* apCont ) {

 uint32_t          i;
 SMS_Stream*       lpStm;
 SMS_CodecContext* lpCodecCtx;

 for ( i = 0; i < apCont -> m_nStm; ++i ) {

  lpStm      = apCont -> m_pStm[ i ];
  lpCodecCtx = lpStm -> m_pCodec;

  if ( lpCodecCtx -> m_Type == SMS_CodecTypeVideo ) {

   lpStm -> m_RealFrameRate     = lpCodecCtx -> m_FrameRate;
   lpStm -> m_RealFrameRateBase = lpCodecCtx -> m_FrameRateBase;

  }  /* end if */

 }  /* end for */

 _EstimateTimings ( apCont );

}  /* end _CalcFrameRate */

static int _LocateFrame ( _AVIdxEntry* apEnt, int anEnt, int aPos ) {

 uint32_t lA, lB, lM, lPos;

 lA = 0;
 lB = anEnt - 1;

 while ( lA <= lB ) {

  lM   = ( lA + lB ) >> 1;
  lPos = apEnt[ lM ].m_Pos;

  if ( lPos == aPos || !lM )

   goto found;

  else if ( lPos > aPos )

   lB = lM - 1;

  else lA = lM + 1;

 }  /* end while */

 lM = lA;

 if ( lM > 0 ) --lM;
found:
 return lM;

}  /* end _LocateFrame */

static int _FixOffsets ( SMS_Container* apCont, uint32_t aPos ) {

 uint32_t       i;
 _AVIContainer* lpCont = MYCONT( apCont );

 for ( i = 0; i < lpCont -> m_nChunks; ++i ) {

  if (  aPos < lpCont -> m_MoviEndList[ i ]  ) {

   lpCont -> m_RiffEnd  = lpCont -> m_RiffEndList [ i ];
   lpCont -> m_MoviEnd  = lpCont -> m_MoviEndList [ i ];
   lpCont -> m_MoviList = lpCont -> m_MoviListList[ i ];

   break;

  }  /* end if */

 }  /* end for */

 return i != lpCont -> m_nChunks;

}  /* end _FixOffsets */

static int _Seek ( SMS_Container* apCont, int anIdx, int aDir, uint32_t aPos ) {

 int         i;
 _AVIStream* lpStm = MYSTRM( apCont -> m_pStm[ anIdx ] );
 uint32_t    lEnd  = aDir > 0 ? lpStm -> m_nIdx : 0;
 uint32_t    lPos;

 if ( aPos >= lpStm -> m_nIdx ) return 0;

 while (  aPos != lEnd && !lpStm -> m_pIdx[ aPos ].m_fKey ) aPos += aDir;

 if ( aPos == lpStm -> m_nIdx ) return 0;

 lpStm -> m_NewFrameOffset = aPos;
 lPos                      = lpStm -> m_pIdx[ aPos ].m_Pos;

 for (  i = 0; i < ( int )apCont -> m_nStm; ++i  ) {

  uint32_t j;

  if ( i != anIdx ) {

   lpStm = MYSTRM( apCont -> m_pStm[ i ] );

   if ( lpStm -> m_nIdx <= 0 ) return 0;

   j = _LocateFrame ( lpStm -> m_pIdx, lpStm -> m_nIdx, lPos );

   if (  ( j + 1 ) < lpStm -> m_nIdx ) ++j;

   if ( lpStm -> m_SampleSize == 0 )

    lpStm -> m_NewFrameOffset = j;

   else lpStm -> m_NewFrameOffset = lpStm -> m_pIdx[ j ].m_CumLen;

  }  /* end if */

 }  /* end for */

 for (  i = 0; i < ( int )apCont -> m_nStm; ++i  ) {

  lpStm = MYSTRM( apCont -> m_pStm[ i ] );
  lpStm -> m_FrameOffset = lpStm -> m_NewFrameOffset;

 }  /* end for */

 if (  !MYCONT( apCont ) -> m_fOffsetBug ) lPos += MYCONT( apCont ) -> m_MoviList;

 if ( lPos < apCont -> m_pFileCtx -> m_Size ) {

  apCont -> m_pFileCtx -> Seek ( apCont -> m_pFileCtx, lPos );

  return _FixOffsets ( apCont, lPos );

 } else return 0;

}  /* end _Seek */

int SMS_GetContainerAVI ( SMS_Container* apCont ) {

 int      retVal = 0;
 uint32_t lRiffEnd;

 if (  ( int )apCont -> m_pFileCtx <= 0  ) return retVal;

 if (  _ProbeFile ( apCont -> m_pFileCtx, &lRiffEnd )  ) {

  apCont -> m_pCtx = ( _AVIContainer* )calloc (  1, sizeof ( _AVIContainer )  );

  MYCONT( apCont ) -> m_RiffEnd = lRiffEnd;

  if (   (  retVal = _ReadHeader ( apCont )  )   ) {

   apCont -> m_pName    = g_pAVIStr;
   apCont -> ReadPacket = _ReadPacket;
   apCont -> Seek       = _Seek;

   _CalcFrameRate ( apCont );

   g_pSynthBuffer = SMS_AUD_SPR;

  } else {

   free ( apCont -> m_pCtx );
   apCont -> m_pCtx = NULL;

  }  /* end else */

 }  /* end if */

 return retVal;

}  /* end SMS_GetContainerAVI */
