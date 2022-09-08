@echo off
REM -- First make map file from App Studio generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by 3DFSTBRK.HPJ. >hlp\3dfstbrk.hm
echo. >>hlp\3dfstbrk.hm
echo // Commands (ID_* and IDM_*) >>hlp\3dfstbrk.hm
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>hlp\3dfstbrk.hm
echo. >>hlp\3dfstbrk.hm
echo // Prompts (IDP_*) >>hlp\3dfstbrk.hm
makehm IDP_,HIDP_,0x30000 resource.h >>hlp\3dfstbrk.hm
echo. >>hlp\3dfstbrk.hm
echo // Resources (IDR_*) >>hlp\3dfstbrk.hm
makehm IDR_,HIDR_,0x20000 resource.h >>hlp\3dfstbrk.hm
echo. >>hlp\3dfstbrk.hm
echo // Dialogs (IDD_*) >>hlp\3dfstbrk.hm
makehm IDD_,HIDD_,0x20000 resource.h >>hlp\3dfstbrk.hm
echo. >>hlp\3dfstbrk.hm
echo // Frame Controls (IDW_*) >>hlp\3dfstbrk.hm
makehm IDW_,HIDW_,0x50000 resource.h >>hlp\3dfstbrk.hm
REM -- Make help for Project 3DFSTBRK
call hc31 3dfstbrk.hpj
echo.
