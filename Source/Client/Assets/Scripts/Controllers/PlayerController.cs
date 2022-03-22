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
	protected CharacterInfoBaseUnion _characterInfo;

	protected GearEquipper _gear;

	public CharacterInfoT CharacterInfo
	{
		get
		{
			return _characterInfo.AsCharacterInfo();
		}
		set
		{
			if (_characterInfo != null && _characterInfo.Value.Equals(value))
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
	}

	protected override void UpdateAnimation()
	{
		if (_animator == null)
			return;

		_animator.ChangeAnimation(State, Dir);
	}

	protected override void UpdateController()
	{
		base.UpdateController();
	}

	public override void UseSkill(int skillId)
	{

	}

	protected virtual void CheckUpdatedFlag()
	{

	}
	public override void OnDamaged()
	{
		Debug.Log("Player HIT !");
	}
}