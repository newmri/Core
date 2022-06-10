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
        Dictionary<long, GameObject> _clientObjects = new Dictionary<long, GameObject>();
        Dictionary<long, GameObject> _serverObjects = new Dictionary<long, GameObject>();

        #region ClientObject
        long _counter = 1;
        public GameObject Add(GameObject prefab, Vector2Int? pos = null, int poolCount = 1, Transform parent = null)
        {
            return Add(prefab, new Vector3(pos.GetValueOrDefault().x, pos.GetValueOrDefault().y), poolCount, parent);
        }

        public GameObject Add(GameObject prefab, Vector3 pos, int poolCount = 1, Transform parent = null)
        {
            GameObject gameObject = CoreManagers.Resource.Instantiate(_counter, prefab, pos, poolCount, parent);
            AddClientObjectPostProcess(gameObject.tag, gameObject);
            return gameObject;
        }

        public GameObject Add(string type, string name, Vector2Int? pos = null, int poolCount = 1, Transform parent = null)
        {
            return Add(type, name, new Vector3(pos.GetValueOrDefault().x, pos.GetValueOrDefault().y), poolCount, parent);
        }

        public GameObject Add(string type, string name, Vector3 pos, int poolCount = 1, Transform parent = null)
        {
            GameObject gameObject = CoreManagers.Resource.Instantiate(_counter, type, name, pos, poolCount, parent);
            AddClientObjectPostProcess(type, gameObject);
            return gameObject;
        }

        private void AddClientObjectPostProcess(string type, GameObject gameObject)
        {
            _clientObjects.Add(_counter++, gameObject);
            gameObject.SetActive(true);
        }

        public void RemoveClientObject(long ID, ObjRemoveReason reason = ObjRemoveReason.Unknown)
        {
            GameObject gameObject = FindClientObject(ID);
            if (null == gameObject)
                return;

            _clientObjects.Remove(ID);

            CoreManagers.Resource.Destroy(gameObject);
        }

        public GameObject FindClientObject(long ID)
        {
            GameObject gameObject = null;
            _clientObjects.TryGetValue(ID, out gameObject);
            return gameObject;
        }
        #endregion

        #region ServerObject
        public GameObject Add(long oid, GameObject prefab, Vector2Int? pos = null, int poolCount = 1, Transform parent = null)
        {
            return Add(oid, prefab, new Vector3(pos.GetValueOrDefault().x, pos.GetValueOrDefault().y), poolCount, parent);
        }

        public GameObject Add(long oid, GameObject prefab, Vector3 pos, int poolCount = 1, Transform parent = null)
        {
            GameObject gameObject = CoreManagers.Resource.Instantiate(oid, prefab, pos, poolCount, parent);
            AddServerObjectPostProcess(oid, gameObject);
            return gameObject;
        }

        public GameObject Add(long oid, string type, string name, Vector2Int? pos = null, int poolCount = 1, Transform parent = null)
        {
            return Add(oid, type, name, new Vector3(pos.GetValueOrDefault().x, pos.GetValueOrDefault().y), poolCount, parent);
        }

        public GameObject Add(long oid, string type, string name, Vector3 pos, int poolCount = 1, Transform parent = null)
        {
            GameObject gameObject = CoreManagers.Resource.Instantiate(oid, type, name, pos, poolCount, parent);
            AddServerObjectPostProcess(oid, gameObject);
            return gameObject;
        }

        private void AddServerObjectPostProcess(long oid, GameObject gameObject)
        {
            _serverObjects.Add(oid, gameObject);
            gameObject.SetActive(true);
        }

        public void RemoveServerObject(long ID, ObjRemoveReason reason = ObjRemoveReason.Unknown)
        {
            GameObject gameObject = FindServerObject(ID);
            if (null == gameObject)
                return;

            _serverObjects.Remove(ID);
            CoreManagers.Resource.Destroy(gameObject);
        }

        public GameObject FindServerObject(long ID)
        {
            GameObject gameObject = null;
            _serverObjects.TryGetValue(ID, out gameObject);
            return gameObject;
        }
        #endregion

        public void Clear()
        {
            foreach (var obj in _clientObjects)
                CoreManagers.Resource.Destroy(obj.Value);

            _clientObjects.Clear();

            foreach (var obj in _serverObjects)
                CoreManagers.Resource.Destroy(obj.Value);

            _serverObjects.Clear();
        }
    }
}
