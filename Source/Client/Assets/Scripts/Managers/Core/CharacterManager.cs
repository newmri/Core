using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using GamePacket;
using UnityCoreLibrary;

public class CharacterManager
{
    CHARACTER_INFOT _myCharacter;
    Dictionary<long, CHARACTER_INFOT> _characterList = new Dictionary<long, CHARACTER_INFOT>();

    public void AddMyCharacter(CHARACTER_INFOT info)
    {
        _myCharacter = info;
    }

    public void SpawnMyCharacter()
    {
        GameObject gameObject = CoreManagers.Obj.Add("Character", "MyCharacter");
        GearEquipper gear = gameObject.GetComponent<GearEquipper>();
        gear.Job = _myCharacter.Job;
        gear.SetGear(_myCharacter.Gear.ToArray());
    }

    public CHARACTER_INFOT GetMyCharacterInfo()
    {
        return _myCharacter;
    }

    public void Add(CHARACTER_INFOT info)
    {
        _characterList.Add(info.Uid, info);
    }

    public void Remove(long uid)
    {
        _characterList.Remove(uid);
    }
}
