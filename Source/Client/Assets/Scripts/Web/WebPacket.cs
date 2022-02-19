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

public class ServerInfo
{
	public string Name;
	public string Ip;
	public int Port;
	public int BusyScore;
}

public class LoginAccountPacketRes
{
	public bool LoginOk;
	public int AccountId;
	public int Token;
	public List<ServerInfo> ServerList = new List<ServerInfo>();
}
