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
@"{0}Handler.cpp";

        public static string FunctionBasicFormat =
@"#include ""Include.h""

{0}Handler::{0}Handler()
{{
    Register();
}}

void {0}Handler::Handle({1}, const {0}::Packet packetID, const void* data)
{{
	if (this->func.end() == this->func.find(packetID))
		return;

	this->func[packetID](this->packetFunc, session, data);
}}

void {0}Handler::Register(void)
{{
";

        public static string FunctionRegisterFormat =
@"    this->func[{0}::{1}] = &{0}Func::{2};
";
        public static string FunctionRegisterEndFormat =
@"}
";
    }
}
