using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;
using UnityCoreLibrary;

namespace UnityCoreLibrary.UI
{
    public abstract class UIAwakeBase : MonoBehaviour
    {
        Dictionary<Type, UnityEngine.Object[]> _objests = new Dictionary<Type, UnityEngine.Object[]>();

        private void Awake()
        {
            Init();
        }

        public abstract void Init();

        protected void Bind<T>(Type type) where T : UnityEngine.Object
        {
            string[] names = Enum.GetNames(type);

            UnityEngine.Object[] objects = new UnityEngine.Object[names.Length];
            _objests.Add(typeof(T), objects);

            for (int i = 0; i < names.Length; ++i)
            {
                if (typeof(T) == typeof(GameObject))
                    objects[i] = Util.FindChild(gameObject, names[i], true);
                else
                    objects[i] = Util.FindChild<T>(gameObject, names[i], true);
            }
        }

        public T Get<T>(int index) where T : UnityEngine.Object
        {
            UnityEngine.Object[] objects = null;
            if (_objests.TryGetValue(typeof(T), out objects) == false)
                return null;

            return objects[index] as T;
        }

        protected GameObject GetObject(int index)
        {
            return Get<GameObject>(index);
        }

        protected Text GetText(int index)
        {
            return Get<Text>(index);
        }

        protected Button GetButton(int index)
        {
            return Get<Button>(index);
        }

        protected Image GetImage(int index)
        {
            return Get<Image>(index);
        }

        protected Slider GetSlider(int index)
        {
            return Get<Slider>(index);
        }

        public static void BindEvent(GameObject go, Action<PointerEventData> action, CoreDefine.UIEvent type = CoreDefine.UIEvent.Click)
        {
            UIEventHandler uiEvent = Util.GetOrAddComponent<UIEventHandler>(go);

            switch (type)
            {
                case CoreDefine.UIEvent.Enter:
                    uiEvent.OnEnterHander -= action;
                    uiEvent.OnEnterHander += action;
                    break;
                case CoreDefine.UIEvent.Exit:
                    uiEvent.OnExitHander -= action;
                    uiEvent.OnExitHander += action;
                    break;
                case CoreDefine.UIEvent.Click:
                    uiEvent.OnClickHander -= action;
                    uiEvent.OnClickHander += action;
                    break;
                case CoreDefine.UIEvent.Drag:
                    uiEvent.OnDragHandler -= action;
                    uiEvent.OnDragHandler += action;
                    break;
            }
        }

        public GameObject GetEmptyGridLayoutGroup()
        {
            GameObject grid = GetComponentInChildren<GridLayoutGroup>().gameObject;
            foreach (Transform child in grid.transform)
                Destroy(child.gameObject);

            return grid;
        }

        public GameObject GetHorizontalLayoutGroup()
        {
            GameObject grid = GetComponentInChildren<HorizontalLayoutGroup>().gameObject;
            foreach (Transform child in grid.transform)
                Destroy(child.gameObject);

            return grid;
        }
    }
}
