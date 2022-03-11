using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using GamePacket;

public class AccountManager
{
    public int WorldID { get; set; }
    public string ID { get; set; }
    public string Password { get; set; }
    public long UID { get; set; }
    public int Token { get; set; }
    public int[] Money {get; set;}
}
