using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace UnityCoreLibrary
{
	public enum ObjState
	{
		InPool = 0,
		OutPool = 1
	}

	public class Poolable : MonoBehaviour
	{
		public long ID;
		public ObjState State;
	}
}
