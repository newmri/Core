using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Define;
using Spine.Unity;
using UnityCoreLibrary;

public class PlayerController : CreatureController
{
	protected CharacterAnimator _animator;

	protected override void Init()
	{
		base.Init();
		_animator = GetComponent<CharacterAnimator>();
		_animator.JobChanged(Job.WARRIOR);
	}

	protected override void UpdateAnimation()
	{
		if (_animator == null)
			return;

		if (State == CreatureState.IDLE)
		{
			_animator.ChangeAnimation("Idle", Dir);

		}
		else if (State == CreatureState.MOVE)
		{
			_animator.ChangeAnimation("Walk", Dir);
		}
		else if (State == CreatureState.SKILL)
		{
			
		}
		else
		{

		}
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