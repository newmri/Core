using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Define;
using Spine;
using Spine.Unity;
using UnityCoreLibrary;
using GamePacket;
using FlatBuffers;

public class PlayerController : CreatureController
{
	protected CharacterAnimator _animator;

	[SerializeField]
	protected CharacterInfoBaseUnion _characterInfo = new CharacterInfoBaseUnion();

	protected GearEquipper _gear;

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

			_gear.Job = CharacterInfo.Job;
			_gear.SetGear(CharacterInfo.Gear);
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
		mesh.sortingOrder = Managers.Creature.GetSortOrder(this);
	}

	protected override void UpdateAnimation()
	{
		if (_animator == null)
			return;

		_animator.ChangeAnimation(State, Dir, _skillAnimationType);
	}

	protected override void UpdateController()
	{
		base.UpdateController();
	}

	public override bool UseSkill(int skillID)
	{
		if (IsDead())
			return false;

		_skillAnimationType = (SkillAnimationType)(int)Managers.CharacterData.GetSkill(skillID, "SkillAnimationType");
		State = CreatureState.SKILL;
		return true;
	}

	protected virtual void CheckUpdatedFlag()
	{

	}

	protected virtual void AnimationEndEvent()
    {
		switch(State)
        {
			case CreatureState.HIT:
			case CreatureState.SKILL:
				State = CreatureState.IDLE;
				break;
        }
    }
}