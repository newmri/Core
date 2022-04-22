using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Define;
using Info;

public class ProjectileController : BaseController
{
	public override ObjectInfoT ObjectInfo
	{
		get { return _objectInfo; }
		set
		{
			if (_objectInfo.Equals(value))
				return;

			_objectInfo = value;

			if(Dir == Dir.LEFT)
				transform.rotation = Quaternion.Euler(0, 0, -90);
			else
				transform.rotation = Quaternion.Euler(0, 0, 90);
		}
	}

	protected override void Init()
	{
		base.Init();
	}

	protected override void UpdateAnimation()
	{

	}
}
