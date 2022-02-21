@echo off

setlocal enabledelayedexpansion

set com=XCOPY /Y output\%2\
set copypath=%1

FOR /R output/%2/ %%F in (*.*) do (
set filename=%%~nxF
set exec=%com%!filename! %copypath%

echo !exec!
!exec!
)

endlocal