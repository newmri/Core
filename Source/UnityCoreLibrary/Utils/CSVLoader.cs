using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using UnityEngine;

namespace UnityCoreLibrary
{
    public class CSVLoader
    {
        string _split = @",(?=(?:[^""]*""[^""]*"")*(?![^""]*""))";
        string _lineSplit = @"\r\n|\n\r|\n|\r";
        char[] _trim = { '\"' };

        public List<Dictionary<string, object>> Load(string path)
        {
            var list = new List<Dictionary<string, object>>();
            TextAsset data = CoreManagers.Resource.Load<TextAsset>(path);

            var lines = Regex.Split(data.text, _lineSplit);

            if (lines.Length <= 1) return list;

            var header = Regex.Split(lines[0], _split);
            for (var i = 1; i < lines.Length; i++)
            {
                var values = Regex.Split(lines[i], _split);
                if (values.Length == 0 || values[0] == "")
                    continue;

                var entry = new Dictionary<string, object>();
                for (var j = 0; j < header.Length && j < values.Length; j++)
                {
                    string value = values[j];
                    //value = value.TrimStart(_trim).TrimEnd(_trim).Replace("\\", "");
                    object finalvalue = value;
                    int n;
                    float f;
                    if (int.TryParse(value, out n))
                    {
                        finalvalue = n;
                    }
                    else if (float.TryParse(value, out f))
                    {
                        finalvalue = f;
                    }
                    entry[header[j]] = finalvalue;
                }
                list.Add(entry);
            }
            return list;
        }
    }
}
