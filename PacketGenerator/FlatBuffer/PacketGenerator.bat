rem make flatbuffer files

setlocal enabledelayedexpansion

rem common
call generator.bat "flatc.exe --gen-object-api --cpp -o output/cpp/common/ fbs/common/" fbs/common/
call generator.bat "flatc.exe --gen-object-api --csharp --gen-onefile -o output/cs/common/ fbs/common/" fbs/common/

rem login
call generator.bat "flatc.exe --gen-object-api --cpp -o output/cpp/login/ fbs/login/" fbs/login/
call generator.bat "flatc.exe --gen-object-api --csharp --gen-onefile -o output/cs/login/ fbs/login/" fbs/login/

rem game
call generator.bat "flatc.exe --gen-object-api --cpp -o output/cpp/game/ fbs/game/" fbs/game/
call generator.bat "flatc.exe --gen-object-api --csharp --gen-onefile -o output/cs/game/ fbs/game/" fbs/game/

rem copy flatbuffer files

rem common
set serverpath="../../Source/Core/Core/*"
set clientpath="../../Source/Client/Assets/Scripts/Network/*"
set dummyclientpath="../../Source/DummyClient/DummyClient/*"

call copy.bat %serverpath% cpp\common
call copy.bat %clientpath% cs\common

rem login
set clientpath="../../Source/Client/Assets/Scripts/Network/LoginServer/*"

call copy.bat %serverpath% cpp\login
call copy.bat %clientpath% cs\login

rem copy LoginPacketHandler

set serverpath="../../Source/LoginServer/LoginServer/*"

START ../Binary/PacketGenerator.exe ../FlatBuffer/output/cpp/login/login_protocol_generated.h server cpp login
START ../Binary/PacketGenerator.exe ../FlatBuffer/output/cs/login/login_protocol.cs client cs login
START ../Binary/PacketGenerator.exe ../FlatBuffer/output/cpp/login/login_protocol_generated.h client cpp login

XCOPY /Y server\cpp\login\LoginPacketHandler.h %serverpath%
XCOPY /Y server\cpp\login\LoginPacketHandler.cpp %serverpath%

XCOPY /Y client\cs\login\LoginPacketManager.cs %clientpath%

XCOPY /Y client\cpp\login\LoginPacketHandler.h %dummyclientpath%
XCOPY /Y client\cpp\login\LoginPacketHandler.cpp %dummyclientpath%

rem game
set serverpath="../../Source/Core/Core/*"
set clientpath="../../Source/Client/Assets/Scripts/Network/GameServer/*"

call copy.bat %serverpath% cpp\game
call copy.bat %clientpath% cs\game

rem copy GamePacketHandler
set serverpath="../../Source/GameServer/GameServer/*"

START ../Binary/PacketGenerator.exe ../FlatBuffer/output/cpp/game/game_protocol_generated.h server cpp game
START ../Binary/PacketGenerator.exe ../FlatBuffer/output/cs/game/game_protocol.cs client cs game
START ../Binary/PacketGenerator.exe ../FlatBuffer/output/cpp/game/game_protocol_generated.h client cpp game

XCOPY /Y server\cpp\game\GamePacketHandler.h %serverpath%
XCOPY /Y server\cpp\game\GamePacketHandler.cpp %serverpath%

XCOPY /Y client\cs\game\GamePacketManager.cs %clientpath%

XCOPY /Y client\cpp\game\GamePacketHandler.h %dummyclientpath%
XCOPY /Y client\cpp\game\GamePacketHandler.cpp %dummyclientpath%

endlocal

pause