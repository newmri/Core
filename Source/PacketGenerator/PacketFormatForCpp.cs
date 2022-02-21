using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PacketGenerator
{
    class PacketFormatForCpp
    {
        public static string FileName =
@"{0}PacketHandler.cpp";

        public static string FunctionBasicFormat =
@"#include ""Include.h""

{0}PacketHandler::{0}PacketHandler()
{{
    Register();
}}

void {0}PacketHandler::Handle({1}, const {0}::Packet packetID, const void* data)
{{
	if (func.end() == func.find(packetID))
		return;

	func[packetID](this->packetFunc, session, data);
}}

void {0}PacketHandler::Register(void)
{{
";

        public static string FunctionRegisterFormat =
@"    this->func[{0}::{1}] = &{0}PacketFunc::{2};
";
        public static string FunctionRegisterEndFormat =
@"}
";
    }
}
