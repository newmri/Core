using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using GamePacket;
using UnityCoreLibrary;
using Info;
using Define;
using System;

public class CreatureManager
{
    bool _isMyPlayerLoaded = false;
    static int _defaultSortOrder = 0;
    int _sortOrder = _defaultSortOrder;

    public CreatureInfoT MyCreatureInfo { get; set; }
    public MyCharacterInfoT MyCharacterInfo { get; set; }
    public MyPlayerController MyPlayer { get; set; }

    Dictionary<long, PlayerController> _playerList = new Dictionary<long, PlayerController>();

    public void AddMyPlayer()
    {
        GameObject gameObject = CoreManagers.Obj.Add(MyCreatureInfo.Oid, "Player", "Player");
        MyPlayer = gameObject.GetOrAddComponent<MyPlayerController>();
        CoreManagers.Coroutine.Add(MyPlayerSetInfoDelay());
    }

    // MyPlayer Init이 끝났을 때 까지 대기
    IEnumerator MyPlayerSetInfoDelay()
    {
        yield return new WaitForEndOfFrame();

        // UI 갱신을 위해
        Managers.Account.Money = Managers.Account.Money;

        MyPlayer.CreatureInfo = MyCreatureInfo;
        MyPlayer.MyCharacterInfo = MyCharacterInfo;
        MyPlayer.SyncPos();
        MyPlayer.AddSkill((int)MyPlayer.MyCharacterInfo.Job);
        Managers.UI.GetSceneUI<UIGameScene>().AddSkill((int)MyPlayer.MyCharacterInfo.Job);

        _isMyPlayerLoaded = true;
    }

    public void AddPlayer(CreatureInfoT creatureInfo, CharacterInfoT characterInfo)
    {
        GameObject gameObject = CoreManagers.Obj.Add(creatureInfo.Oid, "Player", "Player");
        PlayerController playerController = gameObject.GetOrAddComponent<PlayerController>();
        CoreManagers.Coroutine.Add(PlayerSetInfoDelay(playerController, creatureInfo, characterInfo));
    }

    // Player Init이 끝났을 때 까지 대기
    IEnumerator PlayerSetInfoDelay(PlayerController playerController, CreatureInfoT creatureInfo, CharacterInfoT characterInfo)
    {
        if (_isMyPlayerLoaded)
            yield return new WaitForEndOfFrame();
        else
            yield return new WaitUntil(() => _isMyPlayerLoaded == true);

        playerController.CreatureInfo = creatureInfo;
        playerController.CharacterInfo = characterInfo;
        playerController.SyncPos();
        _playerList.Add(creatureInfo.Oid, playerController);
    }

    public void Move(ObjectType objectType, long oid, PositionInfoT pos)
    {
        BaseController baseController = GetBaseController(objectType, oid);
        baseController.PosInfo = pos;
    }

    BaseController GetBaseController(ObjectType objectType, long oid)
    {
        if (oid == MyPlayer.CreatureInfo.Oid)
        {
            return MyPlayer;
        }

        BaseController baseController = null;

        switch (objectType)
        {
            case ObjectType.PLAYER:
                PlayerController controller;
                if (_playerList.TryGetValue(oid, out controller))
                    baseController = controller;
                break;
            default:
                return null;
        }

        return baseController;
    }

    CreatureController GetCreatureController(ObjectType objectType, long oid)
    {
        if (oid == MyPlayer.CreatureInfo.Oid)
        {
            return MyPlayer;
        }

        CreatureController creatureController = null;
        switch (objectType)
        {
            case ObjectType.PLAYER:
                PlayerController controller;
                if (_playerList.TryGetValue(oid, out controller))
                    creatureController = controller;
                break;
            default:
                return null;
        }

        return creatureController;
    }

    public void Revive(ObjectType objectType, long oid, PositionInfoT pos)
    {
        BaseController baseController = GetBaseController(objectType, oid);
        baseController.State = CreatureState.IDLE;
        baseController.PosInfo = pos;
        baseController.SyncPos();
    }

    public void SetState(ObjectType objectType, long oid, CreatureState state)
    {
        BaseController baseController = GetBaseController(objectType, oid);
        baseController.State = state;
    }

    public void UseSkill(ObjectType objectType, long oid, int skillID)
    {
        CreatureController creatureController = GetCreatureController(objectType, oid);
        creatureController.UseSkill(skillID);
    }

    public void OnGetDamage(DamageInfo damageInfo)
    {
        Debug.Log($"ObjectType:{damageInfo.ObjectType} " +
                  $"Oid:{damageInfo.Oid} " +
                  $"Damaged:{damageInfo.Damage}");

        CreatureController creatureController = GetCreatureController(damageInfo.ObjectType, damageInfo.Oid);
        creatureController.OnHit(damageInfo.Damage, damageInfo.IsCritical);
    }

    public void Remove(ObjectType objectType, long oid)
    {
        switch (objectType)
        {
            case ObjectType.PLAYER:
                _playerList.Remove(oid);
                break;
            default:
                return;
        }

        CoreManagers.Obj.RemoveServerObject(oid);
        SetSortOrder(_sortOrder - 1);
    }

    public int GetSortOrder(BaseController baseController)
    {
        if (baseController == MyPlayer)
            return 9999;

        return _sortOrder++;
    }

    void SetSortOrder(int sortOrder)
    {
        _sortOrder = sortOrder;
    }

    public void Clear()
    {
        _isMyPlayerLoaded = false;
        SetSortOrder(_defaultSortOrder);
        MyCreatureInfo = null;
        MyCharacterInfo = null;
        MyPlayer = null;
        _playerList.Clear();
    }
}
