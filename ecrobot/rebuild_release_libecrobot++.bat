
@echo off

cd %~dp0
cd c++\
make clean
make all
if  %errorlevel% NEQ 0 goto builderror
make release
make doxy

echo --------------DONE!------------------
pause
exit

:builderror
echo ----------BUILD ERROR----------------
pause