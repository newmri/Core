using System.Collections;
using System.Collections.Generic;
using UnityCoreLibrary;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;
using Define;
using GamePacket;
using Info;

public class UIGameScene : UIScene
{
    enum GameObjects
    {
        Chat
    }

    enum Sliders
    {
        HPSlider,
        MPSlider,
        EXPSlider,
    }

    enum TextMeshProUGUIs
    {
        CharacterLevelText,
        CharacterNameText,
        GemText,
        GoldText
    }

    enum Images
    {
        JobIconImage,
        AttackCoolTimeImage
    }

    enum Buttons
    {
        AttackButton,
        SettingButton
    }

    public UIItemInventoryPopup Inventory;
    public UIChat Chat;

    public override void Init()
    {
        Managers.UI.SetCanvas(gameObject);

        Bind<GameObject>(typeof(GameObjects));
        Bind<Slider>(typeof(Sliders));
        Bind<TextMeshProUGUI>(typeof(TextMeshProUGUIs));
        Bind<Image>(typeof(Images));
        Bind<Button>(typeof(Buttons));

        GetButton((int)Buttons.AttackButton).gameObject.BindEvent(OnClickAttackButton);
        GetButton((int)Buttons.SettingButton).gameObject.BindEvent(OnClickSettingButton);

        Inventory = Managers.UI.ShowPopupUI<UIItemInventoryPopup>();
        Chat = GetObject((int)GameObjects.Chat).GetComponent<UIChat>();
    }

    private void Update()
    {
        Chat.OnUpdate();
    }

    public void UpdateHPBar(int HP, int MaxHP)
    {
        GetSlider((int)Sliders.HPSlider).value = (float)HP / MaxHP;
    }

    public void UpdateMPBar(int MP, int MaxMP)
    {
        GetSlider((int)Sliders.MPSlider).value = (float)MP / MaxMP;
    }

    public void UpdateEXPBar(long EXP, long MaxEXP)
    {
        float value = (float)EXP / MaxEXP;
        GetSlider((int)Sliders.EXPSlider).value = value;

        if (Inventory.gameObject.activeSelf)
            Inventory.UpdateEXPBar(value);
    }

    public void UpdateCharacterInfo(string name, byte level, Job job)
    {
        this.GetTextMesh((int)TextMeshProUGUIs.CharacterNameText).text = name;
        this.GetTextMesh((int)TextMeshProUGUIs.CharacterLevelText).text = "Lv." + level.ToString();

        Sprite jobIcon = CoreManagers.Resource.Load<Sprite>($"UI/Job/{job.ToString()}Icon");
        GetImage((int)Images.JobIconImage).sprite = jobIcon;

        Inventory.UpdateCharacterName(name);
    }

    public void UpdateCharacterLevel(byte level)
    {
        this.GetTextMesh((int)TextMeshProUGUIs.CharacterLevelText).text = "Lv." + level.ToString();
    }

    public void UpdateMoney(MoneyT money)
    {
        this.GetTextMesh((int)TextMeshProUGUIs.GemText).text = money.Value[(int)MoneyType.GEM].ToString();
        this.GetTextMesh((int)TextMeshProUGUIs.GoldText).text = money.Value[(int)MoneyType.GOLD].ToString();

        if(Inventory.gameObject.activeSelf)
            Inventory.UpdateMoney(money);
    }

    Dictionary<int, Images> _skillCoolTimeList = new Dictionary<int, Images>();

    public void AddSkill(int skillID)
    {
        _skillCoolTimeList.Add(skillID, Images.AttackCoolTimeImage);
    }

    public void UpdateSkillCoolTime(int skillID, float ratio)
    {
        Images image;

        ratio = 1.0f - ratio;

        ratio = ratio == 1.0f ? 0.0f : ratio;

        if (_skillCoolTimeList.TryGetValue(skillID, out image))
        {
            GetImage((int)image).fillAmount = ratio;
        }
    }

    public void OnClickAttackButton(PointerEventData evt)
    {
        Managers.Object.MyPlayer.UseSkill((int)Managers.Object.MyPlayer.MyCharacterInfo.Job);
    }

    public void OnClickSettingButton(PointerEventData evt)
    {
        Managers.UI.ShowPopupUI<UISettingPopup>();
    }
}
