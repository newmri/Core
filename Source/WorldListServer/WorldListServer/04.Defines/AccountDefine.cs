public static class AccountDefine
{
    public static int MinIDLen = 2;
    public static int MaxIDLen = 10;
    public static int MinPasswordLen = 4;
    public static int MaxPasswordLen = 15;

    public static bool IsValidAccount(string id, string password)
    {
       return (id.Length >= MinIDLen && id.Length <= MaxIDLen &&
               password.Length >= MinPasswordLen && password.Length <= MaxPasswordLen);
    }
}