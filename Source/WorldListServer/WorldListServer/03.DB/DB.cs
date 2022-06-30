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
        private static string _info = "Data Source=127.0.0.1;Initial Catalog=World;Integrated Security=True;";

        public bool SignupAccount(SignupAccountPacketReq req)
        {
            if (!AccountDefine.IsValidAccount(req.ID, req.Password))
                return false;

            bool IsSuccess = false;

            using (SqlConnection connection = new SqlConnection(_info))
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

        public LoginAccountPacketRes LoginAccount(LoginAccountPacketReq req)
        {
            LoginAccountPacketRes res = new LoginAccountPacketRes();
            if (!AccountDefine.IsValidAccount(req.ID, req.Password))
                return res;

            if (!LoginAccount(req, ref res))
                return res;

            UpdateAccountToken(req, ref res);

            res.WorldList = GetWorldList();

            return res;
        }

        private bool LoginAccount(LoginAccountPacketReq req, ref LoginAccountPacketRes res)
        {
            using (SqlConnection connection = new SqlConnection(_info))
            {
                connection.Open();

                using (SqlCommand cmd = new SqlCommand("dbo.LoginAccount", connection))
                {
                    cmd.CommandType = CommandType.StoredProcedure;

                    cmd.Parameters.AddWithValue("@IN_ID", req.ID);
                    cmd.Parameters.AddWithValue("@IN_Password", req.Password);

                    using (SqlDataReader rdr = cmd.ExecuteReader())
                    {
                        while (rdr.Read())
                        {
                            res.IsSuccess = Convert.ToBoolean(rdr["IsSuccess"].ToString());
                        }
                    }
                }

                connection.Close();
                connection.Dispose();
            }

            return res.IsSuccess;
        }

        private void UpdateAccountToken(LoginAccountPacketReq req, ref LoginAccountPacketRes res)
        {
            using (SqlConnection connection = new SqlConnection(_info))
            {
                connection.Open();

                using (SqlCommand cmd = new SqlCommand("dbo.UpdateAccountToken", connection))
                {
                    cmd.CommandType = CommandType.StoredProcedure;

                    cmd.Parameters.AddWithValue("@IN_ID", req.ID);

                    using (SqlDataReader rdr = cmd.ExecuteReader())
                    {
                        while (rdr.Read())
                        {
                            res.UID = Convert.ToInt64(rdr["UID"].ToString());
                            res.Token = Convert.ToInt32(rdr["Token"].ToString());
                        }
                    }
                }

                connection.Close();
                connection.Dispose();
            }
        }

        static long ConvertToUnixTimestamp(DateTime date)
        {
            DateTime origin = new DateTime(1970, 1, 1, 0, 0, 0, 0);
            TimeSpan diff = date - origin;
            return (long)Math.Floor(diff.TotalSeconds);
        }

        public List<WorldListInfo> GetWorldList()
        {
            List<WorldListInfo> worldList = new List<WorldListInfo>();

            using (SqlConnection connection = new SqlConnection(_info))
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
                            worldInfo.ID = Convert.ToByte(rdr["WorldID"].ToString());
                            worldInfo.Name = rdr["WorldName"].ToString();
                            worldInfo.BusyScore = Convert.ToInt32(rdr["BusyScore"].ToString());

                            worldList.Add(worldInfo);
                        }
                    }
                }

                connection.Close();
                connection.Dispose();
            }

            return worldList;
        }

        public LoginServerInfoPacketRes GetMinServerInfo(LoginServerInfoPacketReq req)
        {
            LoginServerInfoPacketRes serverInfo = new LoginServerInfoPacketRes();

            if (req.ServerType <= ServerType.WorldList || req.ServerType >= ServerType.ServerTypeEnd)
                return serverInfo;

            using (SqlConnection connection = new SqlConnection(_info))
            {
                connection.Open();

                using (SqlCommand cmd = new SqlCommand("dbo.GetMinServerInfo", connection))
                {
                    cmd.CommandType = CommandType.StoredProcedure;

                    cmd.Parameters.AddWithValue("@IN_WorldID", req.WorldID);
                    cmd.Parameters.AddWithValue("@IN_ServerName", req.ServerType.ToString());

                    using (SqlDataReader rdr = cmd.ExecuteReader())
                    {
                        while (rdr.Read())
                        {
                            serverInfo.ServerIP = rdr["ServerIP"].ToString();
                            serverInfo.ServerPort = Convert.ToInt32(rdr["ServerPort"].ToString());
                        }
                    }
                }

                connection.Close();
                connection.Dispose();
            }

            return serverInfo;
        }
    }
}
