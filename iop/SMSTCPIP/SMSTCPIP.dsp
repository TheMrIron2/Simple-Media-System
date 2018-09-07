# Microsoft Developer Studio Project File - Name="SMSTCPIP" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=SMSTCPIP - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SMSTCPIP.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SMSTCPIP.mak" CFG="SMSTCPIP - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SMSTCPIP - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "SMSTCPIP - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "SMSTCPIP - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f SMSTCPIP.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "SMSTCPIP.exe"
# PROP BASE Bsc_Name "SMSTCPIP.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "make"
# PROP Rebuild_Opt "rebuild"
# PROP Target_File "bin/SMSTCPIP.elf"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "SMSTCPIP - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f SMSTCPIP.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "SMSTCPIP.exe"
# PROP BASE Bsc_Name "SMSTCPIP.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "make"
# PROP Rebuild_Opt "rebuild"
# PROP Target_File "bin/SMSTCPIP.elf"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "SMSTCPIP - Win32 Release"
# Name "SMSTCPIP - Win32 Debug"

!IF  "$(CFG)" == "SMSTCPIP - Win32 Release"

!ELSEIF  "$(CFG)" == "SMSTCPIP - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\api_lib.c
# End Source File
# Begin Source File

SOURCE=.\src\api_msg.c
# End Source File
# Begin Source File

SOURCE=.\src\etharp.c
# End Source File
# Begin Source File

SOURCE=.\src\icmp.c
# End Source File
# Begin Source File

SOURCE=.\src\ip.c
# End Source File
# Begin Source File

SOURCE=.\src\ip_addr.c
# End Source File
# Begin Source File

SOURCE=.\src\ip_frag.c
# End Source File
# Begin Source File

SOURCE=.\src\loopif.c
# End Source File
# Begin Source File

SOURCE=.\src\mem.c
# End Source File
# Begin Source File

SOURCE=.\src\memp.c
# End Source File
# Begin Source File

SOURCE=.\src\netif.c
# End Source File
# Begin Source File

SOURCE=.\src\pbuf.c
# End Source File
# Begin Source File

SOURCE=.\src\ps2ip.c
# End Source File
# Begin Source File

SOURCE=.\src\sockets.c
# End Source File
# Begin Source File

SOURCE=.\src\tcp.c
# End Source File
# Begin Source File

SOURCE=.\src\tcp_in.c
# End Source File
# Begin Source File

SOURCE=.\src\tcp_out.c
# End Source File
# Begin Source File

SOURCE=.\src\tcpip.c
# End Source File
# Begin Source File

SOURCE=.\src\udp.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\include\lwip\api.h
# End Source File
# Begin Source File

SOURCE=.\include\lwip\api_msg.h
# End Source File
# Begin Source File

SOURCE=.\include\lwip\arch.h
# End Source File
# Begin Source File

SOURCE=.\include\arch\cc.h
# End Source File
# Begin Source File

SOURCE=.\include\lwip\debug.h
# End Source File
# Begin Source File

SOURCE=.\include\lwip\def.h
# End Source File
# Begin Source File

SOURCE=.\include\lwip\dhcp.h
# End Source File
# Begin Source File

SOURCE=.\include\lwip\err.h
# End Source File
# Begin Source File

SOURCE=.\include\netif\etharp.h
# End Source File
# Begin Source File

SOURCE=.\include\lwip\icmp.h
# End Source File
# Begin Source File

SOURCE=.\include\lwip\inet.h
# End Source File
# Begin Source File

SOURCE=.\include\lwip\ip.h
# End Source File
# Begin Source File

SOURCE=.\include\lwip\ip_addr.h
# End Source File
# Begin Source File

SOURCE=.\include\lwip\ip_frag.h
# End Source File
# Begin Source File

SOURCE=.\include\irx_imports.h
# End Source File
# Begin Source File

SOURCE=.\include\netif\loopif.h
# End Source File
# Begin Source File

SOURCE=.\include\lwipopts.h
# End Source File
# Begin Source File

SOURCE=.\include\lwip\mem.h
# End Source File
# Begin Source File

SOURCE=.\include\lwip\memp.h
# End Source File
# Begin Source File

SOURCE=.\include\lwip\netif.h
# End Source File
# Begin Source File

SOURCE=.\include\lwip\opt.h
# End Source File
# Begin Source File

SOURCE=.\include\lwip\pbuf.h
# End Source File
# Begin Source File

SOURCE=.\include\ps2ip_internal.h
# End Source File
# Begin Source File

SOURCE=.\include\lwip\raw.h
# End Source File
# Begin Source File

SOURCE=.\include\lwip\snmp.h
# End Source File
# Begin Source File

SOURCE=.\include\lwip\sockets.h
# End Source File
# Begin Source File

SOURCE=.\include\lwip\stats.h
# End Source File
# Begin Source File

SOURCE=.\include\lwip\sys.h
# End Source File
# Begin Source File

SOURCE=.\include\arch\sys_arch.h
# End Source File
# Begin Source File

SOURCE=.\include\lwip\tcp.h
# End Source File
# Begin Source File

SOURCE=.\include\lwip\tcpip.h
# End Source File
# Begin Source File

SOURCE=.\include\lwip\udp.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\Makefile
# End Source File
# End Target
# End Project
