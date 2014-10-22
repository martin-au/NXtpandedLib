
@echo off

cd %~dp0
make clean
make all
if  %errorlevel% NEQ 0 goto builderror
make release
make doxy

rmdir /s /q AutoDocumentation\html
move html AutoDocumentation

echo --------------DONE!------------------
pause
exit

:builderror
echo ----------BUILD ERROR----------------
pause