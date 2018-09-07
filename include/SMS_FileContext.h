/*
#     ___  _ _      ___
#    |    | | |    |
# ___|    |   | ___|    PS2DEV Open Source Project.
#----------------------------------------------------------
# (c) 2005 Eugene Plotnikov <e-plotnikov@operamail.com>
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
*/
#ifndef __SMS_FileContext_H
#define __SMS_FileContext_H

#ifndef __SMS_H
#include "SMS.h"
#endif  /* __SMS_H */

#ifndef INLINE
#  ifdef _WIN32
#   define INLINE __inline
#  else  /* PS2 */
#   define INLINE inline
#  endif  /* WIN32 */
#endif  /* INLINE */

#define FILE_EOF( f ) (  ( f ) -> m_CurPos >= ( f ) -> m_Size  )

typedef enum STIOMode {
 STIOMode_Extended, STIOMode_Ordinary
} STIOMode;

typedef struct CDDAContext {

 unsigned int m_nTracks;
 unsigned int m_StartSector[ 100 ];
 unsigned int m_EndSector  [ 100 ];
 unsigned int m_Offset;
 char*        m_pName;
 char*        m_pDescription;
 void*        m_pData;

} CDDAContext;

typedef struct CDDADirectory {

 char*                 m_pName;
 unsigned int          m_Idx;
 unsigned int          m_ImgIdx;
 struct CDDADirectory* m_pNext;

} CDDADirectory;

typedef struct CDDAFile {

 char*            m_pName;
 unsigned int     m_ImgIdx;
 unsigned int     m_Size;
 unsigned int     m_Offset;
 struct CDDAFile* m_pNext;

} CDDAFile;

typedef struct FileContext {

 unsigned int   m_CurBuf;
 unsigned int   m_Size;
 unsigned int   m_Pos;
 unsigned int   m_CurPos;
 unsigned int   m_BufSize;
 unsigned char* m_pBuff[ 2 ];
 unsigned char* m_pPos;
 unsigned char* m_pEnd;
 void*          m_pData;
 unsigned int   m_StreamSize;
 void*          m_pOpenParam;
 char*          m_pPath;

 int  ( *Read    ) ( struct FileContext*, void*, unsigned int          );
 int  ( *Seek    ) ( struct FileContext*, unsigned int                 );
 int  ( *Fill    ) ( struct FileContext*                               );
 int  ( *Stream  ) ( struct FileContext*, unsigned int, unsigned int   );
 void ( *Destroy ) ( struct FileContext*                               );

 struct FileContext* ( *Open ) ( const char*, void* );

} FileContext;
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

static INLINE int File_GetByte ( FileContext* apCtx ) {
 return apCtx -> m_pPos < apCtx -> m_pEnd || apCtx -> Fill ( apCtx ) > 0 ? ++apCtx -> m_CurPos, *apCtx -> m_pPos++ : 0;
}  /* end File_GetByte */

static INLINE unsigned int File_GetUShort ( FileContext* apCtx ) {
 unsigned int retVal = File_GetByte ( apCtx );
 return retVal | File_GetByte ( apCtx ) << 8;
}  /* end File_GetShort */

static INLINE int File_GetShortBE ( FileContext* apFileCtx ) {
 int retVal = File_GetByte ( apFileCtx ) << 8;
 retVal |= File_GetByte ( apFileCtx );
 return retVal;
}  /* end File_GetShortBE */

static INLINE unsigned int File_GetUInt ( FileContext* apCtx ) {
 unsigned int retVal;
 if (  apCtx -> m_pEnd - apCtx -> m_pPos >= 4  ) {
  retVal = SMS_unaligned32 ( apCtx -> m_pPos );
  apCtx -> m_pPos   += 4;
  apCtx -> m_CurPos += 4;
 } else apCtx -> Read ( apCtx, &retVal, 4 );
 return retVal;
}  /* end File_GetInt */

static INLINE unsigned long File_GetULong ( FileContext* apCtx ) {
 unsigned long retVal;
 if (  apCtx -> m_pEnd - apCtx -> m_pPos >= 8  ) {
  retVal = SMS_unaligned64 ( apCtx -> m_pPos );
  apCtx -> m_pPos   += 8;
  apCtx -> m_CurPos += 8;
 } else apCtx -> Read ( apCtx, &retVal, 8 );
 return retVal;
}  /* end File_GetULong */

void File_Skip      ( FileContext*, unsigned int        );
void File_GetString ( FileContext*, char*, unsigned int );

CDDAContext*         CDDA_InitContext     ( unsigned long                      );
void                 CDDA_DestroyContext  ( CDDAContext*                       );
const CDDADirectory* CDDA_DirectoryList   ( CDDAContext*                       );
CDDAFile*            CDDA_GetFileList     ( CDDAContext*, const CDDADirectory* );
void                 CDDA_DestroyFileList ( CDDAFile*                          );
int                  CDDA_GetPicture      ( CDDAContext*, int, void*           );
int                  CDDA_GetDiskPicture  ( CDDAContext*, void*                );

FileContext* CDDA_InitFileContext ( const char*, void* );
FileContext* STIO_InitFileContext ( const char*, void* );
#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __SMS_FileContext_H */
