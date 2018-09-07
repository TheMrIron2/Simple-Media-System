# Microsoft Developer Studio Project File - Name="USBD" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=USBD - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "USBD.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "USBD.mak" CFG="USBD - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "USBD - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "USBD - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "USBD - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f USBD.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "USBD.exe"
# PROP BASE Bsc_Name "USBD.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "make"
# PROP Rebuild_Opt "rebuild"
# PROP Target_File "USBD.irx"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "USBD - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f USBD.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "USBD.exe"
# PROP BASE Bsc_Name "USBD.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "make"
# PROP Rebuild_Opt "rebuild"
# PROP Target_File "USBD.irx"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "USBD - Win32 Release"
# Name "USBD - Win32 Debug"

!IF  "$(CFG)" == "USBD - Win32 Release"

!ELSEIF  "$(CFG)" == "USBD - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\driver.c
# End Source File
# Begin Source File

SOURCE=.\src\hcd.c
# End Source File
# Begin Source File

SOURCE=.\src\hub.c
# End Source File
# Begin Source File

SOURCE=.\src\interface.c
# End Source File
# Begin Source File

SOURCE=.\src\mem.c
# End Source File
# Begin Source File

SOURCE=.\src\usbd.c
# End Source File
# Begin Source File

SOURCE=.\src\usbio.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\driver.h
# End Source File
# Begin Source File

SOURCE=.\src\hcd.h
# End Source File
# Begin Source File

SOURCE=.\src\hub.h
# End Source File
# Begin Source File

SOURCE=.\src\irx_imports.h
# End Source File
# Begin Source File

SOURCE=.\src\mem.h
# End Source File
# Begin Source File

SOURCE=.\include\usbd.h
# End Source File
# Begin Source File

SOURCE=.\include\usbd_macro.h
# End Source File
# Begin Source File

SOURCE=.\src\usbdpriv.h
# End Source File
# Begin Source File

SOURCE=.\src\usbio.h
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
