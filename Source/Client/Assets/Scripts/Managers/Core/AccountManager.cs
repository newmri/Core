using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using GamePacket;
using Info;

public class AccountManager
{
    public byte WorldID { get; set; }
    public string ID { get; set; }
    public string Password { get; set; }
    public long UID { get; set; }
    public int Token { get; set; }

    public UIGameScene UIGameScene{ private get; set; }

    MoneyT _money;
    public MoneyT Money
    {
        get { return _money; }
        set
        {
            _money = value;
            if(UIGameScene != null)
                UIGameScene.UpdateMoney(_money);
        }
    }
}
