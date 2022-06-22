using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;
using UnityEngine;
using UnityEngine.EventSystems;

namespace UnityCoreLibrary
{
    public class Util
    {
        public static T GetOrAddComponent<T>(string name, Transform parent = null) where T : UnityEngine.Component
        {
            GameObject gameObject = GetOrCreateGameObject(name);

            T component = gameObject.GetComponent<T>();
            if (component == null)
            {
                component = gameObject.AddComponent<T>();
                component.transform.parent = parent;
            }

            return component;
        }

        public static T GetOrAddComponent<T>(GameObject gameObject) where T : UnityEngine.Component
        {
            T component = gameObject.GetComponent<T>();
            if (component == null)
                component = gameObject.AddComponent<T>();
            return component;
        }

        public static T GetOrCreateGameObject<T>(string path, string name) where T : UnityEngine.Component
        {
            UnityEngine.Object obj = GameObject.FindObjectOfType(typeof(T));
            if (obj == null)
            {
                GameObject gameObject = CoreManagers.Resource.Instantiate(path);
                gameObject.name = name;
                if (name.StartsWith("@"))
                    UnityEngine.Object.DontDestroyOnLoad(gameObject);
            }

            return obj as T;
        }

        public static GameObject GetOrCreateGameObject(string name)
        {
            GameObject gameObject = GameObject.Find(name);

            if (null == gameObject)
            {
                gameObject = new GameObject { name = name };
                if (name.StartsWith("@"))
                    UnityEngine.Object.DontDestroyOnLoad(gameObject);
            }

            return gameObject;
        }

        public static void Copy<T>(T source, ref T dest)
        {
            System.Type type = source.GetType();

            System.Reflection.PropertyInfo[] props = type.GetProperties();
            foreach (System.Reflection.PropertyInfo prop in props)
            {
                if (prop.CanWrite || prop.Name != "name")
                    prop.SetValue(dest, prop.GetValue(source, null), null);
            }
        }

        public static T CopyComponent<T>(T original, GameObject dest, HideFlags hide = HideFlags.None) where T : Component
        {
            System.Type type = original.GetType();
            Component copy = dest.AddComponent(type);
            copy.hideFlags = hide;

            System.Reflection.FieldInfo[] fields = type.GetFields();

            foreach (System.Reflection.FieldInfo field in fields)
            {
                field.SetValue(copy, field.GetValue(original));
            }

            System.Reflection.PropertyInfo[] props = type.GetProperties();
            foreach (System.Reflection.PropertyInfo prop in props)
            {
                if (prop.CanWrite || prop.Name != "name")
                    prop.SetValue(copy, prop.GetValue(original, null), null);
            }

            return copy as T;
        }

        public static GameObject FindChild(GameObject gameObject, string name = null, bool recursive = false)
        {
            Transform transform = FindChild<Transform>(gameObject, name, recursive);
            if (transform == null)
                return null;

            return transform.gameObject;
        }

        public static List<T> FindAllChildrens<T>(GameObject gameObject, string name = null, bool recursive = false) where T : UnityEngine.Object
        {
            List<T> childrens = new List<T>();

            if (gameObject == null)
                return childrens;

            if (recursive == false)
            {
                for (int i = 0; i < gameObject.transform.childCount; i++)
                {
                    Transform transform = gameObject.transform.GetChild(i);
                    if (string.IsNullOrEmpty(name) || transform.name == name)
                    {
                        T component = transform.GetComponent<T>();
                        if (component != null)
                            childrens.Add(component);
                    }
                }
            }
            else
            {
                foreach (T component in gameObject.GetComponentsInChildren<T>())
                {
                    if (string.IsNullOrEmpty(name) || component.name == name)
                        childrens.Add(component);
                }
            }

            return childrens;
        }

        public static T FindChild<T>(GameObject gameObject, string name = null, bool recursive = false) where T : UnityEngine.Object
        {
            if (gameObject == null)
                return null;

            if (recursive == false)
            {
                for (int i = 0; i < gameObject.transform.childCount; i++)
                {
                    Transform transform = gameObject.transform.GetChild(i);
                    if (string.IsNullOrEmpty(name) || transform.name == name)
                    {
                        T component = transform.GetComponent<T>();
                        if (component != null)
                            return component;
                    }
                }
            }
            else
            {
                foreach (T component in gameObject.GetComponentsInChildren<T>())
                {
                    if (string.IsNullOrEmpty(name) || component.name == name)
                        return component;
                }
            }

            return null;
        }

        public static bool IsInRange(Vector2Int dist, float range)
        {
            return dist.magnitude <= range;
        }

        public static void MakeTrajectoryLine(LineRenderer lineRenderer, Rigidbody2D rigidbody, Vector2 pos, Vector2 velocity, int steps)
        {
            Vector3[] trajectory = GetTrajectoryVector3(rigidbody, pos, velocity, steps);
            lineRenderer.positionCount = trajectory.Length;
            lineRenderer.SetPositions(trajectory);
        }

        public static Vector3[] GetTrajectoryVector3(Rigidbody2D rigidbody, Vector2 pos, Vector2 velocity, int steps)
        {
            Vector2[] trajectory = GetTrajectoryVector2(rigidbody, pos, velocity, steps);
            Vector3[] trajectory3 = new Vector3[trajectory.Length];

            for (int i = 0; i < trajectory3.Length; ++i)
            {
                trajectory3[i] = trajectory[i];
            }

            return trajectory3;
        }

        public static Vector2[] GetTrajectoryVector2(Rigidbody2D rigidbody, Vector2 pos, Vector2 velocity, int steps)
        {
            Vector2[] trajectory = new Vector2[steps];

            float timeStep = Time.fixedDeltaTime / Physics2D.velocityIterations;
            Vector2 gravityAccel = Physics2D.gravity * rigidbody.gravityScale * timeStep * timeStep;

            float drag = 1.0f - timeStep * rigidbody.drag;
            Vector2 moveStep = velocity * timeStep;

            for (int i = 0; i < steps; ++i)
            {
                moveStep += gravityAccel;
                moveStep *= drag;
                pos += moveStep;
                trajectory[i] = pos;
            }

            return trajectory;
        }

        public static bool IsMouseInJoystickArea(Vector3 mousePos)
        {
            PointerEventData eventDataCurrentPosition = new PointerEventData(EventSystem.current);
            eventDataCurrentPosition.position = new Vector2(mousePos.x, mousePos.y);

            List<RaycastResult> results = new List<RaycastResult>();
            EventSystem.current.RaycastAll(eventDataCurrentPosition, results);
            for (int i = 0; i < results.Count; ++i)
            {
                if (results[i].gameObject.tag == "Joystick")
                    return true;
            }

            return false;
        }

        public static T PascalStringToEnum<T>(string name)
        {
            string upperName = name;
            int count = 0;
            for (int i = 0; i < name.Length; ++i)
            {
                if (i > 0 && Char.IsUpper(name[i]))
                {
                    upperName = upperName.Insert(i + count, "_");
                    ++count;
                }
            }

            upperName = upperName.ToUpper();
            return StringToEnum<T>(upperName);
        }

        public static T StringToEnum<T>(string name)
        {
            var type = typeof(T);

            if (type.ToString() != name)
                name = name.Replace(type.FullName, "");

            return (T)Enum.Parse(type, name);
        }


        public static string EnumToPascal(string str)
        {
            string lowerName = str.ToLower();
            string[] names = Regex.Split(lowerName, "_");
            string resultName = "";
            foreach (string name in names)
            {
                string temp = name;
                temp = temp.Remove(0, 1);
                temp = temp.Insert(0, name.First().ToString().ToUpper());
                resultName += temp;
            }

            return resultName;
        }
    }
}
