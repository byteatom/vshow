del /F /S /Q %1\*.xml
del /F /S /Q %1\*.cfg
del /F /S /Q %1\*.ilk
del /F /S /Q %1\*.pdb
del /F /S /Q %1\*.scn
rmdir /s /q %1\tmp
rmdir /s /q %1\temp
rmdir /s /q %1\log