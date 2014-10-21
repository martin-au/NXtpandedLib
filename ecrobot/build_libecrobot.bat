
cd %~dp0
cd c\
make all

cd %~dp0
copy libecrobot.a c\
cd c\
make clean

move libecrobot.a ..\

pause