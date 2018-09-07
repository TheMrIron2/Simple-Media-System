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
#ifndef __SMS_VIF_H
#define __SMS_VIF_H

#define VIF1_STAT() (  *( volatile unsigned* )0x10003C00  )
#define VIF1_ERR()  (  *( volatile unsigned* )0x10003C20  )

#define VIF_CODE_NOP         0x00
#define VIF_CODE_NOPi        0x80
#define VIF_CODE_DIRECT      0x50
#define VIF_CODE_DIRECTi     0xD0
#define VIF_CODE_DIRECTHL    0x51
#define VIF_CODE_DIRECTHLi   0xD1
#define VIF_CODE_UNPACK_S8   0x62
#define VIF_CODE_UNPACK_S8m  0x72
#define VIF_CODE_UNPACK_S8mi 0xF2
#define VIF_CODE_SET_STMASK  0x20
#define VIF_CODE_SET_STMASKi 0xA0

#define VIF_CODE( CMD, NUM, IMM ) (       \
 (   (  ( unsigned int )CMD  ) << 24   ) | \
 (   (  ( unsigned int )NUM  ) << 16   ) | \
 (   (  ( unsigned int )IMM  )         )   \
)

#define VIF_DIRECT( SIZE ) (                                          \
 (  ( unsigned long int )VIF_CODE( VIF_CODE_DIRECT, 0, SIZE )  ) << 32 \
)

#define VIF_DIRECTHL( SIZE ) (                                          \
 (  ( unsigned long int )VIF_CODE( VIF_CODE_DIRECTHL, 0, SIZE )  ) << 32 \
)
#endif  /* __SMS_VIF_H */
