@echo on

echo info: OutDir ................. %OutDir%
echo info: PlatformArchitecture ... %PlatformArchitecture%
echo.

if errorlevel 1 goto failure

echo Information: Finished post build script successfully
exit 0

:failure
echo Error: Post build script did not succeed
exit 1
