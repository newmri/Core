using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Define;
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
	private Coroutine _skillCoroutine;

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
		if (_skillCoroutine != null)
			CoreManagers.Coroutine.Stop(_skillCoroutine);

		_skillCoroutine = CoreManagers.Coroutine.Add(CoSkill(skillID,
			(float)((int)Managers.CharacterData.GetSkill(skillID, "CoolTime") / 1000.0f)));

		return true;
	}

	IEnumerator CoSkill(int skillID, float coolTime)
    {
		_skillAnimationType = (SkillAnimationType)(int)Managers.CharacterData.GetSkill(skillID, "SkillAnimationType");
		State = CreatureState.SKILL;
		yield return new WaitForSeconds(coolTime);
		State = CreatureState.IDLE;
	}

	protected virtual void CheckUpdatedFlag()
	{

	}
}