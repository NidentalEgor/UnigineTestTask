@ECHO OFF
set CMAKE_BUILD_TYPE=%1
if "%CMAKE_BUILD_TYPE%"=="Debug" goto build_type_ok
if "%CMAKE_BUILD_TYPE%"=="Release" goto build_type_ok
if not "%CMAKE_BUILD_TYPE%"==""    goto build_type_error
set CMAKE_BUILD_TYPE=Release

:build_type_ok
set BUILD_DIRECTORY=VS_15\%CMAKE_BUILD_TYPE%

if not exist %BUILD_DIRECTORY% mkdir %BUILD_DIRECTORY%
cd %BUILD_DIRECTORY%

cmake -DCMAKE_BUILD_TYPE=%CMAKE_BUILD_TYPE% -G"Visual Studio 14 2015" ..\..\..\

cd ..\..\
goto seccess

:build_type_error
echo Unknown build type "%CMAKE_BUILD_TYPE%"
exit /b 1

:seccess
echo ======================== Success! All Done ============================