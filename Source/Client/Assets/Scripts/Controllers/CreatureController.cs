using System.Collections;
using System.Collections.Generic;
using System.Configuration;
using UnityEngine;
using Define;
using UnityCoreLibrary;
using Spine.Unity;
using Info;

public class CreatureController : BaseController
{
	CreatureInfoT _creatureInfo = new CreatureInfoT();

	public virtual CreatureInfoT CreatureInfo
	{
		get { return _creatureInfo; }
		set
		{
			if (_creatureInfo.Equals(value))
				return;

			_creatureInfo = value;
		}
	}

	public override float MoveSpeed
	{
		get { return CreatureInfo.Speed.Value[(int)SpeedType.MOVE_SPEED]; }
		set { CreatureInfo.Speed.Value[(int)SpeedType.MOVE_SPEED] = value; }
	}

	public virtual int HP
	{
		get { return CreatureInfo.Hp; }
		set
		{
			if (0 >= value)
			{
				CreatureInfo.Hp = 0;
				OnDead();
				return;
			}

			CreatureInfo.Hp = value;
		}
	}

	public virtual int MP
	{
		get { return CreatureInfo.Mp; }
		set { CreatureInfo.Mp = value; }
	}

	public virtual byte Level
	{
		get { return CreatureInfo.Level; }
		set
		{
			if(CreatureInfo.Level < value)
            {
				HP = CreatureInfo.Ability.Value[(int)Define.AbilityType.HP];
				MP = CreatureInfo.Ability.Value[(int)Define.AbilityType.MP];
			}

			CreatureInfo.Level = value; 
		}
	}

	public long EXP
	{
		get { return CreatureInfo.Exp; }
		set { CreatureInfo.Exp = value; }
	}

	protected SkillAnimationType _skillAnimationType = SkillAnimationType.NONE;

	protected override void Init()
	{
		base.Init();
	}

	protected override bool UpdateController()
	{
		if (base.UpdateController())
			return true;

		switch (State)
		{
			case ObjectState.HIT:
				UpdateHit();
				return true;
			case ObjectState.SKILL:
				UpdateSkill();
				return true;
			case ObjectState.DEAD:
				UpdateDead();
				return true;
		}

		return false;
	}

	protected virtual void UpdateHit()
	{

	}

	protected virtual void UpdateSkill()
	{

	}

	protected virtual void UpdateDead()
	{

	}

	public virtual void OnHit(int damage, bool isCritical)
	{
		State = ObjectState.HIT;
		HP -= damage;

		DamageText text = CoreManagers.Obj.Add("Text", "DamageText", _damageTextPos.position, 30).GetComponent<DamageText>();
		text.Damage = damage;

		CoreManagers.Obj.Add("Effect", "HitBlood", _bloodPos.position, 30);
	}

	protected virtual void OnDead()
	{
		State = ObjectState.DEAD;
	}

	public virtual bool UseSkill(int SkillID)
	{
		return false;
	}
}