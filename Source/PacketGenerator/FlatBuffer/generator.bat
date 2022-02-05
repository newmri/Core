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
set dest="../../CoreTest/CoreTest"

FOR /R output/ %%F in (*.h) do (
set filename=%%~nxF
set exec=%com%!filename! %dest%

echo !exec!
!exec!
)

endlocal

pause