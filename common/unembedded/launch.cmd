@echo off

set home=%~dp0%
set home=%home:~0,-1%

set PATH=%home%\library;%PATH%

"%home%\shell.exe" ^
	"%home%\kernel" ^
	"%home%\script\main.js" ^
	"%home%" ^
	%*

set /p _=Press ^<ENTER^> to continue ... 
set ERRORLEVEL=0
