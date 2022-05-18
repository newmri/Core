using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Define;
using Info;

public class ArrowController : ProjectileController
{
	public override ObjectInfoWithPosT ObjectInfoWithPos
	{
		get { return _objectInfoWithPos; }
		set
		{
			if (_objectInfoWithPos.Equals(value))
				return;

			_objectInfoWithPos = value;

			if (Dir == Dir.LEFT)
				transform.rotation = Quaternion.Euler(0, 0, -90);
			else
				transform.rotation = Quaternion.Euler(0, 0, 90);
		}
	}

	protected override void Init()
	{
		_posCorrection = new Vector3(0.0f, 2.7f);
		base.Init();
	}

	protected override void UpdateAnimation()
	{

	}
}
