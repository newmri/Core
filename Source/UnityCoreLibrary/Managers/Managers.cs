using UnityEngine;

namespace UnityCoreLibrary
{
    public class CoreManagers : MonoBehaviour
    {
        static CoreManagers s_instance; // 유일성이 보장된다
        public static CoreManagers Instance { get { Init(); return s_instance; } } // 유일한 매니저를 갖고온다

        PoolManager _pool = new PoolManager();
        ResourceManager _resource = new ResourceManager();
        SceneManagerEx _scene = new SceneManagerEx();
        ObjectManager _obj = new ObjectManager();
        CoroutineManager _coroutine;

        public static PoolManager Pool { get { return Instance._pool; } }
        public static ResourceManager Resource { get { return Instance._resource; } }
        public static SceneManagerEx Scene { get { return Instance._scene; } }
        public static ObjectManager Obj { get { return Instance._obj; } }
        public static CoroutineManager Coroutine { get { return Instance._coroutine; } }

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
                GameObject managersObject = GameObject.Find("@CoreManagers");
                if (managersObject == null)
                {
                    managersObject = new GameObject { name = "@CoreManagers" };
                    managersObject.AddComponent<CoreManagers>();
                }

                DontDestroyOnLoad(managersObject);
                s_instance = managersObject.GetComponent<CoreManagers>();

                s_instance._pool.Init();

                s_instance._coroutine = Util.GetOrAddComponent<CoroutineManager>("@Coroutine", managersObject.transform);
            }
        }

        public static void Clear()
        {
            Pool.Clear();
        }
    }

}
