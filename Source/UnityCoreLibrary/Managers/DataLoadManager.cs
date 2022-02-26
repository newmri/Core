using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace UnityCoreLibrary
{
    public class DataLoadManager
    {
        CSVLoader _CSVLoader = new CSVLoader();

        public List<Dictionary<string, object>> LoadCSV(string path)
        {
            return _CSVLoader.Load(path);
        }

    }
}
