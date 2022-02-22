using UnityCoreLibrary;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using FlatBuffers;
using Login;

class LoginPacketHandler
{
    public static void SC_LOGIN_RES(PacketSession session, Root packet)
    {
       SC_LOGIN_RES loginRes = packet.PacketAsSC_LOGIN_RES();
       Debug.Log(loginRes.Result);
    }
}


