using Define;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public static class LimiDefine
{
    public static bool IsValidAccountLen(string id, string password)
    {
        bool isValid = (id.Length >= (int)AccountLimit.MinIDLen &&
                        id.Length <= (int)AccountLimit.MaxIDLen &&
                      password.Length >= (int)AccountLimit.MinPasswordLen &&
                      password.Length <= (int)AccountLimit.MaxPasswordLen);


        if (!isValid)
        {
            UIMessagePopup messagePopup = Managers.UI.ShowPopupUI<UIMessagePopup>();
            string idMessage = "아이디는 {0} ~ {1}자 사이로 입력 해주세요 \n";
            string paswordMessage = "비밀번호는 {0} ~ {1}자 사이로 입력 해주세요 \n";
            string message;
            message = string.Format(idMessage, AccountLimit.MinIDLen, AccountLimit.MaxIDLen);
            message += string.Format(paswordMessage, AccountLimit.MinPasswordLen, AccountLimit.MaxPasswordLen);

            messagePopup.SetText("회원가입 실패", message);
        }

        return isValid;
    }

    public static bool IsValidCharacterLen(string name)
    {
        bool isValid = (name.Length >= (int)CharacterLimit.MinNameLen &&
                        name.Length <= (int)CharacterLimit.MaxNameLen);

        if (!isValid)
        {
            UIMessagePopup messagePopup = Managers.UI.ShowPopupUI<UIMessagePopup>();
            string message = "캐릭터명은 {0} ~ {1}자 사이로 입력 해주세요 \n";
            message = string.Format(message, AccountLimit.MinIDLen, AccountLimit.MaxIDLen);

            messagePopup.SetText("회원가입 실패", message);
        }

        return isValid;
    }
}
