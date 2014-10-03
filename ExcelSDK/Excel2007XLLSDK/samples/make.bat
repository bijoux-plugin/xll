@echo off

set TYPE=DEBUG
set OP=

if "%1" == "" goto usage

:nextarg
	shift

if "%0" == ""   goto build
if /i "%0" == "ship" (set TYPE=RELEASE)
if /i "%0" == "release" (set TYPE=RELEASE)
if /i "%0" == "clean" (set OP=clean)
if /i "%0" == "minclean" (set OP=minclean)
goto nextarg

:usage
	echo.
	echo usage:
	echo.
	echo make TYPE OP
	echo.
	echo TYPE = DEBUG^|RELEASE^|SHIP
	echo.
	echo if no type is specified, DEBUG is assumed.
	echo SHIP and RELEASE refer to same type
	echo.
	echo OP = MINCLEAN^|CLEAN^|BUILD
	echo.
	echo if no OP is specified BUILD is assumed.
	echo.
	echo MINCLEAN deletes all build files expect for the 
	echo buildlog and generated LIB or XLL file(s)
	echo.
	echo CLEAN deletes all build files and the build folder
	echo.
	echo either OP or TYPE must be specified
	echo.
	goto eof

:build
	@call "%VCINSTALLDIR%\VC\bin\vcvars32.bat"
	@for /F %%d in (targets) do @call build.bat %%d %OP%
:eof