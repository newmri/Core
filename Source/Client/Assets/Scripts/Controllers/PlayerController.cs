using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Define;
using Spine;
using Spine.Unity;
using UnityCoreLibrary;
using GamePacket;
using FlatBuffers;
using TMPro;

public class PlayerController : CreatureController
{
	protected CharacterAnimator _animator;
	protected TextMeshPro _name;

	[SerializeField]
	protected CharacterInfoBaseUnion _characterInfo = new CharacterInfoBaseUnion();

	protected GearEquipper _gear;

	public virtual Job Job
	{
		get
        {
			return CharacterInfo.Job;
		}
		set
        {
			CharacterInfo.Job = value;
			_gear.Job = value;
		}
	}

	public virtual Info.CharacterGearT Gear
    {
		get
        {
			return CharacterInfo.Gear;
		}
		set
        {
			_gear.SetGear(value);
		}
    }

	public CharacterInfoT CharacterInfo
	{
		get
		{
			return _characterInfo.AsCharacterInfo();
		}
		set
		{
			if (_characterInfo.Value != null && _characterInfo.Value.Equals(value))
				return;

			_characterInfo.Value = value;

			Job = CharacterInfo.Job;
			Gear = CharacterInfo.Gear;

			_name.text = value.Name;
		}
	}

	public override ObjectState State
	{
		get { return PosInfo.State; }
		set
		{
			if (PosInfo.State == value)
				return;

			PosInfo.State = value;

			if (value == ObjectState.RUN)
				_isRunning = true;
			else
				_isRunning = false;

			UpdateAnimation();
			_updated = true;
		}
	}

	protected override void Init()
	{
		base.Init();
		_animator = GetComponent<CharacterAnimator>();
		_animator.JobChanged(Job.WARRIOR);
		_animator.AddAnimationEndEvent(AnimationEndEvent);

		_gear = GetComponent<GearEquipper>();

		MeshRenderer mesh = GetComponentInChildren<MeshRenderer>();
		mesh.sortingOrder = Managers.Object.GetSortOrder(this);

		_name = transform.Find("NameText").GetComponent<TextMeshPro>();
	}

	protected override void UpdateAnimation()
	{
		if (_animator == null)
			return;

		_animator.ChangeAnimation(State, Dir, _skillAnimationType);
		_name.transform.localScale = _animator.transform.localScale;
	}

	protected override bool UpdateController()
	{
		return base.UpdateController();
	}

	public override bool UseSkill(int skillID)
	{
		if (IsDead())
			return false;

		_skillAnimationType = (SkillAnimationType)(int)Managers.CharacterData.GetSkill(skillID, "SkillAnimationType");
		State = ObjectState.SKILL;
		return true;
	}

	protected virtual void CheckUpdatedFlag()
	{

	}

	protected virtual void AnimationEndEvent()
    {
		switch(State)
        {
			case ObjectState.HIT:
			case ObjectState.SKILL:
				State = ObjectState.IDLE;
				break;
			case ObjectState.DEAD:
				OnDeadEffect();
				break;
        }
    }

    public override void OnHit(int damage, bool isCritical)
    {
		base.OnHit(damage, isCritical);
	}

	protected override void OnDead()
	{
		base.OnDead();
	}

	public Vector3 NamePos
    {
		get
        {
			return _name.transform.position;
		}
    }

	protected virtual void OnDeadEffect()
    {
		Vector3 effectPos = _name.transform.position;
		float offsetX = 1.5f;

		if (Dir == Dir.RIGHT)
			offsetX = -offsetX;

		effectPos.x += offsetX;
		CoreManagers.Obj.Add("Effect", "DeadBlood", effectPos, 30);
	}
}