/*
#     ___  _ _      ___
#    |    | | |    |
# ___|    |   | ___|    PS2DEV Open Source Project.
#----------------------------------------------------------
# (c) 2005-2007 Eugene Plotnikov <e-plotnikov@operamail.com>
# (c) 2006 hjx (widescreen support)
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
*/
#ifndef __SMS_IPU_H
#define __SMS_IPU_H

struct SMS_MacroBlock;
struct SMS_FrameBuffer;

typedef struct IPURegs {

 volatile unsigned long m_CMD  __attribute__(   (  aligned( 16 )  )   );
 volatile unsigned int  m_CTRL __attribute__(   (  aligned( 16 )  )   );
 volatile unsigned int  m_BP   __attribute__(   (  aligned( 16 )  )   );
 volatile unsigned long m_TOP  __attribute__(   (  aligned( 16 )  )   );

} IPURegs;

#define IPU (  ( volatile IPURegs* )0x10002000  )

#define IPU_REG_CMD  (  ( volatile unsigned long* )0x10002000  )
#define IPU_REG_CTRL (  ( volatile unsigned int*  )0x10002010  )
#define IPU_REG_BP   (  ( volatile unsigned int*  )0x10002020  )
#define IPU_REG_TOP  (  ( volatile unsigned long* )0x10002030  )
#define IPU_FIFO_I   (  ( volatile unsigned long* )0x10007000  )
#define IPU_FIFO_O   (  ( volatile unsigned long* )0x10007010  )

#define IPU_SET_CMD( CODE, OPTION ) \
 IPU -> m_CMD = ( unsigned long )(  ( CODE << 28 ) | OPTION  )

#define IPU_CMD_BCLR( BP ) IPU_SET_CMD( 0, BP )

#define IPU_CMD_IDEC( FB, QSC, DTD, SGN, DTE, OFM ) \
 IPU_SET_CMD(  1, FB | ( QSC << 20 ) | ( DTD << 24 ) | ( SGN << 25  ) | ( DTE << 26 ) | ( OFM << 27 )  )

#define IPU_CMD_FDEC( FB ) IPU_SET_CMD( 4, FB )

#define IPU_CMD_SETTH( TH0, TH1 ) \
 IPU_SET_CMD(  9, TH0 | ( TH1 << 16 )  )

#define IPU_CMD_CSC( MBC, DTE, OFM ) \
 IPU_SET_CMD(  7, MBC | ( DTE << 26 ) | ( OFM << 27 )  )

#define IPU_TOP() (  ( u32 )*IPU_REG_TOP  )
#define IPU_CMD() (  ( u32 )*IPU_REG_CMD  )

#define IPU_WAIT() while (  ( int )IPU -> m_CTRL < 0  )
#define IPU_RESET() IPU -> m_CTRL = ( 1 << 30 ); IPU_WAIT(); IPU_CMD_BCLR( 0 );  IPU_WAIT()

typedef struct IPULoadImage {

 unsigned long* m_pDMA;
 unsigned char* m_pData;
 unsigned short m_Width;
 unsigned short m_Height;
 unsigned short m_QWC;
 unsigned short m_fPal;
 unsigned int   m_Pal[ 16 ];

 void ( *Destroy ) ( struct IPULoadImage* );

} IPULoadImage;

typedef struct IPUContext {

 unsigned long int      m_DMAGIFBgtn[ 16 ] __attribute__(   ( aligned( 16 )  )   );
 unsigned long int      m_DMAGIFTX  [  8 ] __attribute__(   ( aligned( 16 )  )   );
 unsigned long int      m_DMAGIFPack[  8 ] __attribute__(   ( aligned( 16 )  )   );
 unsigned int           m_ImgLeft   [  8 ];
 unsigned int           m_ImgTop    [  8 ];
 unsigned int           m_ImgRight  [  8 ];
 unsigned int           m_ImgBottom [  8 ];
 unsigned int           m_TxtLeft   [  8 ];
 unsigned int           m_TxtTop    [  8 ];
 unsigned int           m_TxtRight  [  8 ];
 unsigned int           m_TxtBottom [  8 ];
 unsigned int           m_DMAHandlerID_GIF;
 unsigned int           m_PixFmt;
 unsigned int           m_TexFmt;
 unsigned int           m_fDraw;
 unsigned int           m_VRAM;
 unsigned int           m_TBW;
 unsigned int           m_TW;
 unsigned int           m_TH;
 unsigned int           m_Width;
 unsigned int           m_Height;
 unsigned int           m_ScrRight;
 unsigned int           m_ScrBottom;
 unsigned int           m_ModeIdx;
 unsigned short         m_fWS;
 unsigned char          m_fPG;
 unsigned char          m_fFL;
 unsigned long int*     m_pDMAPacket;
 unsigned long          m_BRGBAQ;
 unsigned long          m_Alpha;
 long*                  m_pAudioPTS;
 long                   m_VideoPTS;
 int                    m_VSync;
 
 void ( *Sync          ) ( void         );
 void ( *Display       ) ( void*, long  );
 void ( *Destroy       ) ( void         );
 void ( *SetTEX        ) ( void         );
 void ( *Reset         ) ( void         );
 void ( *QueuePacket   ) ( int, void*   );
 void ( *Suspend       ) ( void         );
 void ( *Resume        ) ( void         );
 void ( *Repaint       ) ( void         );
 void ( *ChangeMode    ) ( unsigned int );
 void ( *Pan           ) ( int          );
 void ( *Flush         ) ( void         );
 void ( *SetBrightness ) ( unsigned int );
 void ( *StopSync      ) ( int          );
 void ( *QueueSubtitle ) ( void*        );

} IPUContext;

extern IPUContext g_IPUCtx;
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

IPUContext* IPU_InitContext ( int, int, long*, int );

unsigned int IPU_FDEC ( unsigned                                                   );
unsigned int IPU_IDEC ( unsigned, unsigned, unsigned, unsigned, unsigned, unsigned );
void         IPU_FRST ( void                                                       );

void           IPU_InitLoadImage ( IPULoadImage*, int, int                            );
void           IPU_LoadImage     ( IPULoadImage*, void*, int, int, int, int, int, int );
unsigned short IPU_ImageInfo     ( void*, unsigned short*                             );
void           IPU_UnpackImage   ( void*, void*, int, int, int, int, int, int         );

#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __SMS_IPU_H */
