using System;
using System.IO;
using System.Collections.Generic;
using System.Text.RegularExpressions;

namespace PacketGenerator
{
    class Program
    {
        static string serverRegister;
        static string clientRegister;

        static void Main(string[] args)
        {
            string file = "../../../FlatBuffer/output/cpp/login_protocol_generated.h";
            string type = "server";
            string language = "cpp";

            if (args.Length >= 1)
            {
                file = args[0];
                type = args[1];
                language = args[2];
            }

            if (type == "server")
            {
                if (language == "cpp")
                    MakeServerPacketForCpp(file);
                else
                    MakeServerPacketForCS(file);
            }

            else
            {
                if (language == "cpp")
                    MakeClientPacketForCpp(file);
                else
                    MakeClientPacketForCS(file);
            }
        }

        public static void MakeServerPacketForCpp(string file)
        {
            string nameSpaceName = ParseNamespaceName(file);

            serverRegister += string.Format(PacketFormatForCpp.FunctionBasicFormat, nameSpaceName, "std::shared_ptr<CoreClientSession> session");

            List<string> packetNames = ParsePacketName(file);
            foreach (string packetName in packetNames)
            {
                string[] funcNames = packetName.Trim().Split("Packet_");

                if (packetName.Contains("CS"))
                    serverRegister += string.Format(PacketFormatForCpp.FunctionRegisterFormat, nameSpaceName, packetName, funcNames[1]);
            }

            serverRegister += PacketFormatForCpp.FunctionRegisterEndFormat;

            string fileName = string.Format(PacketFormatForCpp.FileName, nameSpaceName);

            CheckDirectoryAndCreate("Server/cpp");

            File.WriteAllText("Server/cpp/" + fileName, serverRegister);
        }
        public static void MakeServerPacketForCS(string file)
        {

        }
        public static void MakeClientPacketForCpp(string file)
        {
            string nameSpaceName = ParseNamespaceName(file);

            clientRegister += string.Format(PacketFormatForCpp.FunctionBasicFormat, nameSpaceName, "CoreServerSession& session");

            List<string> packetNames = ParsePacketName(file);
            foreach (string packetName in packetNames)
            {
                string[] funcNames = packetName.Trim().Split("Packet_");

                if (packetName.Contains("SC"))
                    clientRegister += string.Format(PacketFormatForCpp.FunctionRegisterFormat, nameSpaceName, packetName, funcNames[1]);
            }

            clientRegister += PacketFormatForCpp.FunctionRegisterEndFormat;

            string fileName = string.Format(PacketFormatForCpp.FileName, nameSpaceName);

            CheckDirectoryAndCreate("Client/cpp");

            File.WriteAllText("Client/cpp/" + fileName, clientRegister);
        }

        public static void MakeClientPacketForCS(string file)
        {

        }

        static string ParseNamespaceName(string file)
        {
            foreach (string line in File.ReadAllLines(file))
            {
                if (line.Contains("namespace"))
                {
                    string name = Regex.Replace(line, "namespace ", string.Empty);
                    name = Regex.Replace(name, "{", string.Empty);
                    name = name.Trim();
                    return name;
                }
            }

            return "";
        }

        static List<string> ParsePacketName(string file)
        {
            bool isFoundStartPos = false;
            List<string> packetNames = new List<string>();

            foreach (string line in File.ReadAllLines(file))
            {
                if (isFoundStartPos)
                {
                    string[] names = line.Trim().Split(" =");
                    if (names.Length == 0)
                        continue;

                    if (names[0] == "Packet_MIN")
                        return packetNames;

                    packetNames.Add(names[0]);
                }

                if (line.Contains("Packet_NONE"))
                {
                    isFoundStartPos = true;
                }
            }

            return packetNames;
        }

        static void CheckDirectoryAndCreate(string name)
        {
            DirectoryInfo dir = new DirectoryInfo(name);
            if (!dir.Exists)
                dir.Create();
        }
    }
}
