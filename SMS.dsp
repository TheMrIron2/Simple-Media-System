# Microsoft Developer Studio Project File - Name="SMS" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=SMS - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SMS.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SMS.mak" CFG="SMS - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SMS - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "SMS - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "SMS - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f SMS.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "SMS.exe"
# PROP BASE Bsc_Name "SMS.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "make"
# PROP Rebuild_Opt "rebuild"
# PROP Target_File "bin/SMS.elf"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "SMS - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f SMS.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "SMS.exe"
# PROP BASE Bsc_Name "SMS.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "make"
# PROP Rebuild_Opt "rebuild"
# PROP Target_File "bin/SMS.elf"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "SMS - Win32 Release"
# Name "SMS - Win32 Debug"

!IF  "$(CFG)" == "SMS - Win32 Release"

!ELSEIF  "$(CFG)" == "SMS - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\About.c
# End Source File
# Begin Source File

SOURCE=.\src\About_Data.s
# End Source File
# Begin Source File

SOURCE=.\src\libmpeg.c
# End Source File
# Begin Source File

SOURCE=.\src\libmpeg_core.s
# End Source File
# Begin Source File

SOURCE=.\src\main.c
# End Source File
# Begin Source File

SOURCE=.\src\mbstring.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_AAC.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_AC3.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_AC3_imdct.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_CDDA.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_CDVD.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_Codec.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_Config.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_ConfigIcon.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_Container.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_ContainerAAC.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_ContainerAC3.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_ContainerASF.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_ContainerAVI.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_ContainerFLAC.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_ContainerJPG.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_ContainerM3U.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_ContainerMOV.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_ContainerMP3.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_ContainerMPEG_PS.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_ContainerOGG.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_CopyTree.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_Data.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_DateTime.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_DirTree.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_DMA_0.s
# End Source File
# Begin Source File

SOURCE=.\src\SMS_DSP.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_DSP_FFT.s
# End Source File
# Begin Source File

SOURCE=.\src\SMS_DSP_MMI.s
# End Source File
# Begin Source File

SOURCE=.\src\SMS_DSP_QPel.s
# End Source File
# Begin Source File

SOURCE=.\src\SMS_DTS.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_DXSB.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_EE.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_FileContext.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_FileDir.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_FileMapping.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_FLAC.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_GS_0.s
# End Source File
# Begin Source File

SOURCE=.\src\SMS_GS_1.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_GS_2.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_GSFont.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_GUI.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_GUIClock.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_GUICmdProc.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_GUIcons.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_GUIDesktop.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_GUIDevMenu.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_GUIFileCtxMenu.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_GUIFileMenu.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_GUIMenu.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_GUIMenuSMS.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_GUIMiniBrowser.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_GUInfoPanel.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_GUISMBrowser.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_H263.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_History.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_InverseCodePages.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_IOP.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_IPU.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_IPU_0.s
# End Source File
# Begin Source File

SOURCE=.\src\SMS_IPU_1.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_JPEG.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_JPEGData.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_List.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_Locale.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_MC.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_MP123.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_MP123Core.s
# End Source File
# Begin Source File

SOURCE=.\src\SMS_MPEG.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_MPEG12.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_MPEG4.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_MPEGInit.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_MSMPEG4.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_OGG.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_OS.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_PAD.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_PCM.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_PgInd.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_Player.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_PlayerBallSim.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_PlayerControl.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_PlayerMenu.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_RC.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_RC_0.s
# End Source File
# Begin Source File

SOURCE=.\src\SMS_Rescale.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_RingBuffer.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_SIF.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_Sounds.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_Spectrum.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_SPU.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_SubtitleContext.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_Timer.s
# End Source File
# Begin Source File

SOURCE=.\src\SMS_Utils.s
# End Source File
# Begin Source File

SOURCE=.\src\SMS_VideoBuffer.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_VLC.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_VSync.c
# End Source File
# Begin Source File

SOURCE=.\src\SMS_WMA.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\include\libmpeg.h
# End Source File
# Begin Source File

SOURCE=.\include\libmpeg_internal.h
# End Source File
# Begin Source File

SOURCE=.\include\mbstring.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_AAC.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_AC3.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_Bitio.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_CDDA.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_CDVD.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_Codec.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_Config.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_Container.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_ContainerAAC.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_ContainerAC3.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_ContainerASF.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_ContainerAVI.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_ContainerFLAC.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_ContainerJPG.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_ContainerM3U.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_ContainerMOV.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_ContainerMP3.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_ContainerMPEG_PS.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_ContainerOGG.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_Data.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_DateTime.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_DirTree.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_DMA.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_DSP.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_DTS.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_DXSB.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_EE.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_FileContext.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_FileDir.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_FileMapping.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_FLAC.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_FourCC.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_GS.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_GUI.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_GUIClock.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_GUIcons.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_GUIMenu.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_H263.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_History.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_INTC.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_ioctl.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_IOP.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_IPU.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_JPEG.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_List.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_Locale.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_MC.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_MP123.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_MPEG.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_MPEG12.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_MPEG4.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_MSMPEG4.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_OGG.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_OS.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_PAD.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_PCM.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_PgInd.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_Player.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_PlayerBallSim.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_PlayerControl.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_PlayerMenu.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_RC.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_Rescale.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_RingBuffer.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_SIF.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_SMB.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_Sounds.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_Spectrum.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_SPU.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_SubtitleContext.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_Timer.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_VideoBuffer.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_VIF.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_VLC.h
# End Source File
# Begin Source File

SOURCE=.\include\SMS_VSync.h
# End Source File
# Begin Source File

SOURCE=.\src\SMS_WMA.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\Makefile
# End Source File
# Begin Source File

SOURCE=.\README
# End Source File
# End Target
# End Project
