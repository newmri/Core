using Info;
using Define;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MagicArrowController : ProjectileController
{
	private Vector3 _scale;

	public override ObjectInfoT ObjectInfo
	{
		get { return _objectInfo; }
		set
		{
			if (_objectInfo.Equals(value))
				return;

			_objectInfo = value;

			transform.localScale = _scale;

			if (Dir == Dir.LEFT)
				transform.localScale = new Vector3(-_scale.x, _scale.y, _scale.z);
		}
	}

	protected override void Init()
	{
		_posCorrection = new Vector3(0.0f, 2.3f);
		_scale = transform.localScale;

		base.Init();
	}

	protected override void UpdateAnimation()
	{

	}
}
