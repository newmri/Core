using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using GamePacket;
using UnityCoreLibrary;
using Info;
using Define;

public class CreatureManager
{
    bool _isMyPlayerLoaded = false;

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
        Managers.UI.DecreaseSortingOrder();
    }
}
