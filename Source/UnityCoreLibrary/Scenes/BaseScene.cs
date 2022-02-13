using UnityEngine;
using UnityEngine.EventSystems;

namespace UnityCoreLibrary
{
    public abstract class BaseScene : MonoBehaviour
    {
        public Define.Scene SceneType { get; protected set; } = Define.Scene.Unknown;

        private void Awake()
        {
            
        }

        protected virtual void Init(Define.Scene scene)
        {
            SceneType = scene;
            Util.GetOrCreateGameObject<EventSystem>("UI/EventSystem", "@EventSystem");
            Object.DontDestroyOnLoad(gameObject);
        }

        public abstract void Clear();
    }
}
