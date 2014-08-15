@echo off

set unitTestExecutable=lcppFoundation_unittests.exe
set codeCoverageExecutable="C:\Program Files (x86)\Microsoft Visual Studio 12.0\Team Tools\Dynamic Code Coverage Tools\amd64\CodeCoverage.exe"

set coverageOutput=..\..\temp\lcppcore_unittests\codeCoverageResults.coverage
set analyzationOutput=..\..\temp\lcppcore_unittests\codeCoverageResults.xml

%codeCoverageExecutable% collect /output:%coverageOutput% %unitTestExecutable% --no-pause > nul
