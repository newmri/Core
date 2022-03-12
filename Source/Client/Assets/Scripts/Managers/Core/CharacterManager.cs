using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using GamePacket;
using UnityCoreLibrary;

public class CharacterManager
{
    MyCharacterInfoT _myCharacter;
    //Dictionary<long, CharacterInfoT> _characterList = new Dictionary<long, CharacterInfoT>();

    public void AddMyCharacter(MyCharacterInfoT info)
    {
        _myCharacter = info;
    }

    public void SpawnMyCharacter()
    {
        GameObject gameObject = CoreManagers.Obj.Add("Character", "MyCharacter");
        GearEquipper gear = gameObject.GetComponent<GearEquipper>();
        gear.Job = _myCharacter.Job;
        gear.SetGear(_myCharacter.Gear);
    }

    public MyCharacterInfoT GetMyCharacterInfo()
    {
        return _myCharacter;
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
