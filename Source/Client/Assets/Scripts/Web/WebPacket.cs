using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Define;

public class SignupAccountPacketReq
{
	public string ID;
	public string Password;
}

public class SignupAccountPacketRes
{
	public bool IsSuccess;
}

public class LoginAccountPacketReq
{
	public string ID;
	public string Password;
}

public class WorldListInfo
{
	public byte ID;
	public string Name;
	public int BusyScore;
}

public class LoginAccountPacketRes
{
	public bool IsSuccess;
	public long UID;
	public int Token;
	public List<WorldListInfo> WorldList = new List<WorldListInfo>();
}

public class LoginServerInfoPacketReq
{
	public byte WorldID;
	public ServerType ServerType;
}

public class LoginServerInfoPacketRes : ServerInfo
{
	
}