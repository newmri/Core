﻿using System.Collections;
using System.Collections.Generic;

public enum WebLoginResultCode : byte
{
    SUCCESS = 0,
    LOGINED,
    WRONG,
}

public enum ServerType : byte
{
    WorldList = 0,
    Login = 1,
    Game = 2,
    ServerTypeEnd = 3,
};

public class SignupAccountPacketReq
{
	public string ID { get; set; }
	public string Password { get; set; }
}

public class SignupAccountPacketRes
{
	public bool IsSuccess { get; set; }
}

public class LoginAccountPacketReq
{
	public string ID { get; set; }
	public string Password { get; set; }
}

public class WorldListInfo
{
	public byte ID { get; set; }
	public string Name { get; set; }
	public int BusyScore { get; set; }
}

public class LoginAccountPacketRes
{
	public WebLoginResultCode Code { get; set; }
	public long UID { get; set; }
	public int Token { get; set; }
	public List<WorldListInfo> WorldList { get; set; }
}

public class LoginServerInfoPacketReq
{
	public byte WorldID { get; set; }
	public ServerType ServerType { get; set; }
}

public class LoginServerInfoPacketRes
{
	public string ServerIP { get; set; }
	public int ServerPort { get; set; }
}