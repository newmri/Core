using System;
using UnityEngine;
using UnityEngine.EventSystems;

namespace UnityCoreLibrary
{
    public class UIEventHandler : MonoBehaviour, IPointerEnterHandler, IPointerExitHandler, IPointerClickHandler, IDragHandler
    {
        public Action<PointerEventData> OnEnterHander = null;
        public Action<PointerEventData> OnExitHander = null;
        public Action<PointerEventData> OnClickHander = null;
        public Action<PointerEventData> OnDragHandler = null;

        public void OnPointerEnter(PointerEventData eventData)
        {
            if (OnEnterHander != null)
                OnEnterHander.Invoke(eventData);
        }

        public void OnPointerExit(PointerEventData eventData)
        {
            if (OnExitHander != null)
                OnExitHander.Invoke(eventData);
        }

        public void OnPointerClick(PointerEventData eventData)
        {
            if (OnClickHander != null)
                OnClickHander.Invoke(eventData);
        }

        public void OnDrag(PointerEventData eventData)
        {
            if (OnDragHandler != null)
                OnDragHandler.Invoke(eventData);
        }
    }
}