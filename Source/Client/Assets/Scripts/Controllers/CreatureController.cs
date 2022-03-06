using System.Collections;
using System.Collections.Generic;
using System.Configuration;
using UnityEngine;
using Define;
using UnityCoreLibrary;
using Spine.Unity;

public class CreatureController : BaseController
{
	public override StatInfo Stat
	{
		get { return base.Stat; }
		set
		{
			base.Stat = value;
			//UpdateHpBar();
		}
	}

	public override int HP
	{
		get { return base.HP; }
		set
		{
			base.HP = value;
			//UpdateHpBar();
		}

	}

	//protected void AddHpBar()
	//{
	//	GameObject go = Managers.Resource.Instantiate("UI/HpBar", transform);
	//	go.transform.localPosition = new Vector3(0.0f, 0.5f, 0.0f);
	//	go.name = "HpBar";
	//	_hpBar = go.GetComponent<HpBar>();
	//	UpdateHpBar();
	//}

	//void UpdateHpBar()
	//{
	//	if (null == _hpBar)
	//		return;

	//	float ratio = 0.0f;

	//	if (Stat.MaxHp > 0)
	//	{
	//		ratio = ((float)Stat.Hp) / Stat.MaxHp;
	//	}

	//	_hpBar.SetHpBar(ratio);
	//}

	protected override void Init()
	{
		base.Init();
		//AddHpBar();
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