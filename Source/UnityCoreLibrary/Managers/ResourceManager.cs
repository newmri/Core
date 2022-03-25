using UnityEngine;

namespace UnityCoreLibrary
{
    public class ResourceManager
    {
        public T Load<T>(string path) where T : Object
        {
            if (typeof(T) == typeof(GameObject))
            {
                string name = path;
                int index = name.LastIndexOf('/');
                if (index >= 0)
                    name = name.Substring(index + 1);

                GameObject go = CoreManagers.Pool.GetOriginal(name);
                if (go != null)
                    return go as T;
            }

            return Resources.Load<T>(path);
        }

        public GameObject LoadGameObject(GameObject gameObject)
        {
            GameObject go = CoreManagers.Pool.GetOriginal(gameObject.name);
            if (go != null)
                return go;

            return Object.Instantiate(gameObject);
        }

        public GameObject Instantiate(string path, Transform parent = null, int poolCount = 1)
        {
            GameObject original = Load<GameObject>($"Prefabs/{path}");
            if (original == null)
            {
                Debug.Log($"Failed to load prefab : {path}");
                return null;
            }

            if (original.GetComponent<Poolable>() != null)
            {
                return CoreManagers.Pool.Pop(original, poolCount, parent).gameObject;
            }

            GameObject go = Object.Instantiate(original, parent);
            go.name = original.name;

            return go;
        }

        public GameObject Instantiate(long ID, string type, string name, Vector3? pos = null, int poolCount = 1, Transform parent = null)
        {
            string path = $"Prefabs/{type}/{name}";
            GameObject original = Load<GameObject>(path);
            if (original == null)
            {
                Debug.Log($"Failed to load prefab : {path}");
                return null;
            }

            if (original.GetComponent<Poolable>() != null)
            {
                Poolable poolable = CoreManagers.Pool.Pop(original, poolCount, parent);
                SetPoolableInfo(poolable, ID, type, name, pos.GetValueOrDefault(), parent);
                return poolable.gameObject;
            }

            GameObject go = Object.Instantiate(original, parent);
            go.tag = type;
            go.name = original.name;

            return go;
        }

        public GameObject Instantiate(long ID, GameObject prefab, Vector3? pos = null, int poolCount = 1, Transform parent = null)
        {
            if (prefab.GetComponent<Poolable>() != null)
            {
                Poolable poolable = CoreManagers.Pool.Pop(prefab, poolCount, parent);
                SetPoolableInfo(poolable, ID, prefab.tag, prefab.name, pos.GetValueOrDefault(), parent);
                return poolable.gameObject;
            }

            GameObject go = Object.Instantiate(prefab, parent);
            go.tag = prefab.tag;
            go.name = prefab.name;

            return go;
        }

        private void SetPoolableInfo(Poolable poolable, long ID, string type, string name, Vector3 pos, Transform parent)
        {
            poolable.ID = ID;
            poolable.tag = type;
            poolable.name = name;
            poolable.transform.position = pos;
            poolable.transform.parent = parent;
        }

        public void Destroy(GameObject go)
        {
            if (go == null)
                return;

            Poolable poolable = go.GetComponent<Poolable>();
            if (poolable != null)
            {
                CoreManagers.Pool.Push(poolable);
                return;
            }

            Object.Destroy(go);
        }
    }
}
