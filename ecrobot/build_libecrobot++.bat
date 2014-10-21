
@echo off

cd %~dp0
cd c++\
make all
if  %errorlevel% NEQ 0 goto errorcleanup

cd %~dp0
copy "libecrobot++.a" "c++\"
cd c++\
make clean

move "libecrobot++.a" ..\

echo --------------DONE!------------------
pause
exit

:errorcleanup
make clean
echo ----------BUILD ERROR----------------
pause
