IF %1==x86_64 (set ARCH_FIX=_64) ELSE (set ARCH_FIX=)
set QT_LIB_DIR=%QT_DIR%\%3\msvc2013%ARCH_FIX%_opengl
set QT_LIB_BIN_DIR=%QT_LIB_DIR%\bin
set QT_LIB_PLUGIN_DIR=%QT_LIB_DIR%\plugins
set CREATOR_DIR=%QT_DIR%\Tools\QtCreator\bin

call copy_cmd.bat %QT_LIB_PLUGIN_DIR%\platforms\qminimal%BUILD_FIX%.dll %DST_DIR%\platforms\
call copy_cmd.bat %QT_LIB_PLUGIN_DIR%\platforms\qwindows%BUILD_FIX%.dll %DST_DIR%\platforms\
call copy_cmd.bat %QT_LIB_PLUGIN_DIR%\imageformats\qdds%BUILD_FIX%.dll %DST_DIR%\imageformats\
call copy_cmd.bat %QT_LIB_PLUGIN_DIR%\imageformats\qgif%BUILD_FIX%.dll %DST_DIR%\imageformats\
call copy_cmd.bat %QT_LIB_PLUGIN_DIR%\imageformats\qicns%BUILD_FIX%.dll %DST_DIR%\imageformats\
call copy_cmd.bat %QT_LIB_PLUGIN_DIR%\imageformats\qico%BUILD_FIX%.dll %DST_DIR%\imageformats\
call copy_cmd.bat %QT_LIB_PLUGIN_DIR%\imageformats\qjp2%BUILD_FIX%.dll %DST_DIR%\imageformats\
call copy_cmd.bat %QT_LIB_PLUGIN_DIR%\imageformats\qjpeg%BUILD_FIX%.dll %DST_DIR%\imageformats\
call copy_cmd.bat %QT_LIB_PLUGIN_DIR%\imageformats\qmng%BUILD_FIX%.dll %DST_DIR%\imageformats\
call copy_cmd.bat %QT_LIB_PLUGIN_DIR%\imageformats\qsvg%BUILD_FIX%.dll %DST_DIR%\imageformats\
call copy_cmd.bat %QT_LIB_PLUGIN_DIR%\imageformats\qtga%BUILD_FIX%.dll %DST_DIR%\imageformats\
call copy_cmd.bat %QT_LIB_PLUGIN_DIR%\imageformats\qtiff%BUILD_FIX%.dll %DST_DIR%\imageformats\
call copy_cmd.bat %QT_LIB_PLUGIN_DIR%\imageformats\qwbmp%BUILD_FIX%.dll %DST_DIR%\imageformats\
call copy_cmd.bat %QT_LIB_PLUGIN_DIR%\imageformats\qwebp%BUILD_FIX%.dll %DST_DIR%\imageformats\
call copy_cmd.bat %QT_LIB_BIN_DIR%\icu*.dll %DST_DIR%
call copy_cmd.bat %QT_LIB_BIN_DIR%\Qt5Core%BUILD_FIX%.dll %DST_DIR%
call copy_cmd.bat %QT_LIB_BIN_DIR%\Qt5Gui%BUILD_FIX%.dll %DST_DIR%
call copy_cmd.bat %QT_LIB_BIN_DIR%\Qt5Network%BUILD_FIX%.dll %DST_DIR%
call copy_cmd.bat %QT_LIB_BIN_DIR%\Qt5Svg%BUILD_FIX%.dll %DST_DIR%
call copy_cmd.bat %QT_LIB_BIN_DIR%\Qt5Widgets%BUILD_FIX%.dll %DST_DIR%
call copy_cmd.bat %CREATOR_DIR%\libEGL%BUILD_FIX%.dll %DST_DIR%
call copy_cmd.bat %CREATOR_DIR%\libGLESv2%BUILD_FIX%.dll %DST_DIR%