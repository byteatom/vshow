set QT_DIR=D:\Qt\online
set QT_VER=5.4

call deploy-one.bat x86 release %QT_VER%
call deploy-one.bat x86 debug %QT_VER%
call deploy-one.bat x86_64 release %QT_VER%
call deploy-one.bat x86_64 debug %QT_VER%

