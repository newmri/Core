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
        private static string info = "Data Source={0}; Initial Catalog={1}; User ID=newmri; Password=!a123123123";

        public DB()
        {
            StreamReader sr = new StreamReader("DBConfig.csv");
            if(!sr.EndOfStream)
                sr.ReadLine();

            if(!sr.EndOfStream)
            {
                string line = sr.ReadLine();
                string[] data = line.Split(',');

                info = string.Format(info, data[1], data[0]);
                Console.WriteLine("IP: {0}, DB: {1}", data[1], data[0]);
            }
        }

        public List<WorldListInfo> GetWorldList()
        {
            List<WorldListInfo> worldInfos = new List<WorldListInfo>();

            using (SqlConnection connection = new SqlConnection(info))
            {
                connection.Open();
                using (SqlCommand cmd = new SqlCommand("GetWorldList", connection))
                {
                    Console.WriteLine("aa");

                    cmd.CommandType = CommandType.StoredProcedure;

                    using (SqlDataReader rdr = cmd.ExecuteReader())
                    {
                        while (rdr.Read())
                        {
                            WorldListInfo worldInfo = new WorldListInfo();
                            worldInfo.Name = rdr["Name"].ToString();
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
