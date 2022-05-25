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
    Vector2Int _objectDefaultPos = new Vector2Int(0, -100);

    public ObjectInfoWithPosT MyObjectInfo { get; set; }
    public CreatureInfoT MyCreatureInfo { get; set; }
    public MyCharacterInfoT MyCharacterInfo { get; set; }
    public MyPlayerController MyPlayer { get; set; }

    Dictionary<long, PlayerController> _playerList = new Dictionary<long, PlayerController>();
    Dictionary<long, ProjectileController> _projectileList = new Dictionary<long, ProjectileController>();

    public void AddMyPlayer()
    {
        GameObject gameObject = CoreManagers.Obj.Add(MyObjectInfo.ObjectInfo.Oid, "Player", "Player");
        MyPlayer = gameObject.GetOrAddComponent<MyPlayerController>();
        CoreManagers.Coroutine.Add(MyPlayerSetInfoDelay());
    }

    // MyPlayer Init이 끝났을 때 까지 대기
    IEnumerator MyPlayerSetInfoDelay()
    {
        yield return new WaitForEndOfFrame();

        // UI 갱신을 위해
        Managers.Account.Money = Managers.Account.Money;

        MyPlayer.ObjectInfoWithPos = MyObjectInfo;
        MyPlayer.CreatureInfo = MyCreatureInfo;
        MyPlayer.MyCharacterInfo = MyCharacterInfo;
        MyPlayer.SyncPos();
        MyPlayer.AddSkill((int)MyPlayer.MyCharacterInfo.Job);
        Managers.UI.GetSceneUI<UIGameScene>().AddSkill((int)MyPlayer.MyCharacterInfo.Job);

        Debug.Log($"MyPlayer oid: {MyObjectInfo.ObjectInfo.Oid}");
        _isMyPlayerLoaded = true;
    }

    public IEnumerator AddPlayer(ObjectInfoWithPosT objectInfoWithPos, CreatureInfoT creatureInfo, CharacterInfoT characterInfo)
    {
        yield return new WaitUntil(() => _isMyPlayerLoaded == true);

        GameObject gameObject = CoreManagers.Obj.Add(objectInfoWithPos.ObjectInfo.Oid, "Player", "Player", _objectDefaultPos, 100);
        PlayerController playerController = gameObject.GetOrAddComponent<PlayerController>();
        CoreManagers.Coroutine.Add(PlayerSetInfoDelay(playerController, objectInfoWithPos, creatureInfo, characterInfo));
    }

    public IEnumerator AddProjectile(ObjectInfoWithPosT objectInfoWithPos, ProjectileType projectileType, float speed)
    {
        yield return new WaitUntil(() => _isMyPlayerLoaded == true);

        GameObject gameObject = CoreManagers.Obj.Add(objectInfoWithPos.ObjectInfo.Oid, "Projectile", projectileType.ToString(), _objectDefaultPos, 100);
        ProjectileController projectileController = gameObject.GetOrAddComponent<ProjectileController>();
        CoreManagers.Coroutine.Add(ProjectileSetInfoDelay(projectileController, objectInfoWithPos, speed));
    }

    // Init이 끝났을 때 까지 대기
    IEnumerator PlayerSetInfoDelay(PlayerController playerController, ObjectInfoWithPosT objectInfoWithPos, CreatureInfoT creatureInfo, CharacterInfoT characterInfo)
    {
        yield return new WaitForEndOfFrame();

        if (playerController == null)
        {
            AddPlayer(objectInfoWithPos, creatureInfo, characterInfo);
            yield break;
        }

        playerController.ObjectInfoWithPos = objectInfoWithPos;
        playerController.CreatureInfo = creatureInfo;
        playerController.CharacterInfo = characterInfo;
        playerController.SyncPos();
        _playerList.Add(objectInfoWithPos.ObjectInfo.Oid, playerController);
    }

    // Init이 끝났을 때 까지 대기
    IEnumerator ProjectileSetInfoDelay(ProjectileController projectileController, ObjectInfoWithPosT objectInfoWithPos, float speed)
    {
        yield return new WaitForEndOfFrame();

        projectileController.ObjectInfoWithPos = objectInfoWithPos;
        projectileController.MoveSpeed = speed;
        projectileController.SyncPos();
        _projectileList.Add(objectInfoWithPos.ObjectInfo.Oid, projectileController);
    }

    public void Move(ObjectInfoWithPosT objectInfoWithPos)
    {
        BaseController baseController = GetBaseController(objectInfoWithPos.ObjectInfo);
        if (baseController == null)
            return;

        baseController.PosInfo = objectInfoWithPos.PosInfo;
    }

    BaseController GetBaseController(ObjectInfoT objectInfo)
    {
        if (MyPlayer != null && objectInfo.Oid == MyPlayer.ObjectInfoWithPos.ObjectInfo.Oid)
        {
            return MyPlayer;
        }

        BaseController baseController = null;

        switch (objectInfo.ObjectType)
        {
            case ObjectType.PLAYER:
                PlayerController playerController;
                if (_playerList.TryGetValue(objectInfo.Oid, out playerController))
                    baseController = playerController;
                break;
            case ObjectType.PROJECTILE:
                ProjectileController projectileController;
                if (_projectileList.TryGetValue(objectInfo.Oid, out projectileController))
                    baseController = projectileController;
                break;
            default:
                return null;
        }

        return baseController;
    }

    CreatureController GetCreatureController(ObjectInfoT objectInfo)
    {
        if (MyPlayer != null && objectInfo.Oid == MyPlayer.ObjectInfoWithPos.ObjectInfo.Oid)
        {
            return MyPlayer;
        }

        CreatureController creatureController = null;
        switch (objectInfo.ObjectType)
        {
            case ObjectType.PLAYER:
                PlayerController controller;
                if (_playerList.TryGetValue(objectInfo.Oid, out controller))
                    creatureController = controller;
                break;
            default:
                return null;
        }

        return creatureController;
    }

    public void Revive(ObjectInfoWithPosT objectInfoWithPos)
    {
        BaseController baseController = GetBaseController(objectInfoWithPos.ObjectInfo);
        baseController.PosInfo = objectInfoWithPos.PosInfo;
        baseController.SyncPos();
    }

    public void SetState(ObjectInfoT objectInfo, ObjectState state)
    {
        BaseController baseController = GetBaseController(objectInfo);
        baseController.State = state;
    }

    public void UseSkill(ObjectInfoT objectInfo, int skillID)
    {
        CreatureController creatureController = GetCreatureController(objectInfo);
        if (creatureController == null)
            return;

        creatureController.UseSkill(skillID);
    }

    public void OnGetDamage(DamageInfoT damageInfo)
    {
        Debug.Log($"ObjectType:{damageInfo.ObjectInfo.ObjectType} " +
                  $"Oid:{damageInfo.ObjectInfo.Oid} " +
                  $"Damaged:{damageInfo.Damage}");

        CreatureController creatureController = GetCreatureController(damageInfo.ObjectInfo);
        creatureController.OnHit(damageInfo.Damage, damageInfo.IsCritical);
    }

    public void Remove(ObjectInfoT objectInfo)
    {
        switch (objectInfo.ObjectType)
        {
            case ObjectType.PLAYER:
                _playerList.Remove(objectInfo.Oid);
                break;
            case ObjectType.PROJECTILE:
                _projectileList.Remove(objectInfo.Oid);
                break;
            default:
                return;
        }

        CoreManagers.Obj.RemoveServerObject(objectInfo.Oid);
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
