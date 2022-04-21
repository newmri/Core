using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Define;

public class ArrowController : BaseController
{
	protected override void Init()
	{
		switch (Dir)
		{
			case Dir.LEFT:
				transform.rotation = Quaternion.Euler(0, 0, -90);
				break;
		}

		State = ObjectState.WALK;

		base.Init();
	}

	protected override void UpdateAnimation()
	{

	}
}
