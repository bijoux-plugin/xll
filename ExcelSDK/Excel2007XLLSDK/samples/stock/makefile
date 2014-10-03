!if "$(TYPE)" == ""
TYPE=DEBUG
!endif

SDKDIR  =   ..\..
STOCKDIR=	.
STOCKBUILDDIR = $(STOCKDIR)\$(TYPE)
TARGETLIBDIR = $(SDKDIR)\..\lib
TARGETINCDIR = $(SDKDIR)\include
TARGETSRCDIR = $(SDKDIR)\src

PROJ = STOCK

COPY=copy /Y

!if "$(RM)" == ""
RM=-del /f /q
!endif

!if "$(RMDIR)" == ""
RMDIR=-rd /S /Q
!endif

all : TARGET_DELIVER

STOCK_BINARY   = $(PROJ).xll
STOCK_PDB	 = $(STOCKBUILDDIR)\$(PROJ).pdb     
STOCK_INCLUDE  = /I"$(STOCKDIR)" /I"$(TARGETINCDIR)" /I"$(TARGETSRCDIR)"

!if "$(TYPE)" == "DEBUG"
STOCK_LIBS = msvcrtd.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib $(TARGETLIBDIR)\Frmwrk32.lib $(TARGETLIBDIR)\xlcall32.lib
!else
STOCK_LIBS = msvcrt.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib $(TARGETLIBDIR)\Frmwrk32.lib $(TARGETLIBDIR)\xlcall32.lib
!endif

!if "$(TYPE)" == "DEBUG"
STOCK_LIB_OPTS = /OUT:$(STOCK_BINARY) /machine:X86 /subsystem:windows -def:..\stock.def /DEBUG /INCREMENTAL:NO /NODEFAULTLIB:msvcrt.lib
!else
STOCK_LIB_OPTS = /OUT:$(STOCK_BINARY) /machine:X86 /subsystem:windows -def:..\stock.def /RELEASE /NODEFAULTLIB:msvcrtd.lib
!endif

!if "$(TYPE)" == "DEBUG"
STOCK_DEFINES  = /D"WIN32" /D"_WINDOWS" /D"_MBCS" /D"_USRDLL" /D"STOCK_EXPORTS" /D "_DEBUG" 
!else
STOCK_DEFINES  = /D"WIN32" /D"_WINDOWS" /D"_MBCS" /D"_USRDLL" /D"STOCK_EXPORTS" /D "NDEBUG"
!endif

STOCK_OBJ =	stock.obj \
	StockServiceProxy.obj \
	Metrics.obj \
	
.SUFFIXES:	.cpp .c .obj

!if "$(TYPE)" == "DEBUG"
CPPFLAGS        =/Od /W3 /WX /EHsc /Zi /MDd /Fd"$(STOCK_PDB)" /Fo"$(STOCKBUILDDIR)\\"
!else
CPPFLAGS        =/W3 /WX /EHsc /MD /Fo"$(STOCKBUILDDIR)\\"
!endif

minclean:
	$(RM) "$(STOCKBUILDDIR)\*.obj" 1>nul 2>nul
	$(RM) "$(STOCKBUILDDIR)\*.lib" 1>nul 2>nul
	$(RM) "$(STOCKBUILDDIR)\*.exp" 1>nul 2>nul
	$(RM) "$(STOCKBUILDDIR)\*.manifest" 1>nul 2>nul
	$(RM) "$(STOCKBUILDDIR)\*.res" 1>nul 2>nul
!if "$(TYPE)" == "DEBUG"
	$(RM) "$(STOCKBUILDDIR)\*.pdb" 1>nul 2>nul
!endif
	$(RM) StockService.h 1>nul 2>nul

clean: 
	$(RM) "$(STOCKBUILDDIR)\*.*" 1>nul 2>nul
	$(RMDIR) $(STOCKBUILDDIR) 1>nul 2>nul
	
"$(STOCKBUILDDIR)":
	if not exist "$(STOCKBUILDDIR)" mkdir "$(STOCKBUILDDIR)"

TARGET_DELIVER: WEBSERVICE "$(STOCKBUILDDIR)" $(STOCK_OBJ)
	cd $(STOCKBUILDDIR)
	link.exe /DLL /nologo $(STOCK_OBJ) $(STOCK_LIB_OPTS) $(STOCK_LIBS)
	mt.exe /nologo /outputresource:"$(STOCK_BINARY);#2" /manifest "$(STOCK_BINARY).manifest"
	
WEBSERVICE:
	sproxy.exe "$(STOCKDIR)\StockService\results.discomap" /out:"$(STOCKDIR)\StockService.h" /nologo
	
.cpp.obj:
	$(CPP) /nologo $(CPPFLAGS) $(STOCK_INCLUDE) $(STOCK_DEFINES) /c $<

.c.obj:
	$(CPP) /nologo $(CPPFLAGS) $(STOCK_INCLUDE) $(STOCK_DEFINES) /c $<	
	