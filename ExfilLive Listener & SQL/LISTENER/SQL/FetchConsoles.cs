using System;
using System.Net;
using MySql.Data.MySqlClient;


namespace Server
{
    public struct cData
    {
        public string cpukey;
        public string salt;
        public string ip;
        public bool enabled;
        public bool banned;
        public string name;
        public DateTime expireTime;
        public DateTime kvTime;
        public DateTime lastonline;
        public string token;
        public string kvshash;
        public int kvbanned;
    } 
    
    class FetchConsoles{

        public static string FirstCharToUpper(string value)
        {
            char[] array = value.ToCharArray();
            if (array.Length >= 1)
            {
                if (char.IsLower(array[0]))
                {
                    array[0] = char.ToUpper(array[0]);
                }
            }
            for (int i = 1; i < array.Length; i++)
            {
                if (array[i - 1] == ' ')
                {
                    if (char.IsLower(array[i]))
                    {
                        array[i] = char.ToUpper(array[i]);
                    }
                }
            }
            return new string(array);
        }

                       

        private long ip2long(IPAddress ip){
                return (((long)ip.GetAddressBytes()[0] << 24) | ((uint)ip.GetAddressBytes()[1] << 16) | ((uint)ip.GetAddressBytes()[2] << 8) | ip.GetAddressBytes()[3]);
        }


        public void cron_timeUpdate()
        {
            using (var con = SQLConnection.iniHandle())
            using (var cmd = con.CreateCommand())
            {
                if (!SQLConnection.open(con)) return;
                {
                    if (settings.privateServer)
                    {
                        cmd.CommandText = "UPDATE `private_" + settings.SERVERNAME.ToLower() + "_consoles` SET `enabled`=true WHERE now() >= `expire`";
                    }
                    else
                    {
                        cmd.CommandText = "UPDATE `consoles` SET `enabled`=true WHERE now() >= `expire`";
                    }
                    cmd.ExecuteNonQuery();
                    con.Close();
                }
            }
        }

        public void failConsole(string cpu, string ip){
            using (var con = SQLConnection.iniHandle())
            using (var cmd = con.CreateCommand()){
                if (!SQLConnection.open(con)) return;
                if (settings.privateServer)
                {
                    cmd.CommandText = "insert into private_" + settings.SERVERNAME.ToLower() + "_failed (cpukey, ip, num) values (@cpukey, @ip, 0)";
                }
                else
                {
                    cmd.CommandText = "insert into failed (cpukey, ip, num) values (@cpukey, @ip, 0)";
                }
                cmd.Parameters.AddWithValue("@cpukey", cpu);
                cmd.Parameters.AddWithValue("@ip", ip);
                try
                {
                    cmd.ExecuteNonQuery();
                }
                catch (MySqlException)
                {
                    using (var newCmd = con.CreateCommand())
                    {
                        if (settings.privateServer)
                        {
                            newCmd.CommandText = "UPDATE `private_" + settings.SERVERNAME.ToLower() + "_failed` SET `ip` = @ip WHERE `cpukey` = @cpukey";

                        }
                        else
                        {
                            newCmd.CommandText = "UPDATE `failed` SET `ip` = @ip WHERE `cpukey` = @cpukey";
                        }
                        newCmd.Parameters.AddWithValue("@cpukey", cpu);
                        newCmd.Parameters.AddWithValue("@ip", ip);
                    }
                }
                con.Close();
            }
        }

        public void guestConsole(string cpu)
        {
            using (MySqlConnection mySqlConnection = SQLConnection.iniHandle())
            {
                using (MySqlCommand mySqlCommand = mySqlConnection.CreateCommand())
                {
                    if (SQLConnection.open(mySqlConnection))
                    {
                        mySqlCommand.CommandText = "insert into consoles (name, cpukey, expire, email, enabled) values (@Name, @cpukey, @expire, @email, @enabled)";
                        mySqlCommand.Parameters.AddWithValue("@Name", "New Client");
                        mySqlCommand.Parameters.AddWithValue("@cpukey", cpu);
                        mySqlCommand.Parameters.AddWithValue("@email", "Client@DominatorLive.com");
                        mySqlCommand.Parameters.AddWithValue("@expire", DateTime.Now.AddDays(7));
                        mySqlCommand.Parameters.AddWithValue("@enabled", 1);
                        mySqlCommand.ExecuteNonQuery();
                        mySqlConnection.Close();
                    }
                }
            }
        }


        public void kvreset(ref cData data, string hash)
        {
            using (MySqlConnection mySqlConnection = SQLConnection.iniHandle())
            {
                using (MySqlCommand mySqlCommand = mySqlConnection.CreateCommand())
                {
                    if (SQLConnection.open(mySqlConnection))
                    {
                        mySqlCommand.CommandText = "update consoles set kvtime = @kvtime, kvhash = @kvhash where cpukey = @cpukey";
                        mySqlCommand.Parameters.AddWithValue("@cpukey", data.cpukey);
                        mySqlCommand.Parameters.AddWithValue("@kvhash", hash);
                        mySqlCommand.Parameters.AddWithValue("@kvtime", DateTime.Now);
                        mySqlCommand.ExecuteNonQuery();
                        mySqlConnection.Close();
                    }
                }
            }
        }


        public void expiredConsole(string cpu)
        {
            using (MySqlConnection mySqlConnection = SQLConnection.iniHandle())
            {
                using (MySqlCommand mySqlCommand = mySqlConnection.CreateCommand())
                {
                    if (SQLConnection.open(mySqlConnection))
                    {
                        mySqlCommand.CommandText = "update consoles set expire = @expire where cpukey = @cpukey";
                        mySqlCommand.Parameters.AddWithValue("@cpukey", cpu);
                        mySqlCommand.Parameters.AddWithValue("@expire", DateTime.Now.AddDays(0));
                        mySqlCommand.ExecuteNonQuery();
                        mySqlConnection.Close();
                    }
                }
            }
        }

        public void updatetime(ref cData data)
        {
            TimeSpan span = (DateTime.Now - data.lastonline);
            using (MySqlConnection mySqlConnection = SQLConnection.iniHandle())
            {
                using (MySqlCommand mySqlCommand = mySqlConnection.CreateCommand())
                {
                    if (SQLConnection.open(mySqlConnection))
                    {
                        mySqlCommand.CommandText = "update consoles set expire = @expire where cpukey = @cpukey";
                        mySqlCommand.Parameters.AddWithValue("@cpukey", data.cpukey);
                        mySqlCommand.Parameters.AddWithValue("@expire", data.expireTime.Add(span));
                        mySqlCommand.ExecuteNonQuery();
                        mySqlConnection.Close();
                    }
                }
            }
        }

        public void bannedtime(ref cData data, int banned)
        {
            using (MySqlConnection mySqlConnection = SQLConnection.iniHandle())
            {
                using (MySqlCommand mySqlCommand = mySqlConnection.CreateCommand())
                {
                    if (SQLConnection.open(mySqlConnection))
                    {
                        mySqlCommand.CommandText = "update consoles set kvtime = @kvtime, kvbanned = @kvbanned where cpukey = @cpukey";
                        mySqlCommand.Parameters.AddWithValue("@cpukey", data.cpukey);
                        mySqlCommand.Parameters.AddWithValue("@kvbanned", banned);
                        if (banned == 1)
                        {
                            mySqlCommand.Parameters.AddWithValue("@kvtime", data.kvTime.AddSeconds(30));
                        }
                        if (banned == 0)
                        {
                            mySqlCommand.Parameters.AddWithValue("@kvtime", data.kvTime);
                        }
                        mySqlCommand.ExecuteNonQuery();
                        mySqlConnection.Close();
                    }
                }
            }
        }

        public bool getConsole(ref cData data, string cpukey)
        {
            using (var con = SQLConnection.iniHandle())
            using (var cmd = con.CreateCommand())
            {
                if (!SQLConnection.open(con)) return false;
                cmd.CommandText = "select * from consoles where cpukey=@cpukey";
                cmd.Parameters.AddWithValue("@cpukey", cpukey);
                using (var rdr = cmd.ExecuteReader())
                    if (rdr.Read())
                    {
                        data.name = (string)rdr["name"];
                        data.cpukey = (string)rdr["cpukey"];
                        data.enabled = (bool)rdr["enabled"];
                        data.kvbanned = (int)rdr["kvbanned"];
                        data.expireTime = (DateTime)rdr["expire"];
                        data.kvshash = (string)rdr["kvhash"];
                        data.kvTime = (DateTime)rdr["kvtime"];
                        data.lastonline = (DateTime)rdr["lastonline"];
                        data.salt = !rdr.IsDBNull(rdr.GetOrdinal("salt")) ? data.salt = (string)rdr["salt"] : data.salt = "";
                        con.Close();
                        return true;
                    }
                con.Close();
            }
            return false;
        }

        public bool getConsoleBySession(ref cData data, string seshHash){
            using (var con = SQLConnection.iniHandle())
            using (var cmd = con.CreateCommand())
            {
                if (!SQLConnection.open(con)) return false;
                cmd.CommandText = "select * from consoles where salt=@Session";
                cmd.Parameters.AddWithValue("@Session", seshHash);
                using (var rdr = cmd.ExecuteReader())
                    if (rdr.Read())
                    {
                        data.name = (string)rdr["name"];
                        data.cpukey = (string)rdr["cpukey"];
                        data.enabled = (bool)rdr["enabled"];
                        data.expireTime = (DateTime)rdr["expire"];
                        data.kvTime = (DateTime)rdr["kvtime"];
                        data.kvbanned = (int)rdr["kvbanned"];
                        data.salt = seshHash;
                        data.kvshash = (string)rdr["kvhash"];
                        data.lastonline = (DateTime)rdr["lastonline"];
                        return true;
                    }
            }
            return false;
        }
       
        public int getTokenType(string Token)
        {
            using (var con = SQLConnection.iniHandle())
            using (var cmd = con.CreateCommand())
            {
                if (!SQLConnection.open(con)) return 9;
                cmd.CommandText = "select * from tokens where token=@token";
                cmd.Parameters.AddWithValue("@token", Token);
                using (var rdr = cmd.ExecuteReader())
                {
                    if (rdr.Read())
                    {
                        int time = (int)rdr["time"];
                        int Used = (int)rdr["used"];
                        Server.write(ConsoleColor.Red, string.Format("Token Checking - Time: {0}, Used: {1} |\n", time, Used));
                        if (Used != 0)
                        {

                            con.Close();
                            return 9;
                        }
                        if (time == 1)
                        {
                            con.Close();
                            return 0;
                        }
                        else if (time == 3)
                        {
                            con.Close();
                            return 1;
                        }
                        else if (time == 7)
                        {
                            con.Close();
                            return 2;
                        }
                        else if (time == 14)
                        {
                            con.Close();
                            return 3;
                        }
                        else if (time == 31)
                        {
                            con.Close();
                            return 4;
                        }
                        else if (time == 93)
                        {
                            con.Close();
                            return 5;
                        }
                        else if (time == 186)
                        {
                            con.Close();
                            return 6;
                        }
                        else if (time == 365)
                        {
                            con.Close();
                            return 7;
                        }
                        else if (time == 99999)
                        {
                            con.Close();
                            return 8;
                        }
                        else {
                            con.Close();
                            return 9;
                        }
                    }
                    con.Close();
                    return 9;
                }
            }
        }

        public bool DeleteToken(string Token)
        {
            using (var con = SQLConnection.iniHandle())
            using (var cmd = con.CreateCommand())
            {
                if (!SQLConnection.open(con)) return false;
                cmd.CommandText = "delete from tokens where token=@token";
                cmd.Parameters.AddWithValue("@token", Token);
                cmd.ExecuteNonQuery();
                con.Close();
                return true;
            }
        }

        public bool UseToken(string Token, string cpukey)
        {
            using (var con = SQLConnection.iniHandle())
            using (var cmd = con.CreateCommand())
            {
                if (!SQLConnection.open(con)) return false;
                cmd.CommandText = "update tokens set used=1, usedby=@UsedBy where token=@token";
                cmd.Parameters.AddWithValue("@token", Token);
                cmd.Parameters.AddWithValue("@UsedBy", cpukey);
                cmd.ExecuteNonQuery();
                con.Close();
                return true;
            }
        }
        public void SaveConsole(ref cData c)
        {
            using (var con = SQLConnection.iniHandle())
            using (var cmd = con.CreateCommand()){
                if (!SQLConnection.open(con)) return;
                cmd.CommandText = "update consoles set salt=@salt, enabled=@enabled, ip=@ip, lastonline=@last where cpukey=@cpukey";
                cmd.Parameters.AddWithValue("@cpukey", c.cpukey);
                cmd.Parameters.AddWithValue("@salt", c.salt);
                int num = c.enabled ? 1 : 0;
                cmd.Parameters.AddWithValue("@enabled", num);
                cmd.Parameters.AddWithValue("@ip", c.ip);
                cmd.Parameters.AddWithValue("@last", DateTime.Now);
                cmd.ExecuteNonQuery();
                con.Close();
            }
        }
        public void SaveTime(string cpukey, DateTime dt)
        {
            using (var con = SQLConnection.iniHandle())
            using (var cmd = con.CreateCommand())
            {
                if (!SQLConnection.open(con)) return;
                cmd.CommandText = "update consoles set expire=@expire where cpukey=@cpukey";
                cmd.Parameters.AddWithValue("@cpukey", cpukey);
                cmd.Parameters.AddWithValue("@expire", dt);
                cmd.ExecuteNonQuery();
                con.Close();
                

            }
        }

    }
}