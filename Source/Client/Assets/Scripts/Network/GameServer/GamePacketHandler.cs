using UnityCoreLibrary;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using FlatBuffers;
using GamePacket;

class GamePacketHandler
{
    public static void SC_LOGIN_RES(PacketSession session, Root packet)
    {
        SC_LOGIN_RES loginRes = packet.PacketAsSC_LOGIN_RES();
        if (ErrorCode.SUCCESS != loginRes.Result)
        {
            Managers.UI.CloseAllPopupUI();

            UIMessagePopup messagePopup = Managers.UI.ShowPopupUI<UIMessagePopup>();

            switch (loginRes.Result)
            {
                case ErrorCode.UNKNOWN:
                    messagePopup.SetText("서버 접속 실패", "다시 로그인을 해주세요");
                    break;
                case ErrorCode.ALREADY_LOGINED:
                    messagePopup.SetText("서버 접속 실패", "이미 접속 중 입니다");
                    break;
            }

            session.Disconnect();
            return;
        }

        Managers.Creature.MyCreatureInfo = loginRes.CreatureInfo.Value.UnPack();
        Managers.Creature.MyCharacterInfo = loginRes.CharacterInfo.Value.UnPack();
        Managers.Account.Money = loginRes.Money.Value.UnPack().Value;

        CoreManagers.Scene.LoadScene(CoreDefine.Scene.Game);
    }

    public static void SC_PING_REQ(PacketSession session, Root packet)
    {
        Debug.Log("PingCheck");
        Managers.GameNetwork.Send(Packet.CS_PING_RES);
    }

    public static void SC_SPAWN_PLAYER_NOTI(PacketSession session, Root packet)
    {
        SC_SPAWN_PLAYER_NOTI spawnNoti = packet.PacketAsSC_SPAWN_PLAYER_NOTI();
        Managers.Creature.AddPlayer(spawnNoti.UnPack().CreatureInfo, spawnNoti.UnPack().CharacterInfo);
    }

    public static void SC_DESPAWN_OBJECT_NOTI(PacketSession session, Root packet)
    {
        SC_DESPAWN_OBJECT_NOTI despawnNoti = packet.PacketAsSC_DESPAWN_OBJECT_NOTI();
        Managers.Creature.Remove(despawnNoti.ObjectType, despawnNoti.Uid);
    }

    public static void SC_MOVE_RES(PacketSession session, Root packet)
    {
        SC_MOVE_RES moveRes = packet.PacketAsSC_MOVE_RES();
        Managers.Creature.Move(moveRes.ObjectType, moveRes.ObjectId, moveRes.UnPack().PosInfo);
    }
    public static void SC_SET_STATE_RES(PacketSession session, Root packet)
    {
        SC_SET_STATE_RES setStateRes = packet.PacketAsSC_SET_STATE_RES();
        Managers.Creature.SetState(setStateRes.ObjectType, setStateRes.ObjectId, setStateRes.UnPack().State);
    }

    public static void SC_USE_SKILL_RES(PacketSession session, Root packet)
    {
        SC_USE_SKILL_RES useSkillRes = packet.PacketAsSC_USE_SKILL_RES();
        Managers.Creature.UseSkill(useSkillRes.ObjectType, useSkillRes.ObjectId, useSkillRes.SkillId);
    }

    public static void SC_GET_DAMAGE_NOTI(PacketSession session, Root packet)
    {
        SC_GET_DAMAGE_NOTI getDamageNoti = packet.PacketAsSC_GET_DAMAGE_NOTI();
        for (int i = 0; i < getDamageNoti.DamageInfoLength; ++i)
        {
            Managers.Creature.OnGetDamage(getDamageNoti.DamageInfo(i).Value);
        }
    }
}


