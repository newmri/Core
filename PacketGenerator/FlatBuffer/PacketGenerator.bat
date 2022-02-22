rem make flatbuffer files

setlocal enabledelayedexpansion

rem common
call generator.bat "flatc.exe --cpp -o output/cpp/common/ fbs/common/" fbs/common/
call generator.bat "flatc.exe --csharp --gen-onefile -o output/cs/common/ fbs/common/" fbs/common/

rem login
call generator.bat "flatc.exe --cpp -o output/cpp/login/ fbs/login/" fbs/login/
call generator.bat "flatc.exe --csharp --gen-onefile -o output/cs/login/ fbs/login/" fbs/login/

rem copy flatbuffer files

rem common
set serverpath="../../Source/LoginServer/LoginServer/*"
set clientpath="../../Source/Client/Assets/Scripts/Network/*"

call copy.bat %serverpath% cpp\common
call copy.bat %clientpath% cs\common

rem login
set serverpath="../../Source/LoginServer/LoginServer/*"
set clientpath="../../Source/Client/Assets/Scripts/Network/LoginServer/*"

call copy.bat %serverpath% cpp\login
call copy.bat %clientpath% cs\login

rem copy PacketHandler

START ../Binary/PacketGenerator.exe ../FlatBuffer/output/cpp/login/login_protocol_generated.h server cpp login

XCOPY /Y server\cpp\login\LoginPacketHandler.h %serverpath%
XCOPY /Y server\cpp\login\LoginPacketHandler.cpp %serverpath%


endlocal

pause