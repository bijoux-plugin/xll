!if "$(TYPE)" == ""
TYPE=DEBUG
!endif

SDKDIR  =   ..\..
EXAMPLEDIR=	.
EXAMPLEBUILDDIR = $(EXAMPLEDIR)\$(TYPE)
TARGETLIBDIR = $(SDKDIR)\..\lib
TARGETINCDIR = $(SDKDIR)\include
TARGETSRCDIR = $(SDKDIR)\src

PROJ = EXAMPLE

COPY=copy /Y

!if "$(RM)" == ""
RM=-del /f /q
!endif

!if "$(RMDIR)" == ""
RMDIR=-rd /S /Q
!endif

all : TARGET_DELIVER

EXAMPLE_BINARY   = $(PROJ).xll
EXAMPLE_PDB	 = $(EXAMPLEBUILDDIR)\$(PROJ).pdb  
EXAMPLE_INCLUDE  = /I"$(EXAMPLEDIR)" /I"$(TARGETINCDIR)" /I"$(TARGETSRCDIR)"

!if "$(TYPE)" == "DEBUG"
EXAMPLE_LIBS = msvcrtd.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib $(TARGETLIBDIR)\Frmwrk32.lib $(TARGETLIBDIR)\xlcall32.lib
!else
EXAMPLE_LIBS = msvcrt.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib $(TARGETLIBDIR)\Frmwrk32.lib $(TARGETLIBDIR)\xlcall32.lib
!endif

!if "$(TYPE)" == "DEBUG"
EXAMPLE_LIB_OPTS = /OUT:$(EXAMPLE_BINARY) /machine:X86 /subsystem:windows -def:..\example.def /DEBUG /INCREMENTAL:NO
!else
EXAMPLE_LIB_OPTS = /OUT:$(EXAMPLE_BINARY) /machine:X86 /subsystem:windows -def:..\example.def /RELEASE /NODEFAULTLIB:msvcrtd.lib
!endif

!if "$(TYPE)" == "DEBUG"
EXAMPLE_DEFINES  = /D"WIN32" /D"_WINDOWS" /D"_MBCS" /D"_USRDLL" /D"EXAMPLE_EXPORTS" /D "_DEBUG" 
!else
EXAMPLE_DEFINES  = /D"WIN32" /D"_WINDOWS" /D"_MBCS" /D"_USRDLL" /D"EXAMPLE_EXPORTS" /D "NDEBUG"
!endif

EXAMPLE_OBJ =	EXAMPLE.obj

.SUFFIXES:	.cpp .c .obj

!if "$(TYPE)" == "DEBUG"
CPPFLAGS        =/Od /W3 /WX /EHsc /Zi /MDd /Fd"$(EXAMPLE_PDB)" /Fo"$(EXAMPLEBUILDDIR)\\"
!else
CPPFLAGS        =/W3 /WX /EHsc /MD /Fo"$(EXAMPLEBUILDDIR)\\"
!endif

minclean:
	$(RM) "$(EXAMPLEBUILDDIR)\*.obj" 1>nul 2>nul
	$(RM) "$(EXAMPLEBUILDDIR)\*.lib" 1>nul 2>nul
	$(RM) "$(EXAMPLEBUILDDIR)\*.exp" 1>nul 2>nul
	$(RM) "$(EXAMPLEBUILDDIR)\*.manifest" 1>nul 2>nul
!if "$(TYPE)" == "DEBUG"
	$(RM) "$(EXAMPLEBUILDDIR)\*.pdb" 1>nul 2>nul
!endif

clean: 
	$(RM) "$(EXAMPLEBUILDDIR)\*.*" 1>nul 2>nul
	$(RMDIR) $(EXAMPLEBUILDDIR) 1>nul 2>nul
	
"$(EXAMPLEBUILDDIR)":
	if not exist "$(EXAMPLEBUILDDIR)" mkdir "$(EXAMPLEBUILDDIR)"

TARGET_DELIVER: "$(EXAMPLEBUILDDIR)" $(EXAMPLE_OBJ)
	cd $(EXAMPLEBUILDDIR)
	link.exe /DLL /nologo $(EXAMPLE_OBJ) $(EXAMPLE_LIB_OPTS) $(EXAMPLE_LIBS)
	mt.exe /nologo /outputresource:"$(EXAMPLE_BINARY);#2" /manifest "$(EXAMPLE_BINARY).manifest"
	
.cpp.obj:
	$(CPP) /nologo $(CPPFLAGS) $(EXAMPLE_INCLUDE) $(EXAMPLE_DEFINES) /c $<

.c.obj:
	$(CPP) /nologo $(CPPFLAGS) $(EXAMPLE_INCLUDE) $(EXAMPLE_DEFINES) /c $<
