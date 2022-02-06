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
            string file = "../../../FlatBuffer/output/login_protocol_generated.h";
            if (args.Length >= 1)
                file = args[0];

            string nameSpaceName = ParseNamespaceName(file);

            serverRegister += string.Format(PacketFormat.FunctionBasicFormatForCpp, nameSpaceName);
            clientRegister += string.Format(PacketFormat.FunctionBasicFormatForCpp, nameSpaceName);

            List<string> packetNames = ParsePacketName(file);
            foreach (string packetName in packetNames)
            {
                string[] funcNames = packetName.Trim().Split("Packet_");
 
                if(packetName.Contains("CS"))
                    serverRegister += string.Format(PacketFormat.FunctionRegisterFormatForCpp, nameSpaceName, packetName, funcNames[1]);
                else
                    clientRegister += string.Format(PacketFormat.FunctionRegisterFormatForCpp, nameSpaceName, packetName, funcNames[1]);
            }

            serverRegister += PacketFormat.FunctionRegisterEndFormatForCpp;
            clientRegister += PacketFormat.FunctionRegisterEndFormatForCpp;

            string fileName = string.Format(PacketFormat.FileNameForCpp, nameSpaceName);

            CheckDirectoryAndCreate("Server");
            CheckDirectoryAndCreate("Client");

            File.WriteAllText("Server/" + fileName, serverRegister);
            File.WriteAllText("Client/" + fileName, clientRegister);
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
