call generator.bat

START ../Binary/PacketGenerator.exe ../FlatBuffer/output/login_protocol_generated.h

XCOPY /Y Server\LoginPacketHandler.h "../../Source/CoreTestServer/CoreTestServer/*"
XCOPY /Y Server\LoginPacketHandler.cpp "../../Source/CoreTestServer/CoreTestServer/*"

XCOPY /Y Client\LoginPacketHandler.h "../../Source/CoreTest/CoreTest/*"
XCOPY /Y Client\LoginPacketHandler.cpp "../../Source/CoreTest/CoreTest/*"

pause