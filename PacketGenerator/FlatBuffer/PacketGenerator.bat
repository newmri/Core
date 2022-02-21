rem make flatbuffer files

setlocal enabledelayedexpansion

call generator.bat "flatc.exe --cpp -o output/cpp/ fbs/"
call generator.bat "flatc.exe --csharp --gen-onefile -o output/cs/ fbs/"

rem copy flatbuffer files

set loginpath="../../Source/LoginServer/LoginServer/*"
set clientpath="../../Source/Client/Assets/Scripts/Network/*"

call copy.bat %loginpath% cpp
call copy.bat %clientpath% cs

rem copy PacketHandler

START ../Binary/PacketGenerator.exe ../FlatBuffer/output/cpp/login_protocol_generated.h server cpp

XCOPY /Y Server\cpp\LoginPacketHandler.h %loginpath%
XCOPY /Y Server\cpp\LoginPacketHandler.cpp %loginpath%


endlocal

pause