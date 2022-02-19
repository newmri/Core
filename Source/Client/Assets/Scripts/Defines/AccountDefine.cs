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
            string idMessage = "���̵�� {0} ~ {1}�� ���̷� �Է� ���ּ��� \n";
            string paswordMessage = "��й�ȣ�� {0} ~ {1}�� ���̷� �Է� ���ּ��� \n";
            string message;
            message = string.Format(idMessage, AccountDefine.MinIDLen, AccountDefine.MaxIDLen);
            message += string.Format(paswordMessage, AccountDefine.MinPasswordLen, AccountDefine.MaxPasswordLen);

            messagePopup.SetText("ȸ������ ����", message);
        }

        return isValid;
    }
}
