using System.Collections;
using System.Collections.Generic;
using UnityEngine;
public enum SkillAnimationType : byte
{
    NONE,
    ATTACK_1,
    ATTACK_2
};

public class Skill
{
    public int SkillID = 0;
    public float ElapsedCoolTime = 0.0f;
    public float CoolTime = 0.0f;
    public bool Used = false;
    public Skill(int skillID)
    {
        SkillID = skillID;
        CoolTime = (float)Managers.CharacterData.GetSkill(skillID, "CoolTime");
    }

    public void Init()
    {
        Used = false;
        ElapsedCoolTime = 0.0f;
    }
}
