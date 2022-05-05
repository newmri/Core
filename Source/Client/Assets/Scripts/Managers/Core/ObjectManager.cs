using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using GamePacket;
using UnityCoreLibrary;
using Info;
using Define;
using System;

public class ObjectManager
{
    bool _isMyPlayerLoaded = false;
    static int _defaultSortOrder = 0;
    int _sortOrder = _defaultSortOrder;

    public ObjectInfoT MyObjectInfo { get; set; }
    public CreatureInfoT MyCreatureInfo { get; set; }
    public MyCharacterInfoT MyCharacterInfo { get; set; }
    public MyPlayerController MyPlayer { get; set; }

    Dictionary<long, PlayerController> _playerList = new Dictionary<long, PlayerController>();
    Dictionary<long, ProjectileController> _projectileList = new Dictionary<long, ProjectileController>();

    public void AddMyPlayer()
    {
        GameObject gameObject = CoreManagers.Obj.Add(MyObjectInfo.Oid, "Player", "Player");
        MyPlayer = gameObject.GetOrAddComponent<MyPlayerController>();
        CoreManagers.Coroutine.Add(MyPlayerSetInfoDelay());
    }

    // MyPlayer Init이 끝났을 때 까지 대기
    IEnumerator MyPlayerSetInfoDelay()
    {
        yield return new WaitForEndOfFrame();

        // UI 갱신을 위해
        Managers.Account.Money = Managers.Account.Money;

        MyPlayer.ObjectInfo = MyObjectInfo;
        MyPlayer.CreatureInfo = MyCreatureInfo;
        MyPlayer.MyCharacterInfo = MyCharacterInfo;
        MyPlayer.SyncPos();
        MyPlayer.AddSkill((int)MyPlayer.MyCharacterInfo.Job);
        Managers.UI.GetSceneUI<UIGameScene>().AddSkill((int)MyPlayer.MyCharacterInfo.Job);

        _isMyPlayerLoaded = true;
    }

    public void AddPlayer(ObjectInfoT objectInfo, CreatureInfoT creatureInfo, CharacterInfoT characterInfo)
    {
        GameObject gameObject = CoreManagers.Obj.Add(objectInfo.Oid, "Player", "Player");
        PlayerController playerController = gameObject.GetOrAddComponent<PlayerController>();
        CoreManagers.Coroutine.Add(PlayerSetInfoDelay(playerController, objectInfo, creatureInfo, characterInfo));
    }

    public void AddProjectile(ObjectInfoT objectInfo, ProjectileType projectileType, float speed)
    {
        GameObject gameObject = CoreManagers.Obj.Add(objectInfo.Oid, "Projectile", projectileType.ToString(), null, 10);
        ProjectileController projectileController = gameObject.GetOrAddComponent<ProjectileController>();
        CoreManagers.Coroutine.Add(ProjectileSetInfoDelay(projectileController, objectInfo, speed));
    }

    // Player Init이 끝났을 때 까지 대기
    IEnumerator PlayerSetInfoDelay(PlayerController playerController, ObjectInfoT objectInfo, CreatureInfoT creatureInfo, CharacterInfoT characterInfo)
    {
        if (_isMyPlayerLoaded)
            yield return new WaitForEndOfFrame();
        else
            yield return new WaitUntil(() => _isMyPlayerLoaded == true);

        if (playerController == null)
        {
            AddPlayer(objectInfo, creatureInfo, characterInfo);
            yield break;
        }

        Debug.Log("AddPlayer");
        Debug.Log("oid: " + objectInfo.Oid);
        Debug.Log($"X: {objectInfo.PosInfo.Pos.X} Y: {objectInfo.PosInfo.Pos.Y}");

        playerController.ObjectInfo = objectInfo;
        playerController.CreatureInfo = creatureInfo;
        playerController.CharacterInfo = characterInfo;
        playerController.SyncPos();
        _playerList.Add(objectInfo.Oid, playerController);
    }

    // Player Init이 끝났을 때 까지 대기
    IEnumerator ProjectileSetInfoDelay(ProjectileController projectileController, ObjectInfoT objectInfo, float speed)
    {
        if (_isMyPlayerLoaded)
            yield return new WaitForEndOfFrame();
        else
            yield return new WaitUntil(() => _isMyPlayerLoaded == true);

        projectileController.ObjectInfo = objectInfo;
        projectileController.MoveSpeed = speed;
        projectileController.SyncPos();
        _projectileList.Add(objectInfo.Oid, projectileController);
    }

    public void Move(ObjectInfoT objectInfo)
    {
        BaseController baseController = GetBaseController(objectInfo.ObjectType, objectInfo.Oid);
        baseController.PosInfo = objectInfo.PosInfo;
    }

    BaseController GetBaseController(ObjectType objectType, long oid)
    {
        if (oid == MyPlayer.ObjectInfo.Oid)
        {
            return MyPlayer;
        }

        BaseController baseController = null;

        switch (objectType)
        {
            case ObjectType.PLAYER:
                PlayerController playerController;
                if (_playerList.TryGetValue(oid, out playerController))
                    baseController = playerController;
                break;
            case ObjectType.PROJECTILE:
                ProjectileController projectileController;
                if (_projectileList.TryGetValue(oid, out projectileController))
                    baseController = projectileController;
                break;
            default:
                return null;
        }

        return baseController;
    }

    CreatureController GetCreatureController(ObjectType objectType, long oid)
    {
        if (oid == MyPlayer.ObjectInfo.Oid)
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

    public void Revive(ObjectInfoT objectInfo)
    {
        BaseController baseController = GetBaseController(objectInfo.ObjectType, objectInfo.Oid);
        baseController.PosInfo = objectInfo.PosInfo;
        baseController.SyncPos();
    }

    public void SetState(ObjectType objectType, long oid, ObjectState state)
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
            case ObjectType.PROJECTILE:
                _projectileList.Remove(oid);
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
        MyObjectInfo = null;
        MyCreatureInfo = null;
        MyCharacterInfo = null;
        MyPlayer = null;
        _playerList.Clear();
    }
}
