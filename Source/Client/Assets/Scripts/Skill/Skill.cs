using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Skill
{
    public int SkillID = 0;
    public float ElapsedCoolTime = 0.0f;
    public float CoolTime = 0.0f;
    public bool Used = false;
    public Skill(int skillID)
    {
        SkillID = skillID;
        CoolTime = (float)((int)Managers.CharacterData.GetSkill(skillID, "CoolTime") / 1000.0f);
    }

    public void Init()
    {
        Used = false;
        ElapsedCoolTime = 0.0f;
    }

    public bool CanUseSkill(int HP, int MP)
    {
        return (HP >= (int)Managers.CharacterData.GetSkill(SkillID, "HP") &&
                 MP >= (int)Managers.CharacterData.GetSkill(SkillID, "MP"));
    }
}
