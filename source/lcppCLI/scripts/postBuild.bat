@echo off

set "OutDir=%~1"
set "PlatformArchitecture=%~2"

echo Copying SFML dlls...
xcopy /Y /F /D  "%SFML_API%\%PlatformArchitecture%\SFML-2.1\bin\libsndfile-1.dll" "%OutDir%\libsndfile-1.dll*"
xcopy /Y /F /D  "%SFML_API%\%PlatformArchitecture%\SFML-2.1\bin\openal32.dll" "%OutDir%\openal32.dll*"
xcopy /Y /F /D  "%SFML_API%\%PlatformArchitecture%\SFML-2.1\bin\sfml-audio-2.dll" "%OutDir%\sfml-audio-2.dll*"
xcopy /Y /F /D  "%SFML_API%\%PlatformArchitecture%\SFML-2.1\bin\sfml-audio-d-2.dll" "%OutDir%\sfml-audio-d-2.dll*"
xcopy /Y /F /D  "%SFML_API%\%PlatformArchitecture%\SFML-2.1\bin\sfml-graphics-2.dll" "%OutDir%\sfml-graphics-2.dll*"
xcopy /Y /F /D  "%SFML_API%\%PlatformArchitecture%\SFML-2.1\bin\sfml-graphics-d-2.dll" "%OutDir%\sfml-graphics-d-2.dll*"
xcopy /Y /F /D  "%SFML_API%\%PlatformArchitecture%\SFML-2.1\bin\sfml-network-2.dll" "%OutDir%\sfml-network-2.dll*"
xcopy /Y /F /D  "%SFML_API%\%PlatformArchitecture%\SFML-2.1\bin\sfml-network-d-2.dll" "%OutDir%\sfml-network-d-2.dll*"
xcopy /Y /F /D  "%SFML_API%\%PlatformArchitecture%\SFML-2.1\bin\sfml-system-2.dll" "%OutDir%\sfml-system-2.dll*"
xcopy /Y /F /D  "%SFML_API%\%PlatformArchitecture%\SFML-2.1\bin\sfml-system-d-2.dll" "%OutDir%\sfml-system-d-2.dll*"
xcopy /Y /F /D  "%SFML_API%\%PlatformArchitecture%\SFML-2.1\bin\sfml-window-2.dll" "%OutDir%\sfml-window-2.dll*"
xcopy /Y /F /D  "%SFML_API%\%PlatformArchitecture%\SFML-2.1\bin\sfml-window-d-2.dll" "%OutDir%\sfml-window-d-2.dll*"

echo Finished post build script
