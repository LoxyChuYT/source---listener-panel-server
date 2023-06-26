using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using MySql.Data.MySqlClient;
using System.IO;

namespace Server{
    class SQLConnection
    {
        public static string
        serverAddr,
        database,
        username,
        passwd;

        public static void iniLogin()
        {
            try
            {
                serverAddr = "localhost";
                database = "outherelive";//rename to use diff databases
                username = "root";
                passwd = "";

            }
            catch (Exception)
            { 

            }
       
        }


        public static MySqlConnection iniHandle(){
            return new MySqlConnection(String.Format("Server={0};Database={1};Uid={2};Pwd={3};", serverAddr, database, username, passwd));
        }

        public static bool open(MySqlConnection con)
        {
            try
            {
                con.Open();
                return true;
            }
            catch (MySqlException ex)
            {
                Server.write(ConsoleColor.Red,string.Format("The server is restarting in two seconds due to [SQLCONNECTION.CS] {0}\n", ex.Message));
                Thread.Sleep(3000);
                //Server.restart();
                return false;
            }
        }
    }
}
