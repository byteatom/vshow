set DST_DIR=vshow-%1-%2-qt%3\
IF %2==debug (set BUILD_FIX=d) ELSE (set BUILD_FIX=)

call copy_cmd.bat ..\lib\xerces-3.1.1\%1\xerces-c_3_1%BUILD_FIX%.dll %DST_DIR%

call deploy-vlc-qt.bat %1 %2 %3

call deploy-qt.bat %1 %2 %3

call copy_cmd.bat ..\lib\vcrt\%1-%2\* %DST_DIR%

call copy_cmd.bat ..\asset\* %DST_DIR%