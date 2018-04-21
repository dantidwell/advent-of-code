SET VSCMD_START_DIR=%CD%\bin\%1

call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
set compilerflags=/std:c++latest /Od /Zi /EHsc /I ..\..\
set linkerflags=/OUT:%2.exe

cl.exe %compilerflags% ..\..\%1\%2.cpp
