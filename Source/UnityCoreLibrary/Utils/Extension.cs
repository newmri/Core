using System;
using UnityEngine;
using UnityEngine.EventSystems;

namespace UnityCoreLibrary
{
	public static class Extension
	{
		public static void Init(this GameObject go)
		{
			
		}

		public static T GetOrAddComponent<T>(this GameObject go) where T : UnityEngine.Component
		{
			return Util.GetOrAddComponent<T>(go);
		}

		public static bool IsValid(this GameObject go)
		{
			return go != null && go.activeSelf;
		}

		public static T ToEnum<T>(this string value)
		{
			if (!System.Enum.IsDefined(typeof(T), value))
				return default(T);

			return (T)System.Enum.Parse(typeof(T), value, true);
		}

		public static long GetID(this GameObject go)
		{
			Poolable poolable = go.GetComponent<Poolable>();
			if (poolable)
				return poolable.ID;

			return 0;
		}

		public static ObjState GetState(this GameObject go)
		{
			Poolable poolable = go.GetComponent<Poolable>();
			if (poolable)
				return poolable.State;

			return 0;
		}

		public static void SetState(this GameObject go, ObjState objState)
		{
			Poolable poolable = go.GetComponent<Poolable>();
			if (poolable)
				poolable.State = objState;
		}
	}
}
