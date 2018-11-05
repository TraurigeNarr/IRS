@echo off

set BUILD_DIR=Builds

set PLATFORM=x86

IF [%1] == [] (
	set PLATFORM=x86
) ELSE (
	set PLATFORM=%1
)

set TARGET_DIR=%BUILD_DIR%\%PLATFORM%

if not exist %BUILD_DIR%\%PLATFORM% mkdir %BUILD_DIR%\%PLATFORM%

set SOURCE_DIR=Binaries\%PLATFORM%\ReleaseStatic
echo Copy from "%SOURCE_DIR%" to "%TARGET_DIR%"
xcopy /I /R /Y /Q Binaries\%PLATFORM%\"ReleaseStatic"\*.dll %TARGET_DIR%\
xcopy /I /R /Y /Q Binaries\%PLATFORM%\"ReleaseStatic"\*.exe %TARGET_DIR%\
xcopy /I /R /Y /Q Binaries\%PLATFORM%\"ReleaseStatic"\*.xml %TARGET_DIR%\
xcopy /I /R /Y /Q Binaries\%PLATFORM%\"ReleaseStatic"\*.ttf %TARGET_DIR%\
