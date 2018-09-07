/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
# $Id: ps2ip.c 577 2004-09-14 14:41:46Z pixel $
# PS2 TCP/IP STACK FOR IOP
*/

#include <types.h>
#include <stdio.h>
#include <intrman.h>
#include <loadcore.h>
#include <thbase.h>
#include <vblank.h>
#include <modload.h>
#include <sysclib.h>
#include <thevent.h>
#include <thsemap.h>
#include <libsd.h>
#include <sysmem.h>
#include <lwip/memp.h>

#include <lwip/sys.h>
#include <lwip/tcpip.h>
#include <lwip/netif.h>
#include <lwip/dhcp.h>
#include <netif/loopif.h>
#include <netif/etharp.h>

#include "ps2ip_internal.h"
#include "arch/sys_arch.h"
#include "../../SMSUTILS/smsutils.h"

#define	SYS_MBOX_SIZE 64
#define QUEUE_INPUT   0

struct sys_mbox_msg {
 struct sys_mbox_msg* pNext;
 void*                pvMSG;
};

struct sys_mbox {
 u16_t     u16First;
 u16_t     u16Last;
 void*     apvMSG[ SYS_MBOX_SIZE ];
 int       iWaitPost;
 int       iWaitFetch;
 sys_sem_t Mail;
 sys_sem_t Mutex;
};

typedef struct ip_addr IPAddr;

#define MODNAME	"TCP/IP Stack"
IRX_ID( MODNAME, 1, 3 );

extern struct irx_export_table _exp_ps2ip;

#ifdef PS2IP_DHCP
static int iTimerDHCP = 0;
#endif  /* PS2IP_DHCP */

#if LWIP_HAVE_LOOPIF
static NetIF LoopIF;
#endif /* LWIP_HAVE_LOOPIF */

static u16_t inline GenNextMBoxIndex ( u16_t u16Index ) {
 return ( u16Index + 1 ) & ( SYS_MBOX_SIZE - 1 );
}  /* end GenNextMBoxIndex */

static int inline IsMessageBoxFull ( sys_mbox_t apMBox ) {
 return GenNextMBoxIndex ( apMBox -> u16Last ) == apMBox -> u16First;
}  /* end IsMessageBoxFull */

static int inline IsMessageBoxEmpty ( sys_mbox_t apMBox ) {
 return apMBox -> u16Last == apMBox -> u16First;
}  /* end IsMessageBoxEmpty */

int ps2ip_getconfig ( char* pszName, t_ip_info* pInfo ) {

 struct netif* pNetIF = netif_find ( pszName );

 if ( pNetIF == NULL ) {
  mips_memset (  pInfo, 0, sizeof ( *pInfo )  );
  return 0;
 }  /* end if */

 strcpy ( pInfo -> netif_name, pszName );

 pInfo -> ipaddr.s_addr  = pNetIF -> ip_addr.addr;
 pInfo -> netmask.s_addr = pNetIF -> netmask.addr;
 pInfo -> gw.s_addr      = pNetIF -> gw.addr;
 mips_memcpy(  pInfo -> hw_addr,pNetIF -> hwaddr, sizeof ( pInfo -> hw_addr )  );
#if LWIP_DHCP
 if ( pNetIF -> dhcp ) {
  pInfo -> dhcp_enabled = 1;
  pInfo -> dhcp_status  = pNetIF -> dhcp->state;
 } else {
  pInfo -> dhcp_enabled = 0;
  pInfo -> dhcp_status  = 0;
 }  /* end else */
#else
 pInfo -> dhcp_enabled = 0;
#endif  /* LWIP_DHCP */
 return 1;

}  /* end ps2ip_getconfig */

int ps2ip_setconfig ( t_ip_info* pInfo ) {

 struct netif* pNetIF = netif_find ( pInfo -> netif_name );

 if	( !pNetIF ) return	0;

 netif_set_ipaddr  (  pNetIF, ( IPAddr* )&pInfo -> ipaddr  );
 netif_set_netmask (  pNetIF, ( IPAddr* )&pInfo -> netmask );
 netif_set_gw      (  pNetIF, ( IPAddr* )&pInfo -> gw      );
#if	LWIP_DHCP
 if ( pInfo -> dhcp_enabled ) {
  if ( !pNetIF -> dhcp ) dhcp_start ( pNetIF );
 } else {
  if ( pNetIF -> dhcp ) dhcp_stop ( pNetIF );
 }  /* end else */
#endif  /* LWIP_DHCP */
 return 1;

}  /* end ps2ip_setconfig */

#define ALARM_TCP 0x00000001
#define ALARM_ARP 0x00000002
#define ALARM_MSK ( ALARM_TCP | ALARM_ARP )

typedef struct AlarmData {

 int             m_EventFlag;
 unsigned long   m_EventMask;
 iop_sys_clock_t m_Clock;

} AlarmData;

unsigned int _alarm ( void* apArg ) {

 AlarmData* lpData = ( AlarmData* )apArg;

 iSetEventFlag ( lpData -> m_EventFlag, lpData -> m_EventMask );

 return lpData -> m_Clock.lo;

}  /* end _alarm */

static void TimerThread ( void* apArg ) {

 AlarmData   lTCPData;
 AlarmData   lARPData;
 iop_event_t lEvent;

 lEvent.attr = 0;
 lEvent.bits = 0;
 lTCPData.m_EventFlag =
 lARPData.m_EventFlag = CreateEventFlag ( &lEvent );
 lTCPData.m_EventMask = ALARM_TCP;
 lARPData.m_EventMask = ALARM_ARP;
 USec2SysClock ( TCP_TMR_INTERVAL * 1024, &lTCPData.m_Clock );
 USec2SysClock ( ARP_TMR_INTERVAL * 1024, &lARPData.m_Clock );

 SetAlarm ( &lTCPData.m_Clock, _alarm, &lTCPData );
 SetAlarm ( &lARPData.m_Clock, _alarm, &lARPData );

 while ( 1 ) {

  unsigned long lRes;

  WaitEventFlag ( lTCPData.m_EventFlag, ALARM_MSK, WEF_CLEAR | WEF_OR, &lRes );

  if ( lRes & ALARM_TCP ) tcp_tmr    ();
  if ( lRes & ALARM_ARP ) etharp_tmr ();

 }  /* end while */

}  /* end TimerThread */

static void inline InitTimer ( void ) {

 iop_thread_t lThread = { TH_C, 0, TimerThread, 0x800, 0x22 };

 StartThread (  CreateThread ( &lThread ), NULL  );

}  /* end InitTimer */
#if QUEUE_INPUT
static void InputCB ( struct tcpip_msg* apMsg ) {

 struct pbuf*  pInput = apMsg -> p;
 struct netif* pNetIF = apMsg -> netif;

 switch (   (  ( struct eth_hdr* )( pInput -> payload )  ) -> type   ) {

  case ETHTYPE_IP:
   etharp_ip_input ( pNetIF, pInput );
   pbuf_header ( pInput, -14 );
   ip_input ( pInput, pNetIF );
  break;

  case ETHTYPE_ARP:
   etharp_arp_input (
    pNetIF, ( struct eth_addr* )&pNetIF -> hwaddr, pInput
   );
  default: pbuf_free ( pInput );

 }  /* end switch */

}  /* end InputCB */

extern sys_mbox_t g_TCPIPMBox;
#endif  /* QUEUE_INPUT */
err_t ps2ip_input ( struct pbuf* pInput, struct netif* pNetIF ) {
#if QUEUE_INPUT
 struct tcpip_msg* lpMSG;

 if (  !IsMessageBoxFull ( g_TCPIPMBox ) && (
         lpMSG = ( struct tcpip_msg* )memp_malloc ( MEMP_TCPIP_MSG )
        )
 ) {

  lpMSG -> type  = TCPIP_MSG_CALLBACK;
  lpMSG -> p     = pInput;
  lpMSG -> netif = pNetIF;
  lpMSG -> f     = InputCB;

  g_TCPIPMBox -> apvMSG[ g_TCPIPMBox -> u16Last ] = lpMSG;
  g_TCPIPMBox -> u16Last = GenNextMBoxIndex ( g_TCPIPMBox -> u16Last );

  if ( g_TCPIPMBox -> iWaitFetch > 0 ) SignalSema ( g_TCPIPMBox -> Mail );

 } else pbuf_free ( pInput );
#else
 switch (   *( unsigned short* )&(  ( struct eth_hdr* )( pInput -> payload )  ) -> type   ) {

  case ETHTYPE_IP:
   etharp_ip_input ( pNetIF, pInput );
   pbuf_header ( pInput, -14 );
   ip_input ( pInput, pNetIF );
  break;

  case ETHTYPE_ARP:
   etharp_arp_input (
    pNetIF, ( struct eth_addr* )&pNetIF -> hwaddr, pInput
   );
  default: pbuf_free ( pInput );

 }  /* end switch */
#endif  /* QUEUE_INPUT */
 return ERR_OK;

}  /* end ps2ip_input */

void ps2ip_Stub ( void ) {

}  /* end ps2ip_Stub */

int ps2ip_ShutDown ( void ) {

 return 1;

}  /* end ps2ip_ShutDown */
#if LWIP_HAVE_LOOPIF
static void AddLoopIF ( void ) {

 IPAddr IP;
 IPAddr NM;
 IPAddr GW;

 IP4_ADDR( &IP, 127, 0, 0, 1 );
 IP4_ADDR( &NM, 255, 0, 0, 0 );
 IP4_ADDR( &GW, 127, 0, 0, 1 );

 netif_add ( &LoopIF, &IP, &NM, &GW, NULL, loopif_init, tcpip_input );

}  /* end AddLoopIF */
#endif  /* LWIP_HAVE_LOOPIF */
int _start ( int argc,char** argv ) {

 sys_sem_t lSema;

 RegisterLibraryEntries ( &_exp_ps2ip );

 mem_init   ();
 memp_init  ();
 pbuf_init  ();
 netif_init ();

 lSema = sys_sem_new ( 0 );
 tcpip_init (   (  void ( * )( void* )  )SignalSema, ( void* )lSema   );
 WaitSema   ( lSema );
 DeleteSema ( lSema );
#if LWIP_HAVE_LOOPIF
 AddLoopIF ();
#endif  /* LWIP_HAVE_LOOPIF */
 InitTimer ();

 return MODULE_RESIDENT_END; 

}  /* end _start */

#define	SYS_THREAD_PRIO_BASE 0x22

sys_thread_t sys_thread_new (
              void ( *pFunction )(  void* ),void* pvArg,int iPrio
             ) {

 iop_thread_t Info = {
  TH_C, 0, pFunction, 0x900, iPrio + SYS_THREAD_PRIO_BASE
 };
 int iThreadID;

 iThreadID = CreateThread ( &Info );

 if	( iThreadID < 0 ) return -1;

 StartThread ( iThreadID, pvArg );

 return iThreadID;

}  /* end sys_thread_new */

sys_mbox_t sys_mbox_new ( void ) {

 sys_mbox_t pMBox;

 pMBox = ( sys_mbox_t )AllocSysMemory (  0, sizeof ( struct sys_mbox ), 0  );

 if ( !pMBox ) return NULL;

 pMBox -> u16First  = pMBox -> u16Last = 0;
 pMBox -> Mail      = sys_sem_new ( 0 );
 pMBox -> Mutex     = sys_sem_new ( 1 );
 pMBox -> iWaitPost = pMBox -> iWaitFetch = 0;

 return	pMBox;

}  /* end sys_mbox_new */

void sys_mbox_free ( sys_mbox_t pMBox ) {

 if	( !pMBox ) return;

 WaitSema ( pMBox -> Mutex );

 DeleteSema ( pMBox -> Mail  );
 DeleteSema ( pMBox -> Mutex );

 FreeSysMemory ( pMBox );

}  /* end sys_mbox_free */

void sys_mbox_post ( sys_mbox_t pMBox, void* pvMSG ) {

 sys_prot_t Flags;

 if	( !pMBox ) return;

 CpuSuspendIntr ( &Flags );

 while (  IsMessageBoxFull ( pMBox )  ) {

  ++pMBox -> iWaitPost;

  CpuResumeIntr ( Flags );
   WaitSema ( pMBox -> Mail );
  CpuSuspendIntr ( &Flags );

  --pMBox -> iWaitPost;

 }  /* end while */

 pMBox -> apvMSG[ pMBox -> u16Last ] = pvMSG;
 pMBox -> u16Last = GenNextMBoxIndex ( pMBox -> u16Last );

 if	( pMBox -> iWaitFetch > 0 ) SignalSema ( pMBox -> Mail );

 CpuResumeIntr ( Flags );

}  /* end sys_mbox_post */

u32_t sys_arch_mbox_fetch ( sys_mbox_t pMBox, void** ppvMSG, u32_t u32Timeout ) {

 sys_prot_t	Flags;
 u32_t      u32Time = 0;

 CpuSuspendIntr ( &Flags );

 while (  IsMessageBoxEmpty ( pMBox )  ) {

  u32_t u32WaitTime;

  ++pMBox -> iWaitFetch;

  CpuResumeIntr ( Flags );
   u32WaitTime = sys_arch_sem_wait ( pMBox -> Mail, u32Timeout );
  CpuSuspendIntr ( &Flags );

  --pMBox -> iWaitFetch;

  if ( u32WaitTime == SYS_ARCH_TIMEOUT )  {
   u32Time = u32WaitTime;
   goto end;
  }  /* end if */

  u32Time    += u32WaitTime;
  u32Timeout -= u32WaitTime;

 }  /* end while */

 *ppvMSG = pMBox -> apvMSG[ pMBox -> u16First ];
  pMBox -> u16First = GenNextMBoxIndex ( pMBox -> u16First );

 if	( pMBox -> iWaitPost > 0 ) SignalSema ( pMBox -> Mail );
end:
 CpuResumeIntr ( Flags );

 return u32Time;

}  /* end sys_arch_mbox_fetch */

sys_sem_t sys_sem_new ( u8_t aCount ) {

 iop_sema_t lSema = { 1, 1, aCount,1 };
 int        retVal;

 retVal = CreateSema ( &lSema );

 if ( retVal <= 0 ) retVal = SYS_SEM_NULL;

 return	retVal;

}  /* end sys_sem_new */

u32_t sys_arch_sem_wait ( sys_sem_t aSema, u32_t aTimeout ) {

 if	( !aTimeout )
  return WaitSema ( aSema );
 else {

  iop_sys_clock_t lTimeout;
  int             lTID = GetThreadId ();

  USec2SysClock ( aTimeout * 1024, &lTimeout );
  SetAlarm (   &lTimeout, (  unsigned ( * ) ( void* )  )iReleaseWaitThread, ( void* )lTID   );

  if (  !WaitSema ( aSema )  ) {
   CancelAlarm (   (  unsigned ( * ) ( void* )  )iReleaseWaitThread, ( void* )lTID   );
   --aTimeout;
  } else aTimeout = SYS_ARCH_TIMEOUT;

 }  /* end else */

 return aTimeout;

}  /* end sys_arch_sem_wait */
