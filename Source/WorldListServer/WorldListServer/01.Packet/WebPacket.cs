using System.Collections;

public class SignupAccountPacketReq
{
	public string AccountName { get; set; }
	public string Password { get; set; }
}

public class SignupAccountPacketRes
{
	public bool IsSuccess { get; set; }
}

public class LoginAccountPacketReq
{
	public string AccountName;
	public string Password;
}

public class ServerInfo
{
	public string Name;
	public string Ip;
	public int Port;
	public int BusyScore;
}
