
@echo off

cd %~dp0
cd c++\
make all
if  %errorlevel% NEQ 0 goto builderror

echo --------------DONE!------------------
pause
exit

:builderror
echo ----------BUILD ERROR----------------
pause
