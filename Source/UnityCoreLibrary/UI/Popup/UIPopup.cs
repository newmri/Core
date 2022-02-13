namespace UnityCoreLibrary
{
    public class UIPopup : UIBase
    {
        public override void Init()
        {
            CoreManagers.UI.SetCanvas(gameObject, true);
        }

        public virtual void ClosePopupUI()
        {
            CoreManagers.UI.ClosePopupUI(this);
        }
    }
}
