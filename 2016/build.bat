@echo off

SET VSCMD_START_DIR=%CD%\bin\%1
mkdir %VSCMD_START_DIR%
pushd %VSCMD_START_DIR%

if defined VSCMD_ENV_SET (
  echo
) else (
  call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
  SET VSCMD_ENV_SET=1
) 
set compilerflags=/std:c++latest /O2 /Zi /EHsc /I ..\..\
set linkerflags=/Fe:%1.exe 

copy ..\..\%1\%1.txt .\
cl.exe %compilerflags% ..\..\%1\main.cpp %linkerflags% 

popd