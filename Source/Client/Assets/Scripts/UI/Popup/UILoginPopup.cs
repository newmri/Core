using UnityEngine.EventSystems;
using UnityEngine.UI;

public class UILoginPopup : UIPopup
{
    enum Buttons
    {
        SignupButton,
        CloseButton
    }

    public override void Init()
    {
        base.Init();

        Bind<Button>(typeof(Buttons));

        GetButton((int)Buttons.SignupButton).gameObject.BindEvent(OnClickSignupButton);
        GetButton((int)Buttons.CloseButton).gameObject.BindEvent(OnClickCloseButton);
    }

    public void OnClickSignupButton(PointerEventData evt)
    {
        gameObject.SetActive(false);
        Managers.UI.ShowPopupUI<UISignupPopup>();
    }

    public void OnClickCloseButton(PointerEventData evt)
    {
        Managers.UI.ClosePopupUI();
    }
}
