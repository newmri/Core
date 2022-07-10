using Define;
using FlatBuffers;
using GamePacket;
using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityCoreLibrary;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class UIChat : UIBase
{
    enum GameObjects
    {
        ChatContent,
        ChatList
    }

    enum Buttons
    {
        ChatTypeButton
    }

    enum TextMeshProUGUIs
    {
        ChatTypeText
    }

    enum InputTexts
    {
        ChatInputText
    }

    [SerializeField]
    private Sprite[] _chatInputTypeSprite;

    [SerializeField]
    private string[] _chatInputTypeName;

    [SerializeField]
    private Color[] _textColorList;

    private ChatType _currInputType;
    private Color _currTextColor;

    public override void Init()
    {
        Bind<GameObject>(typeof(GameObjects));
        Bind<Button>(typeof(Buttons));
        Bind<TextMeshProUGUI>(typeof(TextMeshProUGUIs));
        Bind<TMP_InputField>(typeof(InputTexts));

        GetButton((int)Buttons.ChatTypeButton).gameObject.BindEvent(OnClickChatTypeButton);

        gameObject.SetActive(false);
        _currInputType = ChatType.NORMAL;
        _currTextColor = Color.black;
    }

    public void OnUpdate()
    {
        GetUIKeyInput();
    }

    public bool IsFocused()
    {
        return this.GetInputText((int)InputTexts.ChatInputText).isFocused;
    }

    private void GetUIKeyInput()
    {
        if (Input.GetKeyDown(KeyCode.Return))
            UpdateChatText();
        else if (Input.GetKeyDown(KeyCode.Tab) && IsFocused())
            UpdateChatInputType();
    }

    private void UpdateChatText()
    {
        if (gameObject.activeSelf)
        {
            if (this.GetInputText((int)InputTexts.ChatInputText).text.Length > 0)
            {
                FlatBufferBuilder builder = new FlatBufferBuilder(1);
                StringOffset chatMessage = builder.CreateString(this.GetInputText((int)InputTexts.ChatInputText).text);
                var message = CS_NORMAL_CHAT_REQ.CreateCS_NORMAL_CHAT_REQ(builder, _currInputType, chatMessage);
                Managers.GameNetwork.Send(builder, Packet.CS_NORMAL_CHAT_REQ, message.Value);

                this.GetInputText((int)InputTexts.ChatInputText).text = "";
            }

            gameObject.SetActive(false);
        }
        else
        {
            gameObject.SetActive(true);
            this.GetInputText((int)InputTexts.ChatInputText).ActivateInputField();
            GetObject((int)GameObjects.ChatList).GetComponent<ScrollRect>().verticalNormalizedPosition = -1;
        }
    }

    private void UpdateChatInputType()
    {
        _currInputType = (int)_currInputType < (int)ChatType.END - 3 ? _currInputType + 1 : 0;
        OnChangedChatInputType();
    }

    private void OnChangedChatInputType()
    {
        GetButton((int)Buttons.ChatTypeButton).GetComponent<Image>().sprite = _chatInputTypeSprite[(int)_currInputType];
        this.GetTextMesh((int)TextMeshProUGUIs.ChatTypeText).text = _chatInputTypeName[(int)_currInputType];

        _currTextColor = _textColorList[(int)_currInputType];
    }

    private void OnClickChatTypeButton(PointerEventData evt)
    {
        UpdateChatInputType();
    }

    public void AddMessage(long oid, ChatType chatType, string message)
    {
        GameObject go = CoreManagers.Resource.Instantiate($"UI/UIChatText", GetObject((int)GameObjects.ChatContent).transform, 20);
        UIChatData chatData = go.GetComponent<UIChatData>();
        chatData.SetChatData(chatType, _textColorList[(int)chatType], Managers.Object.GetCharacterName(oid) + ": " + message);
    }
}
