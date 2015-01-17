
@echo off

setlocal
set rootdir=%cd%
for /d %%i in (%rootdir%\*) do (
  cd %%i
  make all
  if "%errorlevel%"=="1" cls &Echo Fail
)
pause
endlocal