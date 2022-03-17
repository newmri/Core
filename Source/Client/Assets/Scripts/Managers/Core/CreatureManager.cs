using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using GamePacket;
using UnityCoreLibrary;
using Info;

public class CreatureManager
{
    public CreatureInfoT MyCreatureInfo { get; set; }
    public MyCharacterInfoT MyCharacterInfo { get; set; }
    public MyPlayerController MyPlayer { get; set; }

    Dictionary<long, GameObject> _objects = new Dictionary<long, GameObject>();

    public void AddMyPlayer()
    {
        GameObject gameObject = CoreManagers.Obj.Add("Character", "MyCharacter");
        MyPlayer = gameObject.GetComponent<MyPlayerController>();
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
    }


    //public void Add(CharacterInfoT info)
    //{
    //    _characterList.Add(info.Uid, info);
    //}

    //public void Remove(long uid)
    //{
    //    _characterList.Remove(uid);
    //}
}
