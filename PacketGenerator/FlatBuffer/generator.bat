@echo off

setlocal enabledelayedexpansion

set com=%~1

FOR /R fbs/ %%F in (*.fbs) do (
set filename=%%~nxF
set exec=%com%!filename!

echo !exec!
!exec!
)

endlocal