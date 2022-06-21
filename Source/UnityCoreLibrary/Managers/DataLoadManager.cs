using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace UnityCoreLibrary
{
    public class DataLoadManager
    {
        CSVLoader _CSVLoader = new CSVLoader();

        public void LoadCSV(string path, out List<Dictionary<string, object>> dataList)
        {
            _CSVLoader.Load(path, out dataList);
        }

        public void LoadCSV(string key, string path, out Dictionary<int, Dictionary<string, object>> dataList)
        {
            _CSVLoader.Load(key, path, out dataList);
        }
    }
}
