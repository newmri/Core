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
    enum Sliders
    {
        HPSlider,
        MPSlider
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
        AttackButton
    }

    public override void Init()
    {
        Managers.UI.SetCanvas(gameObject);

        Bind<Slider>(typeof(Sliders));
        Bind<TextMeshProUGUI>(typeof(TextMeshProUGUIs));
        Bind<Image>(typeof(Images));
        Bind<Button>(typeof(Buttons));

        GetButton((int)Buttons.AttackButton).gameObject.BindEvent(OnClickAttackButton);
    }

    public void UpdateHPBar(int HP, int MaxHP)
    {
        GetSlider((int)Sliders.HPSlider).value = HP / MaxHP;
    }

    public void UpdateMPBar(int MP, int MaxMP)
    {
        GetSlider((int)Sliders.MPSlider).value = MP / MaxMP;
    }

    public void UpdateCharacterInfo(string name, byte level, Define.Job job)
    {
        this.GetTextMesh((int)TextMeshProUGUIs.CharacterNameText).text = name;
        this.GetTextMesh((int)TextMeshProUGUIs.CharacterLevelText).text = "Lv." + level.ToString();

        Sprite jobIcon = CoreManagers.Resource.Load<Sprite>($"UI/Job/{job.ToString()}Icon");
        GetImage((int)Images.JobIconImage).sprite = jobIcon;
    }

    public void UpdateMoney(MoneyT money)
    {
        this.GetTextMesh((int)TextMeshProUGUIs.GemText).text = money.Value[(int)MoneyType.GEM].ToString();
        this.GetTextMesh((int)TextMeshProUGUIs.GoldText).text = money.Value[(int)MoneyType.GOLD].ToString();
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
        Managers.Creature.MyPlayer.UseSkill((int)Managers.Creature.MyPlayer.MyCharacterInfo.Job);
    }
}
