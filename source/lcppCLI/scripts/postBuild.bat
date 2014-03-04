@echo on

echo info: OutDir ................. %OutDir%
echo info: PlatformArchitecture ... %PlatformArchitecture%
echo.

echo Copying SFML dlls...
xcopy /Y /F /D "%SFML_API%\%PlatformArchitecture%\bin\libsndfile-1.dll"      "%OutDir%\libsndfile-1.dll*"
xcopy /Y /F /D "%SFML_API%\%PlatformArchitecture%\bin\openal32.dll"          "%OutDir%\openal32.dll*"
xcopy /Y /F /D "%SFML_API%\%PlatformArchitecture%\bin\sfml-audio-2.dll"      "%OutDir%\sfml-audio-2.dll*"
xcopy /Y /F /D "%SFML_API%\%PlatformArchitecture%\bin\sfml-audio-d-2.dll"    "%OutDir%\sfml-audio-d-2.dll*"
xcopy /Y /F /D "%SFML_API%\%PlatformArchitecture%\bin\sfml-graphics-2.dll"   "%OutDir%\sfml-graphics-2.dll*"
xcopy /Y /F /D "%SFML_API%\%PlatformArchitecture%\bin\sfml-graphics-d-2.dll" "%OutDir%\sfml-graphics-d-2.dll*"
xcopy /Y /F /D "%SFML_API%\%PlatformArchitecture%\bin\sfml-network-2.dll"    "%OutDir%\sfml-network-2.dll*"
xcopy /Y /F /D "%SFML_API%\%PlatformArchitecture%\bin\sfml-network-d-2.dll"  "%OutDir%\sfml-network-d-2.dll*"
xcopy /Y /F /D "%SFML_API%\%PlatformArchitecture%\bin\sfml-system-2.dll"     "%OutDir%\sfml-system-2.dll*"
xcopy /Y /F /D "%SFML_API%\%PlatformArchitecture%\bin\sfml-system-d-2.dll"   "%OutDir%\sfml-system-d-2.dll*"
xcopy /Y /F /D "%SFML_API%\%PlatformArchitecture%\bin\sfml-window-2.dll"     "%OutDir%\sfml-window-2.dll*"
xcopy /Y /F /D "%SFML_API%\%PlatformArchitecture%\bin\sfml-window-d-2.dll"   "%OutDir%\sfml-window-d-2.dll*"

echo.

if errorlevel 1 goto failure

echo Information: Finished post build script successfully
exit 0

:failure
echo Error: Post build script did not succeed
exit 1
