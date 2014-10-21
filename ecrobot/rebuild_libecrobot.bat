
@echo off

cd %~dp0
cd c\
make clean
make all
if  %errorlevel% NEQ 0 goto builderror

cd %~dp0
copy libecrobot.a c\
cd c\
make clean

move libecrobot.a ..\

echo --------------DONE!------------------
pause
exit

:builderror
echo ----------BUILD ERROR----------------
pause