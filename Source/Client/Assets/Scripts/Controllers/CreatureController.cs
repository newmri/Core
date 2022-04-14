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
	protected SkillAnimationType _skillAnimationType = SkillAnimationType.NONE;

	protected override void Init()
	{
		base.Init();
	}

	public virtual bool UseSkill(int SkillID)
	{
		return false;
	}
}