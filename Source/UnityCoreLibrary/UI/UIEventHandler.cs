using System;
using UnityEngine;
using UnityEngine.EventSystems;

namespace UnityCoreLibrary
{
    public class UIEventHandler : MonoBehaviour, IPointerClickHandler, IDragHandler
    {
        public Action<PointerEventData> OnClickHander = null;
        public Action<PointerEventData> OnDragHandler = null;

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
