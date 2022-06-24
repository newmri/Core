using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityCoreLibrary;
using UnityEngine.UI;
using TMPro;

public class UIManager
{
    //Sort Order
    static int _defaultSortOrder = 0;
    int _sortOrder = _defaultSortOrder;

    Stack<UIPopup> _popupStack = new Stack<UIPopup>();
    UIScene _sceneUI = null;

    public GameObject Root
    {
        get
        {
            GameObject root = GameObject.Find("@UI_Root");
            if (root == null)
                root = new GameObject { name = "@UI_Root" };

            return root;
        }
    }

    public void SetCanvas(GameObject go, bool sort = true)
    {
        Canvas canvas = Util.GetOrAddComponent<Canvas>(go);
        canvas.renderMode = RenderMode.ScreenSpaceCamera;
        canvas.worldCamera = Camera.main;
        canvas.overrideSorting = true;

        if (sort)
        {
            canvas.sortingLayerName = "UI";
            canvas.sortingOrder = _sortOrder;
            _sortOrder++;
        }
        else
        {
            canvas.sortingOrder = 0;
        }

        Util.GetOrAddComponent<CanvasScaler>(go).uiScaleMode = CanvasScaler.ScaleMode.ScaleWithScreenSize;
        Util.GetOrAddComponent<CanvasScaler>(go).referenceResolution = new Vector2(1920, 1080);
        Util.GetOrAddComponent<CanvasScaler>(go).screenMatchMode = CanvasScaler.ScreenMatchMode.Expand;

        Util.GetOrAddComponent<GraphicRaycaster>(go);
    }

    public T ShowPopupUI<T>(string name = null) where T : UIPopup
    {
        if (name == null)
            name = typeof(T).Name;

        GameObject go = CoreManagers.Resource.Instantiate($"UI/Popup/{name}");

        T popup = Util.GetOrAddComponent<T>(go);
        _popupStack.Push(popup);

        go.transform.SetParent(Root.transform);

        return popup;
    }

    public T FindPopupUI<T>(string name = null) where T : UIPopup
    {
        if (0 == _popupStack.Count)
            return null;

        if (name == null)
            name = typeof(T).Name;

        foreach (UIPopup popup in _popupStack)
        {
            if (popup.name == name)
                return popup as T;
        }

        return null;
    }

    public T ShowSceneUI<T>(string name = null) where T : UIScene
    {
        if (name == null)
            name = typeof(T).Name;

        GameObject go = CoreManagers.Resource.Instantiate($"UI/Scene/{name}");

        T sceneUI = Util.GetOrAddComponent<T>(go);
        _sceneUI = sceneUI;

        go.transform.SetParent(Root.transform);

        return sceneUI;
    }

    public T GetSceneUI<T>() where T : UIScene
    {
        return _sceneUI as T;
    }

    public void HideCurrentSceneUI()
    {
        if (_sceneUI)
            _sceneUI.gameObject.SetActive(false);
    }

    public void ShowCurrentSceneUI()
    {
        if (_sceneUI)
            _sceneUI.gameObject.SetActive(true);
    }

    public T MakeWorldSpaceUI<T>(string name = null, Transform parent = null) where T : UIBase
    {
        if (name == null)
            name = typeof(T).Name;

        GameObject go = CoreManagers.Resource.Instantiate($"UI/WorldSpace/{name}");

        if (parent != null)
            go.transform.SetParent(parent);

        Canvas canvas = go.GetOrAddComponent<Canvas>();
        canvas.renderMode = RenderMode.WorldSpace;
        canvas.worldCamera = Camera.main;

        return go.GetOrAddComponent<T>();
    }

    public T MakeSubItem<T>(string name = null, Transform parent = null) where T : UIBase
    {
        if (name == null)
            name = typeof(T).Name;

        GameObject go = CoreManagers.Resource.Instantiate($"UI/SubItem/{name}");

        if (parent != null)
            go.transform.SetParent(parent);

        return go.GetOrAddComponent<T>();
    }

    public void ClosePopupUI(UIPopup uiPopup)
    {
        if (_popupStack.Count == 0)
            return;

        if (_popupStack.Peek() != uiPopup)
        {
            Debug.Log("Close Popup Failed!");
            return;
        }
        ClosePopupUI();
    }


    public void ClosePopupUI(bool setActive = false)
    {
        if (_popupStack.Count == 0)
            return;

        UIPopup popup = _popupStack.Pop();
        CoreManagers.Resource.Destroy(popup.gameObject);
        popup = null;

        _sortOrder--;

        if (setActive && _popupStack.Count > 0)
        {
            _popupStack.Peek().gameObject.SetActive(true);
        }
    }


    public void CloseAllPopupUI()
    {
        while (_popupStack.Count > 0)
            ClosePopupUI();
    }

    public void Clear()
    {
        CloseAllPopupUI();
        _sceneUI = null;
        _sortOrder = _defaultSortOrder;
    }
}