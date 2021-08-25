@echo off
set SSI_BUILD_WITH_DEBUG_TOOLS=true

set SCRIPT_DIRECTORY=%~dp0
cd "%SCRIPT_DIRECTORY%\.."

set "VSDEVCMD_2019_ENTERPRISE=C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\Common7\Tools\VsDevCmd.bat"
set "VSDEVCMD_2019_PROFESSIONAL=C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\Common7\Tools\VsDevCmd.bat"
set "VSDEVCMD_2019_COMMUNITY=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat"

if exist "%VSDEVCMD_2019_ENTERPRISE%" (
    echo Use VS2019 Enterprise
    call "%VSDEVCMD_2019_ENTERPRISE%" -arch=amd64
) else if exist "%VSDEVCMD_2019_PROFESSIONAL%" (
    echo Use VS2019 Professional
    call "%VSDEVCMD_2019_PROFESSIONAL%" -arch=amd64
) else if exist "%VSDEVCMD_2019_COMMUNITY%" (
    echo Use VS2019 Community
    call "%VSDEVCMD_2019_COMMUNITY%" -arch=amd64
) else (
    echo Error: VS2019 is not installed. 1>&2
    exit /b 1
)

cmake -B Build -G "Visual Studio 16 2019" .

cmake --build Build
