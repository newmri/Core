using UnityCoreLibrary;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using FlatBuffers;
using Login;

class PacketHandler
{
    public static void SC_LOGIN_RES(PacketSession session, IFlatbufferObject packet)
    {
        SC_LOGIN_RES loginRes = (SC_LOGIN_RES)packet;
    }
}


