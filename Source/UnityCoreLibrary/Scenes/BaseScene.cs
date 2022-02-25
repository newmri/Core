using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

namespace UnityCoreLibrary
{
    public abstract class BaseScene : MonoBehaviour
    {
        public CoreDefine.Scene SceneType { get; protected set; } = CoreDefine.Scene.Unknown;

        private void Awake()
        {
            
        }

        protected virtual void Init(CoreDefine.Scene scene)
        {
            SceneType = scene;
            Util.GetOrCreateGameObject<EventSystem>("UI/EventSystem", "@EventSystem");
            Object.DontDestroyOnLoad(gameObject);
        }

        public abstract void Clear();
    }
}
