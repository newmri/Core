using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UnityEngine;

namespace UnityCoreLibrary
{
    public enum ObjRemoveReason
    {
        Unknown = 0,
        Dead = 1
    }

    public class ObjectManager
    {
        Dictionary<int, GameObject> _objects = new Dictionary<int, GameObject>();

        int _counter = 1;

        public GameObject Add(GameObject prefab, Vector2Int? pos = null, int poolCount = 1, Transform parent = null)
        {
            return Add(prefab, new Vector3(pos.GetValueOrDefault().x, pos.GetValueOrDefault().y), poolCount, parent);
        }

        public GameObject Add(GameObject prefab, Vector3 pos, int poolCount = 1, Transform parent = null)
        {
            GameObject gameObject = CoreManagers.Resource.Instantiate(_counter, prefab, pos, poolCount, parent);
            AddPostProcess(gameObject.tag, gameObject);
            return gameObject;
        }

        public GameObject Add(string type, string name, Vector2Int? pos = null, int poolCount = 1, Transform parent = null)
        {
            return Add(type, name, new Vector3(pos.GetValueOrDefault().x, pos.GetValueOrDefault().y), poolCount, parent);
        }

        public GameObject Add(string type, string name, Vector3 pos, int poolCount = 1, Transform parent = null)
        {
            GameObject gameObject = CoreManagers.Resource.Instantiate(_counter, type, name, pos, poolCount, parent);
            AddPostProcess(type, gameObject);
            return gameObject;
        }

        private void AddPostProcess(string type, GameObject gameObject)
        {
            _objects.Add(_counter++, gameObject);
            gameObject.SetActive(true);
        }

        public void Remove(int ID, ObjRemoveReason reason = ObjRemoveReason.Unknown)
        {
            GameObject gameObject = Find(ID);
            if (null == gameObject)
                return;

            _objects.Remove(ID);

            CoreManagers.Resource.Destroy(gameObject);
        }

        public GameObject Find(int ID)
        {
            GameObject gameObject = null;
            _objects.TryGetValue(ID, out gameObject);
            return gameObject;
        }
    }
}
