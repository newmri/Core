using System;
using System.Globalization;

namespace UnityCoreLibrary
{
    public class TimeUtils
    {
        public static DateTime GetNow()
        {
            return DateTime.Now;
        }

        public static DateTime GetDateTime(string date)
        {
            return DateTime.ParseExact(date, _dateFormat, CultureInfo.InvariantCulture);
        }

        public static string GetCurrentDate()
        {
            return DateTime.Now.ToString(_dateFormat);
        }

        public static string GetTimeDiff(string start, string end)
        {
            return (GetDateTime(start) - GetDateTime(end)).ToString();
        }

        public static string GetYear()
        {
            return DateTime.Now.ToString(("yyyy"));
        }

        public static double GetCurrentMilliSeconds()
        {
            TimeSpan time = (DateTime.UtcNow - new DateTime(1970, 1, 1));
            return time.TotalMilliseconds;
        }

        public static string GetMilliSecondsToDate(double expiredTime)
        {
            var dateTime = new DateTime(1970, 1, 1, 0, 0, 0, 0, DateTimeKind.Utc);
            dateTime = dateTime.AddMilliseconds((double)expiredTime).ToLocalTime();
            return dateTime.ToString();
        }

        private static string _dateFormat = "yyyy-MM-dd HH:mm:ss tt";
    }
}
