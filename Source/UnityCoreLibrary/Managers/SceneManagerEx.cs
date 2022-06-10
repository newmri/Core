using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

namespace UnityCoreLibrary
{
    public class SceneManagerEx : MonoBehaviour
    {
        private float _fillAmount;
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
                Debug.Log(_fillAmount);
                if (asyncScene.progress < 0.9f)
                {
                    _fillAmount = Mathf.Lerp(_fillAmount, asyncScene.progress, timer);
                    if (_fillAmount >= asyncScene.progress)
                    {
                        timer = 0f;
                    }
                }
                else
                {
                    _fillAmount = Mathf.Lerp(_fillAmount, 1f, timer);
                    if (_fillAmount == 1.0f)
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
            CurrentScene.Clear();

            var objectList = Resources.FindObjectsOfTypeAll<BaseScene>();

            foreach (var obj in objectList)
            {
                CoreManagers.Resource.Destroy(obj.gameObject);
            }
        }
    }
}