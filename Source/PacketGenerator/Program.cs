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

        //static string file = "../../../../../PacketGenerator/output/cpp/login/login_protocol_generated.h";
        //static string type = "server";
        //static string language = "cpp";
        //static string serverName = "login";

        static string file = "../../../../../PacketGenerator/FlatBuffer/output/cs/login/login_protocol.cs";
        static string type = "client";
        static string language = "cs";
        static string serverName = "login";

        static void Main(string[] args)
        {
            if (args.Length >= 1)
            {
                file = args[0];
                type = args[1];
                language = args[2];
                serverName = args[3];
            }

            if (type == "server")
            {
                if (language == "cpp")
                    MakeServerPacketForCpp();
                else
                    MakeServerPacketForCS();
            }

            else
            {
                if (language == "cpp")
                    MakeClientPacketForCpp();
                else
                    MakeClientPacketForCS();
            }
        }

        public static void MakeServerPacketForCpp()
        {
            string nameSpaceName = ParseNamespaceName();

            serverRegister += string.Format(PacketFormatForCpp.FunctionBasicFormat, nameSpaceName, "std::shared_ptr<CoreClientSession> session");

            List<string> packetNames = ParsePacketName("Packet_NONE", "Packet_MIN");
            foreach (string packetName in packetNames)
            {
                string[] funcNames = packetName.Trim().Split("Packet_");

                if (packetName.Contains("CS"))
                    serverRegister += string.Format(PacketFormatForCpp.FunctionRegisterFormat, nameSpaceName, packetName, funcNames[1]);
            }

            serverRegister += PacketFormatForCpp.FunctionRegisterEndFormat;

            string fileName = string.Format(PacketFormatForCpp.FileName, nameSpaceName);

            string outPath = type + "/" + language + "/" + serverName;

            CheckDirectoryAndCreate(outPath);

            File.WriteAllText(outPath + "/" + fileName, serverRegister);
        }
        public static void MakeServerPacketForCS()
        {

        }
        public static void MakeClientPacketForCpp()
        {
            string nameSpaceName = ParseNamespaceName();

            clientRegister += string.Format(PacketFormatForCpp.FunctionBasicFormat, nameSpaceName, "CoreServerSession& session");

            List<string> packetNames = ParsePacketName("Packet_NONE", "Packet_MIN");
            foreach (string packetName in packetNames)
            {
                string[] funcNames = packetName.Trim().Split("Packet_");

                if (packetName.Contains("SC"))
                    clientRegister += string.Format(PacketFormatForCpp.FunctionRegisterFormat, nameSpaceName, packetName, funcNames[1]);
            }

            clientRegister += PacketFormatForCpp.FunctionRegisterEndFormat;

            string fileName = string.Format(PacketFormatForCpp.FileName, nameSpaceName);

            string outPath = type + "/" + language + "/" + serverName;

            CheckDirectoryAndCreate(outPath);

            File.WriteAllText(outPath + "/" + fileName, clientRegister);
        }

        public static void MakeClientPacketForCS()
        {
            string nameSpaceName = ParseNamespaceName();

            List<string> packetNames = ParsePacketName("NONE", "};");
            foreach (string packetName in packetNames)
            {
                if (packetName.Contains("SC"))
                {
                    clientRegister += "\n       ";
                    clientRegister += string.Format(PacketFormatForCS.managerRegisterFormat, packetName, nameSpaceName);
                }
            }

            clientRegister = string.Format(PacketFormatForCS.managerFormat, nameSpaceName, clientRegister);

            string fileName = string.Format(PacketFormatForCS.FileName, nameSpaceName);

            string outPath = type + "/" + language + "/" + serverName;

            CheckDirectoryAndCreate(outPath);

            File.WriteAllText(outPath + "/" + fileName, clientRegister);
        }

        static string ParseNamespaceName()
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

        static List<string> ParsePacketName(string start, string end)
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

                    if (names[0] == end)
                        return packetNames;

                    packetNames.Add(names[0]);
                }

                if (line.Contains(start))
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
