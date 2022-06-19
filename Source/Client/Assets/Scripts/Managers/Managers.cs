using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;

public class Managers : MonoBehaviour
{
    static Managers s_instance; // 유일성이 보장된다
    public static Managers Instance { get { Init(); return s_instance; } } // 유일한 매니저를 갖고온다

    #region Contents
    ItemManager _item = new ItemManager();
    EffectDataManager _effectData = new EffectDataManager();
    GradeManager _grade = new GradeManager();

    public static ItemManager Item { get { return Instance._item; } }
    public static EffectDataManager EffectData { get { return Instance._effectData; } }
    public static GradeManager Grade { get { return Instance._grade; } }
    #endregion

    #region Core
    UIManager _ui = new UIManager();
    WebManager _web = new WebManager();
    LoginNetworkManager _loginNetwork = new LoginNetworkManager();
    GameNetworkManager _gameNetwork = new GameNetworkManager();
    LoginDataManager _loginData = new LoginDataManager();
    MapManager _map = new MapManager();
    AccountManager _account = new AccountManager();
    ObjectManager _object = new ObjectManager();
    CharacterDataManager _characterData = new CharacterDataManager();

    public static UIManager UI { get { return Instance._ui; } }
    public static WebManager Web { get { return Instance._web; } }
    public static LoginNetworkManager LoginNetwork { get { return Instance._loginNetwork; } }
    public static GameNetworkManager GameNetwork { get { return Instance._gameNetwork; } }
    public static LoginDataManager LoginData { get { return Instance._loginData; } }
    public static MapManager Map { get { return Instance._map; } }
    public static AccountManager Account { get { return Instance._account; } }
    public static ObjectManager Object { get { return Instance._object; } }
    public static CharacterDataManager CharacterData { get { return Instance._characterData; } }

    #endregion

    void Start()
    {
        Init();
    }

    void Update()
    {
        _loginNetwork.Update();
        _gameNetwork.Update();
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

            LoginNetwork.Init();
            GameNetwork.Init();
        }		
	}

    void OnApplicationQuit()
    {
        LoginNetwork.Disconnect(false);
        GameNetwork.Disconnect(false);
    }

    public static void Clear()
    {
        UI.Clear();
        Object.Clear();
    }
}
