
# set up vs environment
@REM Keep VSCode working directory
REM SET
SET VSCMD_START_DIR=%CD%/bin

call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
set compilerflags=/std:c++latest /Od /Zi /EHsc /I ..\ext\include
set libs=advapi32.lib iphlpapi.lib msvcrtd.lib psapi.lib shell32.lib user32.lib userenv.lib ws2_32.lib libuv.lib
set linkerflags=/OUT:studex.exe /LIBPATH:..\ext\lib /NODEFAULTLIB:libcmtd /NODEFAULTLIB:libcmt
cl.exe %compilerflags% ../studex.cpp ../parse.cpp ../routing.cpp ../server.cpp ../search.cpp %libs% /link %linkerflags%

cl.exe /std:c++latest ../$year/$problem.cpp
