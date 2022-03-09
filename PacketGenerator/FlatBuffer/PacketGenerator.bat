rem make flatbuffer files

setlocal enabledelayedexpansion

rem common
call generator.bat "flatc.exe --gen-object-api --cpp -o output/cpp/common/ fbs/common/" fbs/common/
call generator.bat "flatc.exe --gen-object-api --csharp --gen-onefile -o output/cs/common/ fbs/common/" fbs/common/

rem login
call generator.bat "flatc.exe --gen-object-api --cpp -o output/cpp/login/ fbs/login/" fbs/login/
call generator.bat "flatc.exe --gen-object-api --csharp --gen-onefile -o output/cs/login/ fbs/login/" fbs/login/

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
START ../Binary/PacketGenerator.exe ../FlatBuffer/output/cs/login/login_protocol.cs client cs login

XCOPY /Y server\cpp\login\LoginPacketHandler.h %serverpath%
XCOPY /Y server\cpp\login\LoginPacketHandler.cpp %serverpath%

XCOPY /Y client\cs\login\LoginPacketManager.cs %clientpath%

endlocal

pause