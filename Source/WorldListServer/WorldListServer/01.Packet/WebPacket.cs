using System.Collections;
using System.Collections.Generic;

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
	public string Name { get; set; }
	public int BusyScore { get; set; }
}

public class LoginAccountPacketRes
{
	public bool IsSuccess { get; set; }
	public long UID { get; set; }
	public long Token { get; set; }
	public List<WorldListInfo> WorldList { get; set; }
}