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
	protected override void Init()
	{
		base.Init();
	}

	public virtual void OnDamaged()
	{

	}

	public virtual void OnDead()
	{
		State = CreatureState.DEAD;

		//GameObject effect = Managers.Resource.Instantiate("Effect/DieEffect");
		//effect.transform.position = transform.position;
		//effect.GetComponent<Animator>().Play("START");
		//GameObject.Destroy(effect, 0.5f);
	}

	public virtual void UseSkill(int skillId)
	{

	}
}