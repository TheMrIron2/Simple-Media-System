/*
#     ___  _ _      ___
#    |    | | |    |
# ___|    |   | ___|    PS2DEV Open Source Project.
#----------------------------------------------------------
# Based on ffmpeg project (no copyright notes in the original source code)
# (c) 2005 Eugene Plotnikov <e-plotnikov@operamail.com>
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
*/
#ifndef __SMS_Bitio_H
#define __SMS_Bitio_H

#ifndef __SMS_H
#include "SMS.h"
#endif  /* __SMS_H */

#define SMS_MIN_CACHE_BITS 25

#define SMS_OPEN_READER( name, gb ) \
 int name##_index = ( gb ) -> m_Idx; \
 int name##_cache = 0;

#define SMS_CLOSE_READER( name, gb ) \
 ( gb ) -> m_Idx = name##_index;

#define SMS_UPDATE_CACHE( name, gb )                 \
 name##_cache = SMS_unaligned32_be (                  \
                 (  ( uint8_t* )( gb ) -> m_pBuf  ) + \
                 ( name##_index >> 3 )                \
                ) << ( name##_index & 0x07 );

#define SMS_SKIP_COUNTER( name, num )       name##_index += ( num );
#define SMS_SHOW_UBITS( name, gb, num )     SMS_NEG_USR32( name##_cache, num )
#define SMS_SHOW_SBITS( name, gb, num )     SMS_NEG_SSR32( name##_cache, num )
#define SMS_LAST_SKIP_BITS( name, gb, num ) SMS_SKIP_COUNTER( name, num )
#define SMS_SKIP_CACHE( name, num )         name##_cache <<= ( num );
#define SMS_LAST_SKIP_CACHE( name, gb, num )

#define SMS_SKIP_BITS( name, gb, num ) { \
 SMS_SKIP_CACHE( name, num )              \
 SMS_SKIP_COUNTER( name, num )            \
}

#define SMS_GET_CACHE( name, gb ) (  ( uint32_t )name##_cache  )

typedef struct SMS_BitContext {

 const uint8_t* m_pBuf;
 int            m_Idx;
 int            m_szInBits;

} SMS_BitContext;

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

//void SMS_InitGetBits ( SMS_BitContext*, const uint8_t*, uint32_t );

#ifdef __cplusplus
}
#endif  /* __cplusplus */

static SMS_INLINE int SMS_unaligned32_be ( const void* apVal ) {
 return SMS_bswap32 (  SMS_unaligned32 ( apVal )  );
}  /* end SMS_unaligned32_be */

static SMS_INLINE uint32_t SMS_GetBits ( SMS_BitContext* apCtx, uint32_t aN ) {
 uint32_t lVal;
 SMS_OPEN_READER( re, apCtx )
  SMS_UPDATE_CACHE( re, apCtx )
  lVal = SMS_SHOW_UBITS( re, apCtx, aN );
  SMS_LAST_SKIP_BITS( re, apCtx, aN )
 SMS_CLOSE_READER( re, apCtx )
 return lVal;
}  /* end SMS_GetBits */

static SMS_INLINE int32_t SMS_GetSBits ( SMS_BitContext* apCtx, uint32_t aN ) {
 int32_t lVal;
 SMS_OPEN_READER( re, apCtx )
  SMS_UPDATE_CACHE( re, apCtx )
  lVal = SMS_SHOW_SBITS( re, apCtx, aN );
  SMS_LAST_SKIP_BITS( re, apCtx, aN )
 SMS_CLOSE_READER( re, apCtx )
 return lVal;
}  /* end SMS_GetSBits */

static SMS_INLINE uint32_t SMS_GetBit ( SMS_BitContext* apCtx ) {
 uint32_t lIdx = apCtx -> m_Idx;
 uint8_t  lRes = apCtx -> m_pBuf[ lIdx >> 3 ];
 lRes <<= ( lIdx & 0x07 );
 lRes >>= 8 - 1;
 apCtx -> m_Idx = ++lIdx;
 return lRes;
}  /* end SMS_GetBit */

static SMS_INLINE int32_t SMS_BitCount ( SMS_BitContext* apCtx ) {
 return apCtx -> m_Idx;
}  /* end SMS_BitCount */

static SMS_INLINE void SMS_SkipBits ( SMS_BitContext* apCtx, uint32_t aN ) {
 SMS_OPEN_READER( re, apCtx )
  SMS_UPDATE_CACHE( re, apCtx )
  SMS_LAST_SKIP_BITS( re, apCtx, aN )
 SMS_CLOSE_READER( re, apCtx )
}  /* end SMS_SkipBits */

static SMS_INLINE void SMS_SkipBit ( SMS_BitContext* apCtx ) {
 SMS_SkipBits ( apCtx, 1 );
}  /* end SMS_SkipBit */

static SMS_INLINE void SMS_AlignBits ( SMS_BitContext* apCtx ) {
 int32_t lN = (  -SMS_BitCount ( apCtx )  ) & 7;
 if ( lN ) SMS_SkipBits( apCtx, lN );
}  /* end SMS_AlignBits */

static SMS_INLINE uint32_t SMS_ShowBits ( SMS_BitContext* apCtx, uint32_t aN ) {
 uint32_t lVal;
 SMS_OPEN_READER( re, apCtx )
 SMS_UPDATE_CACHE( re, apCtx )
 lVal = SMS_SHOW_UBITS( re, apCtx, aN);
 return lVal;
}  /* end SMS_ShowBits */

static SMS_INLINE int32_t SMS_GetXBits ( SMS_BitContext* apCtx, uint32_t aN ) {
 int32_t lVal;
 int32_t lCache;
 SMS_OPEN_READER( re, apCtx )
  SMS_UPDATE_CACHE( re, apCtx )
  lCache = SMS_GET_CACHE( re, apCtx );
  if (  ( int32_t )lCache < 0  ) {
   lVal = SMS_NEG_USR32( lCache, aN );
  } else {
   lVal = -( int )SMS_NEG_USR32( ~lCache, aN );
  }
  SMS_LAST_SKIP_BITS( re, apCtx, aN )
 SMS_CLOSE_READER( re, apCtx )
 return lVal;
}  /* end SMS_GetXBits */

static SMS_INLINE void SMS_InitGetBits ( SMS_BitContext* apCtx, const uint8_t* apBuf, uint32_t aBitSize ) {
 apCtx -> m_pBuf     = apBuf;
 apCtx -> m_szInBits = aBitSize;
 apCtx -> m_Idx      = 0;
}  /* end SMS_InitGetBits */
#endif  /* __SMS_Bitio_H */
