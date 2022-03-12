using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using GamePacket;
using Info;

public class AccountManager
{
    public int WorldID { get; set; }
    public string ID { get; set; }
    public string Password { get; set; }
    public long UID { get; set; }
    public int Token { get; set; }
    public MoneyT Money { get; set; }
}
