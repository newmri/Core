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
        private static string info = "Data Source=127.0.0.1;Initial Catalog=World;Integrated Security=True;";

        public bool SignupAccount(SignupAccountPacketReq req)
        {
            if (!AccountDefine.IsValidAccount(req.ID, req.Password))
                return false;

            bool IsSuccess = false;

            using (SqlConnection connection = new SqlConnection(info))
            {
                connection.Open();

                using (SqlCommand cmd = new SqlCommand("dbo.SignupAccount", connection))
                {
                    cmd.CommandType = CommandType.StoredProcedure;

                    cmd.Parameters.AddWithValue("@IN_ID", req.ID);
                    cmd.Parameters.AddWithValue("@IN_Password", req.Password);

                    using (SqlDataReader rdr = cmd.ExecuteReader())
                    {
                        while (rdr.Read())
                        {
                            IsSuccess = Convert.ToBoolean(rdr["IsSuccess"].ToString());
                        }
                    }
                }

                connection.Close();
                connection.Dispose();
            }

            return IsSuccess;
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
