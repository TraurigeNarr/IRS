@echo off
SET SolutionDir=%1
SET ConfigurationName=%2
SET DestDir=%SolutionDir%..\Binaries\%ConfigurationName%

xcopy /Y /R /I %THIRD_PARTIES%\freetype\bin\freetype6.dll %DestDir%
xcopy /Y /R /I %THIRD_PARTIES%\freetype\bin\zlib1.dll %DestDir%
xcopy /Y /R /I %THIRD_PARTIES%\opengl\freeglut_2.8.1.1\bin\freeglut.dll %DestDir%

SET FontPath=%SolutionDir%font.ttf
xcopy /Y /R /I %FontPath% %DestDir%
