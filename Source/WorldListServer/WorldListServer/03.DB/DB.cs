using System;
using System.IO;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlClient;

// 나중에 redis로 바꾸면 좋지 않을까?
namespace WorldListServer
{
	public class DB
	{
        private static string info = "Data Source=127.0.0.1;Initial Catalog={0};Integrated Security=True;";

        public DB()
        {
            StreamReader sr = new StreamReader("DBConfig.csv");
            if (!sr.EndOfStream)
                sr.ReadLine();

            if (!sr.EndOfStream)
            {
                string line = sr.ReadLine();
                string[] data = line.Split(',');

                info = string.Format(info, data[0]);
                Console.WriteLine(info);
            }
        }

        public List<WorldListInfo> GetWorldList()
        {
            List<WorldListInfo> worldInfos = new List<WorldListInfo>();

            using (SqlConnection connection = new SqlConnection(info))
            {
                connection.Open();

                using (SqlCommand cmd = new SqlCommand("dbo.GetWorldList", connection))
                {
                    cmd.CommandType = CommandType.StoredProcedure;

                    using (SqlDataReader rdr = cmd.ExecuteReader())
                    {
                        while (rdr.Read())
                        {
                            WorldListInfo worldInfo = new WorldListInfo();
                            worldInfo.Name = rdr["WorldName"].ToString();
                            worldInfo.BusyScore = Convert.ToInt32(rdr["BusyScore"].ToString());

                            worldInfos.Add(worldInfo);
                        }
                    }
                }

                connection.Close();
                connection.Dispose();
            }

            return worldInfos;
        }
    }
}
