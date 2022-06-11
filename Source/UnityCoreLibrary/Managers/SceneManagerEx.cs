using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

namespace UnityCoreLibrary
{
    public class SceneManagerEx : MonoBehaviour
    {
        public float LoadingAmount;
        private string _nextScene;
        public Action _sceneLoadDoneAction;

        public BaseScene CurrentScene
        {
            get
            { 
                return GameObject.FindObjectOfType<BaseScene>(); 
            } 
        }

        public void LoadScene(CoreDefine.Scene type, Action sceneLoadDoneAction = null)
        {
            CoreManagers.Clear();
            Clear();

            _nextScene = GetSceneName(type);
            _sceneLoadDoneAction = sceneLoadDoneAction;

            SceneManager.LoadScene("Loading");
            StartCoroutine(LoadAsyncScene());
        }

        IEnumerator LoadAsyncScene()
        {
            yield return null;

            AsyncOperation asyncScene = SceneManager.LoadSceneAsync(_nextScene);
            asyncScene.allowSceneActivation = false;

            float timer = 0.0f;
            while (!asyncScene.isDone)
            {
                yield return null;

                timer += Time.deltaTime;
                float progress = asyncScene.progress;

                if (progress < 0.9f)
                {
                    LoadingAmount = Mathf.Lerp(LoadingAmount, progress, timer);

                    if (LoadingAmount >= progress)
                    {
                        timer = 0f;
                    }
                }
                else
                {
                    LoadingAmount = Mathf.Lerp(LoadingAmount, 1.0f, timer);

                    if (LoadingAmount == 1.0f)
                    {
                        asyncScene.allowSceneActivation = true;
                        CoreManagers.Resource.Destroy(CurrentScene.gameObject);
                        if (_sceneLoadDoneAction != null)
                            _sceneLoadDoneAction();

                        yield break;
                    }
                }
            }
        }

        private string GetSceneName(CoreDefine.Scene type)
        {
            string name = System.Enum.GetName(typeof(CoreDefine.Scene), type);
            return name;
        }

        public void Clear()
        {
            LoadingAmount = 0.0f;
            CurrentScene.Clear();

            var objectList = Resources.FindObjectsOfTypeAll<BaseScene>();

            foreach (var obj in objectList)
            {
                CoreManagers.Resource.Destroy(obj.gameObject);
            }
        }
    }
}