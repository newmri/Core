using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public static class AccountDefine
{
    public static int MinIDLen = 2;
    public static int MaxIDLen = 10;
    public static int MinPasswordLen = 4;
    public static int MaxPasswordLen = 15;

    public static bool IsValidAccount(string id, string password)
    {
        bool isValid = (id.Length >= MinIDLen && id.Length <= MaxIDLen &&
                      password.Length >= MinPasswordLen && password.Length <= MaxPasswordLen);


        if (!isValid)
        {
            UIMessagePopup messagePopup = Managers.UI.ShowPopupUI<UIMessagePopup>();
            string idMessage = "아이디는 {0} ~ {1}자 사이로 입력 해주세요 \n";
            string paswordMessage = "비밀번호는 {0} ~ {1}자 사이로 입력 해주세요 \n";
            string message;
            message = string.Format(idMessage, AccountDefine.MinIDLen, AccountDefine.MaxIDLen);
            message += string.Format(paswordMessage, AccountDefine.MinPasswordLen, AccountDefine.MaxPasswordLen);

            messagePopup.SetText("회원가입 실패", message);
        }

        return isValid;
    }
}
