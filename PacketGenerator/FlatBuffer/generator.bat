@echo off

setlocal enabledelayedexpansion

rem make .h files

set com=flatc.exe -c -o output/ fbs/

FOR /R fbs/ %%F in (*.fbs) do (
set filename=%%~nxF
set exec=%com%!filename!

echo !exec!
!exec!
)

rem copy .h files

set com=XCOPY /Y output\
set client="../../Source/CoreTest/CoreTest/*"
set server="../../Source/CoreTestServer/CoreTestServer/*"

FOR /R output/ %%F in (*.h) do (
set filename=%%~nxF

set execClient=%com%!filename! %client%
echo !execClient!
!execClient!

set execServer=%com%!filename! %server%
echo !execServer!
!execServer!
)

endlocal