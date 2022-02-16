using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Managers : MonoBehaviour
{
    static Managers s_instance; // 유일성이 보장된다
    static Managers Instance { get { Init(); return s_instance; } } // 유일한 매니저를 갖고온다

    #region Contents

    #endregion

    #region Core

    #endregion

    void Start()
    {
        Init();
	}

    void Update()
    {

    }

    static void Init()
    {
        if (s_instance == null)
        {
			GameObject managersObject = GameObject.Find("@Managers");
            if (managersObject == null)
            {
                managersObject = new GameObject { name = "@Managers" };
                managersObject.AddComponent<Managers>();
            }

            DontDestroyOnLoad(managersObject);
            s_instance = managersObject.GetComponent<Managers>();
        }		
	}

    public static void Clear()
    {

    }
}
