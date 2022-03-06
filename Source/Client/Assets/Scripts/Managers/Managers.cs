using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;

public class Managers : MonoBehaviour
{
    static Managers s_instance; // 유일성이 보장된다
    public static Managers Instance { get { Init(); return s_instance; } } // 유일한 매니저를 갖고온다

    #region Contents

    #endregion

    #region Core
    UIManager _ui = new UIManager();
    WebManager _web = new WebManager();
    LoginNetworkManager _loginNetwork = new LoginNetworkManager();
    LoginDataManager _loginData = new LoginDataManager();
    MapManager _map = new MapManager();

    public static UIManager UI { get { return Instance._ui; } }
    public static WebManager Web { get { return Instance._web; } }
    public static LoginNetworkManager LoginNetwork { get { return Instance._loginNetwork; } }
    public static LoginDataManager LoginData { get { return Instance._loginData; } }
    public static MapManager Map { get { return Instance._map; } }

    #endregion

    void Start()
    {
        Init();
    }

    void Update()
    {
        _loginNetwork.Update();
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
        UI.Clear();
    }
}
