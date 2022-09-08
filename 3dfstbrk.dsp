# Microsoft Developer Studio Project File - Name="3dfstbrk" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=3dfstbrk - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "3dfstbrk.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "3dfstbrk.mak" CFG="3dfstbrk - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "3dfstbrk - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "3dfstbrk - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "3dfstbrk - Win32 Release"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /c
# ADD CPP /nologo /MT /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 oldnames.lib /nologo /stack:0x2800 /subsystem:windows /machine:IX86
# ADD LINK32 oldnames.lib /nologo /stack:0x2800 /subsystem:windows /machine:IX86

!ELSEIF  "$(CFG)" == "3dfstbrk - Win32 Debug"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 oldnames.lib /nologo /stack:0x2800 /subsystem:windows /debug /machine:IX86
# ADD LINK32 oldnames.lib winmm.lib ZoomView.lib zoomview.lib /nologo /stack:0x2800 /subsystem:windows /debug /machine:IX86

!ENDIF 

# Begin Target

# Name "3dfstbrk - Win32 Release"
# Name "3dfstbrk - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\3dfstbrk.cpp
# End Source File
# Begin Source File

SOURCE=.\3dfstbrk.def
# End Source File
# Begin Source File

SOURCE=.\3dfstbrk.rc
# End Source File
# Begin Source File

SOURCE=.\Askgroup.cpp
# End Source File
# Begin Source File

SOURCE=.\Cdswpar.cpp
# End Source File
# Begin Source File

SOURCE=.\CheckT.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\ColorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ControlDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ControlFile.cpp
# End Source File
# Begin Source File

SOURCE=.\ControlView.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAppeData.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFbkShow.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPlusCoor.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRelation.cpp
# End Source File
# Begin Source File

SOURCE=.\Dpgnum.cpp
# End Source File
# Begin Source File

SOURCE=.\Drlispi.cpp
# End Source File
# Begin Source File

SOURCE=.\Drlnspn.cpp
# End Source File
# Begin Source File

SOURCE=.\Dsvsys.cpp
# End Source File
# Begin Source File

SOURCE=.\Dswnum.cpp
# End Source File
# Begin Source File

SOURCE=.\Equation.cpp
# End Source File
# Begin Source File

SOURCE=.\fbk.cpp
# End Source File
# Begin Source File

SOURCE=.\FbkDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\FbkInFile.cpp
# End Source File
# Begin Source File

SOURCE=.\FbkPureDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\FbkPureView.cpp
# End Source File
# Begin Source File

SOURCE=.\FbkView.cpp
# End Source File
# Begin Source File

SOURCE=.\Fh.cpp
# End Source File
# Begin Source File

SOURCE=.\Fileface.cpp
# End Source File
# Begin Source File

SOURCE=.\FontGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\FrtCvr.cpp
# End Source File
# Begin Source File

SOURCE=.\InP190.cpp
# End Source File
# Begin Source File

SOURCE=.\Line.cpp
# End Source File
# Begin Source File

SOURCE=.\LWCtl.cpp
# End Source File
# Begin Source File

SOURCE=.\Mainfrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Makeequa.cpp
# End Source File
# Begin Source File

SOURCE=.\Memequa.cpp
# End Source File
# Begin Source File

SOURCE=.\MidFbkFile.cpp
# End Source File
# Begin Source File

SOURCE=.\My3DPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\Mylist.cpp
# End Source File
# Begin Source File

SOURCE=.\MyPrcPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\Neunet.cpp
# End Source File
# Begin Source File

SOURCE=.\OutFbkFile.cpp
# End Source File
# Begin Source File

SOURCE=.\P190.cpp
# End Source File
# Begin Source File

SOURCE=.\P190Doc.cpp
# End Source File
# Begin Source File

SOURCE=.\P190View.cpp
# End Source File
# Begin Source File

SOURCE=.\posvalue.cpp
# End Source File
# Begin Source File

SOURCE=.\ProDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Progdlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Relation.cpp
# End Source File
# Begin Source File

SOURCE=.\sndfile.cpp
# End Source File
# Begin Source File

SOURCE=.\SPPDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\Sppmaker.cpp
# End Source File
# Begin Source File

SOURCE=.\SPPView.cpp
# End Source File
# Begin Source File

SOURCE=.\Stdafx.cpp
# End Source File
# Begin Source File

SOURCE=.\Svsys.cpp
# End Source File
# Begin Source File

SOURCE=.\Swathpar.cpp
# End Source File
# Begin Source File

SOURCE=.\TagLine.cpp
# End Source File
# Begin Source File

SOURCE=.\TDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\testDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\testView.cpp
# End Source File
# Begin Source File

SOURCE=.\TopoNet.cpp
# End Source File
# Begin Source File

SOURCE=.\TopoNetDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\TopoNetView.cpp
# End Source File
# Begin Source File

SOURCE=.\TView.cpp
# End Source File
# Begin Source File

SOURCE=.\Unform.cpp
# End Source File
# Begin Source File

SOURCE=.\ValueFbk.cpp
# End Source File
# Begin Source File

SOURCE=.\ZDEquation.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\3dfstbrk.h
# End Source File
# Begin Source File

SOURCE=.\AskGroup.h
# End Source File
# Begin Source File

SOURCE=.\BlockTrace.h
# End Source File
# Begin Source File

SOURCE=.\cdswpar.h
# End Source File
# Begin Source File

SOURCE=.\CheckT.h
# End Source File
# Begin Source File

SOURCE=.\ColorDlg.h
# End Source File
# Begin Source File

SOURCE=.\ControlDoc.h
# End Source File
# Begin Source File

SOURCE=.\ControlFile.h
# End Source File
# Begin Source File

SOURCE=.\ControlView.h
# End Source File
# Begin Source File

SOURCE=.\DlgAppeData.h
# End Source File
# Begin Source File

SOURCE=.\DlgFbkShow.h
# End Source File
# Begin Source File

SOURCE=.\DlgPlusCoor.h
# End Source File
# Begin Source File

SOURCE=.\DlgRelation.h
# End Source File
# Begin Source File

SOURCE=.\Dpgnum.h
# End Source File
# Begin Source File

SOURCE=.\Drlispi.h
# End Source File
# Begin Source File

SOURCE=.\Drlnspn.h
# End Source File
# Begin Source File

SOURCE=.\Dsvsys.h
# End Source File
# Begin Source File

SOURCE=.\Dswnum.h
# End Source File
# Begin Source File

SOURCE=.\Equation.h
# End Source File
# Begin Source File

SOURCE=.\fbk.h
# End Source File
# Begin Source File

SOURCE=.\FbkDoc.h
# End Source File
# Begin Source File

SOURCE=.\FbkInFile.h
# End Source File
# Begin Source File

SOURCE=.\FbkPureDoc.h
# End Source File
# Begin Source File

SOURCE=.\FbkPureView.h
# End Source File
# Begin Source File

SOURCE=.\FbkView.h
# End Source File
# Begin Source File

SOURCE=.\fh.h
# End Source File
# Begin Source File

SOURCE=.\FileFace.h
# End Source File
# Begin Source File

SOURCE=.\FontGroup.h
# End Source File
# Begin Source File

SOURCE=.\FrtCvr.h
# End Source File
# Begin Source File

SOURCE=.\InP190.h
# End Source File
# Begin Source File

SOURCE=.\Line.h
# End Source File
# Begin Source File

SOURCE=.\LWCtl.h
# End Source File
# Begin Source File

SOURCE=.\Mainfrm.h
# End Source File
# Begin Source File

SOURCE=.\Makeequa.h
# End Source File
# Begin Source File

SOURCE=.\Memequa.h
# End Source File
# Begin Source File

SOURCE=.\MidFbkFile.h
# End Source File
# Begin Source File

SOURCE=.\My3DPoint.h
# End Source File
# Begin Source File

SOURCE=.\Mylist.h
# End Source File
# Begin Source File

SOURCE=.\Mypoint.h
# End Source File
# Begin Source File

SOURCE=.\MyPrcPoint.h
# End Source File
# Begin Source File

SOURCE=.\neunet.h
# End Source File
# Begin Source File

SOURCE=.\OutFbkFile.h
# End Source File
# Begin Source File

SOURCE=.\P190.h
# End Source File
# Begin Source File

SOURCE=.\P190Doc.h
# End Source File
# Begin Source File

SOURCE=.\P190View.h
# End Source File
# Begin Source File

SOURCE=.\posvalue.h
# End Source File
# Begin Source File

SOURCE=.\ProDlg.h
# End Source File
# Begin Source File

SOURCE=.\ProgDlg.h
# End Source File
# Begin Source File

SOURCE=.\pxyv.h
# End Source File
# Begin Source File

SOURCE=.\RcvPoint.h
# End Source File
# Begin Source File

SOURCE=.\Relation.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\shotmsg.h
# End Source File
# Begin Source File

SOURCE=.\sndfile.h
# End Source File
# Begin Source File

SOURCE=.\Spp.h
# End Source File
# Begin Source File

SOURCE=.\SPPDoc.h
# End Source File
# Begin Source File

SOURCE=.\SPPMaker.h
# End Source File
# Begin Source File

SOURCE=.\SPPView.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# Begin Source File

SOURCE=.\Struct.h
# End Source File
# Begin Source File

SOURCE=.\Svsys.h
# End Source File
# Begin Source File

SOURCE=.\Swathpar.h
# End Source File
# Begin Source File

SOURCE=.\TagLine.h
# End Source File
# Begin Source File

SOURCE=.\TDoc.h
# End Source File
# Begin Source File

SOURCE=.\testDoc.h
# End Source File
# Begin Source File

SOURCE=.\testView.h
# End Source File
# Begin Source File

SOURCE=.\TopoNet.h
# End Source File
# Begin Source File

SOURCE=.\TopoNetDoc.h
# End Source File
# Begin Source File

SOURCE=.\TopoNetView.h
# End Source File
# Begin Source File

SOURCE=.\TView.h
# End Source File
# Begin Source File

SOURCE=.\Unform.h
# End Source File
# Begin Source File

SOURCE=.\ValueFbk.h
# End Source File
# Begin Source File

SOURCE=.\ZDEquation.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\3dfstbrk.ico
# End Source File
# Begin Source File

SOURCE=.\res\3dfstbrk.rc2
# End Source File
# Begin Source File

SOURCE=.\Res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap6.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap7.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap8.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\checkt.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Fhsoft.ico
# End Source File
# Begin Source File

SOURCE=.\res\Fstbrk.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\mainfram.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\menuspp.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\MenuSPP.ico
# End Source File
# Begin Source File

SOURCE=.\Res\MenuT.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Limit.txt
# End Source File
# Begin Source File

SOURCE=.\ProNote.txt
# End Source File
# Begin Source File

SOURCE=.\Repair.txt
# End Source File
# End Target
# End Project
