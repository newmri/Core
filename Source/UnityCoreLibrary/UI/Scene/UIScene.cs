namespace UnityCoreLibrary
{
    public class UIScene : UIBase
    {
        public override void Init()
        {
            CoreManagers.UI.SetCanvas(gameObject, false);
        }
    }
}
