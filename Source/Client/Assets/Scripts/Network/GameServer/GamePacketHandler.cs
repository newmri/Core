﻿using UnityCoreLibrary;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using FlatBuffers;
using GamePacket;
using Define;

class GamePacketHandler
{
    public static void SC_LOGIN_RES(PacketSession session, Root packet)
    {
        Debug.Log("Recv SC_LOGIN_RES");

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

        Managers.Object.MyObjectInfo = loginRes.ObjectInfoWithPos.Value.UnPack();
        Managers.Object.MyCreatureInfo = loginRes.CreatureInfo.Value.UnPack();
        Managers.Object.MyCharacterInfo = loginRes.CharacterInfo.Value.UnPack();
        Managers.Account.Money = loginRes.Money.Value.UnPack().Value;

        CoreManagers.Scene.LoadScene(CoreDefine.Scene.Game);
    }

    public static void SC_ITEM_INVENTORY_INFO_NOTI(PacketSession session, Root packet)
    {
        Debug.Log("Recv SC_ITEM_INVENTORY_INFO_NOTI");

        SC_ITEM_INVENTORY_INFO_NOTI itemInventoryInfoNoti = packet.PacketAsSC_ITEM_INVENTORY_INFO_NOTI();
        List<Info.ItemSlotInfoT> itemSlotInfoList = itemInventoryInfoNoti.UnPack().ItemSlotInfo;
        CoreManagers.Coroutine.Add(UpdateItemInventoryDelay(itemInventoryInfoNoti.MaxSlotCount, itemSlotInfoList));
    }

    public static IEnumerator UpdateItemInventoryDelay(byte maxSlotCount, List<Info.ItemSlotInfoT> itemSlotInfoList)
    {
        yield return new WaitUntil(() => Managers.Object.IsMyPlayerLoaded == true);
        Managers.UI.GetSceneUI<UIGameScene>().Inventory.UpdateItemSlot(maxSlotCount, itemSlotInfoList);
    }

    public static void SC_PING_REQ(PacketSession session, Root packet)
    {
        Debug.Log("PingCheck");
        Managers.GameNetwork.Send(Packet.CS_PING_RES);
    }

    public static void SC_SPAWN_PLAYER_NOTI(PacketSession session, Root packet)
    {
        SC_SPAWN_PLAYER_NOTI spawnNoti = packet.PacketAsSC_SPAWN_PLAYER_NOTI();
        CoreManagers.Coroutine.Add(
            Managers.Object.AddPlayer(spawnNoti.UnPack().ObjectInfoWithPos,
            spawnNoti.UnPack().CreatureInfo,
            spawnNoti.UnPack().CharacterInfo));

    }

    public static void SC_DESPAWN_OBJECT_NOTI(PacketSession session, Root packet)
    {
        SC_DESPAWN_OBJECT_NOTI despawnNoti = packet.PacketAsSC_DESPAWN_OBJECT_NOTI();
        Managers.Object.Remove(despawnNoti.UnPack().ObjectInfo);
    }

    public static void SC_MOVE_RES(PacketSession session, Root packet)
    {
        SC_MOVE_RES moveRes = packet.PacketAsSC_MOVE_RES();
        Managers.Object.Move(moveRes.UnPack().ObjectInfoWithPos);
    }
    public static void SC_SET_STATE_RES(PacketSession session, Root packet)
    {
        SC_SET_STATE_RES setStateRes = packet.PacketAsSC_SET_STATE_RES();
        Managers.Object.SetState(setStateRes.UnPack().ObjectInfo, setStateRes.UnPack().State);
    }

    public static void SC_USE_SKILL_RES(PacketSession session, Root packet)
    {
        SC_USE_SKILL_RES useSkillRes = packet.PacketAsSC_USE_SKILL_RES();
        Managers.Object.UseSkill(useSkillRes.UnPack().ObjectInfo, useSkillRes.SkillId);
    }

    public static void SC_GET_DAMAGE_NOTI(PacketSession session, Root packet)
    {
        SC_GET_DAMAGE_NOTI getDamageNoti = packet.PacketAsSC_GET_DAMAGE_NOTI();
        for (int i = 0; i < getDamageNoti.DamageInfoLength; ++i)
        {
            Managers.Object.OnGetDamage(getDamageNoti.DamageInfo(i).Value.UnPack());
        }
    }

    public static void SC_REVIVE_RES(PacketSession session, Root packet)
    {
        SC_REVIVE_RES reviveRes = packet.PacketAsSC_REVIVE_RES();
        Managers.Object.Revive(reviveRes.UnPack().ObjectInfoWithPos);
    }

    public static void SC_SPAWN_PROJECTILE_NOTI(PacketSession session, Root packet)
    {
        SC_SPAWN_PROJECTILE_NOTI spawnNoti = packet.PacketAsSC_SPAWN_PROJECTILE_NOTI();
        CoreManagers.Coroutine.Add(
            Managers.Object.AddProjectile(spawnNoti.UnPack().ObjectInfoWithPos,
            spawnNoti.UnPack().ProjectileType,
            spawnNoti.UnPack().Speed));
    }

    public static void SC_EQUIP_GEAR_RES(PacketSession session, Root packet)
    {
        SC_EQUIP_GEAR_RES equipGearRes = packet.PacketAsSC_EQUIP_GEAR_RES();
        if (ErrorCode.SUCCESS != equipGearRes.Result)
        {
            UIMessagePopup messagePopup = Managers.UI.ShowPopupUI<UIMessagePopup>();

            switch (equipGearRes.Result)
            {
                case ErrorCode.UNKNOWN:
                    messagePopup.SetText("장착 실패", "다시 시도해주세요");
                    break;
            }

            return;
        }

        Managers.UI.GetSceneUI<UIGameScene>().Inventory.EquipGear(equipGearRes.ItemUid);
    }

    public static void SC_UNEQUIP_GEAR_RES(PacketSession session, Root packet)
    {
        SC_UNEQUIP_GEAR_RES unEquipGearRes = packet.PacketAsSC_UNEQUIP_GEAR_RES();
        if (ErrorCode.SUCCESS != unEquipGearRes.Result)
        {
            UIMessagePopup messagePopup = Managers.UI.ShowPopupUI<UIMessagePopup>();

            switch (unEquipGearRes.Result)
            {
                case ErrorCode.UNKNOWN:
                    messagePopup.SetText("장착 해제 실패", "다시 시도해주세요");
                    break;
            }

            return;
        }

        Managers.UI.GetSceneUI<UIGameScene>().Inventory.UnEquipGear(unEquipGearRes.GearType);
    }

    public static void SC_ABILITY_INFO_NOTI(PacketSession session, Root packet)
    {
        SC_ABILITY_INFO_NOTI abilityInfo = packet.PacketAsSC_ABILITY_INFO_NOTI();

        Managers.Object.MyCreatureInfo.Ability = abilityInfo.UnPack().Ability;

        UIItemInventoryPopup inventory = Managers.UI.GetSceneUI<UIGameScene>().Inventory;
        if (inventory.gameObject.activeSelf)
            inventory.UpdateAbility();
    }

    public static void SC_NORMAL_CHAT_RES(PacketSession session, Root packet)
    {
        SC_NORMAL_CHAT_RES chatRes = packet.PacketAsSC_NORMAL_CHAT_RES();
        UIGameScene ui = Managers.UI.GetSceneUI<UIGameScene>();
        if (ui)
        {
            ui.Chat.AddMessage(chatRes.Oid, chatRes.ChatType, chatRes.Message);
        }
    }

    public static void SC_ADDED_ITEM_TO_ITEM_INVENTORY_NOTI(PacketSession session, Root packet)
    {
        SC_ADDED_ITEM_TO_ITEM_INVENTORY_NOTI itemInfo = packet.PacketAsSC_ADDED_ITEM_TO_ITEM_INVENTORY_NOTI();

        UIItemInventoryPopup inventory = Managers.UI.GetSceneUI<UIGameScene>().Inventory;
        var itemInfoList = itemInfo.UnPack().ItemSlotInfo;
        foreach (var info in itemInfoList)
            inventory.AddItem(info);
    }

    public static void SC_ADD_STAT_RES(PacketSession session, Root packet)
    {
        SC_ADD_STAT_RES addStatRes = packet.PacketAsSC_ADD_STAT_RES();

        if (ErrorCode.SUCCESS != addStatRes.Result)
        {
            UIMessagePopup messagePopup = Managers.UI.ShowPopupUI<UIMessagePopup>();

            switch (addStatRes.Result)
            {
                case ErrorCode.UNKNOWN:
                    messagePopup.SetText("스탯 투자 실패", "다시 시도해주세요");
                    break;
            }

            return;
        }

        Managers.Object.MyCreatureInfo.Stat.Value[(int)addStatRes.StatType] += 1;
        Managers.Object.MyCharacterInfo.BonusStat -= 1;

        UIItemInventoryPopup inventory = Managers.UI.GetSceneUI<UIGameScene>().Inventory;
        if (inventory.gameObject.activeSelf)
        {
            inventory.UpdateStatPoint(addStatRes.StatType);
            inventory.UpdateBonusStat();
        }
    }

    public static void SC_MY_LEVEL_UP_NOTI(PacketSession session, Root packet)
    {
        SC_MY_LEVEL_UP_NOTI myLevelUpNoti = packet.PacketAsSC_MY_LEVEL_UP_NOTI();
        Managers.Object.MyPlayer.Level = myLevelUpNoti.NewLevel;
        Managers.Object.MyCharacterInfo.BonusStat = myLevelUpNoti.NewStatPoint;

        CoreManagers.Obj.Add("Effect", "LevelUp", Managers.Object.MyPlayer.EffectPos);
    }

    public static void SC_LEVEL_UP_NOTI(PacketSession session, Root packet)
    {
        SC_LEVEL_UP_NOTI levelUpNoti = packet.PacketAsSC_LEVEL_UP_NOTI();

        Managers.Object.AddEffect(levelUpNoti.Oid, "LevelUp");
    }

    public static void SC_USE_ITEM_RES(PacketSession session, Root packet)
    {
        SC_USE_ITEM_RES itemUseRes = packet.PacketAsSC_USE_ITEM_RES();
        if (ErrorCode.SUCCESS != itemUseRes.Result)
        {
            UIMessagePopup messagePopup = Managers.UI.ShowPopupUI<UIMessagePopup>();

            switch (itemUseRes.Result)
            {
                case ErrorCode.UNKNOWN:
                    messagePopup.SetText("아이템 사용 실패", "다시 시도해주세요");
                    break;
            }

            return;
        }

        Managers.UI.GetSceneUI<UIGameScene>().Inventory.UpdateItemCount(itemUseRes.ItemUid, itemUseRes.ItemCount);
    }

    public static void SC_ADDED_EFFECT_NOTI(PacketSession session, Root packet)
    {
        SC_ADDED_EFFECT_NOTI addedEffectNoti = packet.PacketAsSC_ADDED_EFFECT_NOTI();
        for (int i = 0; i < addedEffectNoti.EffectIdLength; ++i)
        {
            EffectData effect = new EffectData();
            Managers.EffectData.GetEffect(addedEffectNoti.EffectId(i), ref effect);
            switch (effect.EffectType)
            {
                case EffectType.HP:
                    Managers.Object.MyPlayer.HP += effect.EffectValue;
                    break;
                default:
                    break;
            }
        }
    }
}


