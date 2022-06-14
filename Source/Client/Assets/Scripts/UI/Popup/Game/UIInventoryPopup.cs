using TMPro;
using Info;
using Define;
using UnityEngine;
using UnityEngine.UI;

public class UIInventoryPopup : UIPopup
{
    enum GameObjects
    {
        Stats
    }

    enum TextMeshProUGUIs
    {
        CharacterNameText,
        CharacterLevelText,
        EXPText,
        GemText,
        GoldText,
    }

    enum Sliders
    {
        EXPSlider
    }

    public override void Init()
    {
        base.Init();

        Bind<GameObject>(typeof(GameObjects));
        Bind<Slider>(typeof(Sliders));
        Bind<TextMeshProUGUI>(typeof(TextMeshProUGUIs));

        gameObject.SetActive(false);
    }

    public void Refresh()
    {
        UpdateMoney(Managers.Account.Money);
        UpdateCharacterLevel(Managers.Object.MyPlayer.Level);
        UpdateEXPBar((float)Managers.Object.MyPlayer.EXP / Managers.Object.MyPlayer.MaxEXP);
    }

    public void UpdateMoney(MoneyT money)
    {
        this.GetTextMesh((int)TextMeshProUGUIs.GemText).text = money.Value[(int)MoneyType.GEM].ToString();
        this.GetTextMesh((int)TextMeshProUGUIs.GoldText).text = money.Value[(int)MoneyType.GOLD].ToString();
    }

    public void UpdateCharacterName(string name)
    {
        this.GetTextMesh((int)TextMeshProUGUIs.CharacterNameText).text = name;
    }

    public void UpdateCharacterLevel(byte level)
    {
        this.GetTextMesh((int)TextMeshProUGUIs.CharacterLevelText).text = level.ToString();
    }

    public void UpdateEXPBar(float value)
    {
       this.GetTextMesh((int)TextMeshProUGUIs.EXPText).text = value + "%";
       GetSlider((int)Sliders.EXPSlider).value = value;
    }
}
