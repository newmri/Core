using System.Collections;
using System.Collections.Generic;
using UnityEngine;

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
	public int ID;
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
	public int WorldID;
}

public class LoginServerInfoPacketRes : ServerInfo
{
	
}