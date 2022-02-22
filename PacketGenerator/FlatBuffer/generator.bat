@echo off

setlocal enabledelayedexpansion

set com=%~1
set fbspath=%2

FOR /R %fbspath% %%F in (*.fbs) do (
set filename=%%~nxF
set exec=%com%!filename!

echo !exec!
!exec!
)

endlocal