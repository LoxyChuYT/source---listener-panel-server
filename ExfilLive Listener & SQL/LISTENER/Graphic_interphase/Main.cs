using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;
using MySql.Data.MySqlClient;
using System.IO;
using System.Runtime.InteropServices;

namespace Server
{
    class Server
    {
        const Int32 SW_MINIMIZE = 6;

        [DllImport("Kernel32.dll", CallingConvention = CallingConvention.StdCall, SetLastError = true)]
        private static extern IntPtr GetConsoleWindow();

        [DllImport("User32.dll", CallingConvention = CallingConvention.StdCall, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool ShowWindow([In] IntPtr hWnd, [In] Int32 nCmdShow);


        public static byte[] xexBytes = null;
        public static byte[] HVBytes = null;
        public static byte[] iniBytes = null;
        public static byte[] XOSBytes = null;

        public static string XeXName = "bin/outherelive.xex";
        public static string hypervisor = "bin/HV.bin";
        public static string ini = "bin/outherelive.ini";
        public static string XOS = "bin/xosc.bin";


        private static bool enable_cronRestart = false;

        private static void MinimizeConsoleWindow()
        {
            IntPtr hWndConsole = GetConsoleWindow();
            ShowWindow(hWndConsole, SW_MINIMIZE);
        }

        static void Main(string[] args)
        {
            if (settings.crashRecovery)
            {
                Application.ThreadException += new ThreadExceptionEventHandler(exception_appThread);
                AppDomain.CurrentDomain.UnhandledException += new UnhandledExceptionEventHandler(exception_curdomainUnhandled);
            }

            Console.Title = "Server: waiting..";
            Console.BackgroundColor = ConsoleColor.Black;
            Console.ForegroundColor = ConsoleColor.White;
           // Console.BufferWidth = 80;
            Console.Clear();
            if (Initialize()) Server.write(ConsoleColor.DarkMagenta, "-------------------------------------------\n");
            else
            {
                Thread.Sleep(5000);
                //restart();
            }
            //MinimizeConsoleWindow();
        }


        private static bool Initialize()
        {
            Server.write(ConsoleColor.DarkMagenta, "-------------------------------------------\n");
            Server.write(ConsoleColor.DarkCyan, "OutHereLive!!\n");
            Server.write(ConsoleColor.DarkMagenta, "-------------------------------------------\n");

            /*try
            {
                conf = File.ReadAllLines("settings.ini");
                settings.SERVERNAME = conf[0];
                settings.port = int.Parse(conf[2]);
                Server.XeXName = conf[3] + ".xex";
                Server.Xnet = conf[4] + ".xex";
                Server.Xnetini = conf[5] + ".ini";
                Server.chall_resp = conf[6] + ".bin";
                Server.hypervisor = conf[7] + ".bin";
                Server.CachePatchBytes = conf[8] + ".bin";
                
                Server.write(ConsoleColor.Magenta,       "OutHereLive Start up Process... Complete!\n");
            }
            catch
            {
                Server.write(ConsoleColor.DarkYellow, "SKIPPED\n");
            }*/
            
            SQLConnection.iniLogin();
            using (var con = SQLConnection.iniHandle())
                try
                {
                    con.Open();
                }
                catch (MySqlException ex)
                {
                    Server.write(ConsoleColor.Magenta, string.Format("FAILED! #{0}\n", ex.Number));
                    write(ConsoleColor.Red, string.Format("{0}\n", ex.Message));
                    return false;
                }

            server svr = new server();
            if (!svr.checkhandle())
            {
                return false;
            }
            else
            {
                svr.start();
                try
                {
                    HVBytes = System.IO.File.ReadAllBytes(hypervisor);
                }

                catch
                {
                    Server.write(ConsoleColor.Blue, "Start FAILED - hypervisor NOT FOUND\n");
                    return false;
                }
                try
                {
                    xexBytes = System.IO.File.ReadAllBytes(XeXName);
                }
                catch
                {
                    Server.write(ConsoleColor.Magenta, "Start FAILED - XeX NOT FOUND\n");
                    return false;

                }
                try
                {
                    iniBytes = System.IO.File.ReadAllBytes(ini);
                }
                catch
                {

                    Server.write(ConsoleColor.Blue, "Start FAILED - server ini NOT FOUND\n");
                    return false;
                }
                try
                {
                    XOSBytes = System.IO.File.ReadAllBytes(XOS);
                }
                catch
                {

                    Server.write(ConsoleColor.Blue, "Start FAILED - server XOS NOT FOUND\n");
                    return false;
                }

                Server.write(ConsoleColor.Cyan, "Server: ");
                Server.write(ConsoleColor.Green, string.Format("{0}\n", settings.SERVERNAME));
                Server.write(ConsoleColor.Cyan, "Database: ");
                Server.write(ConsoleColor.Green, string.Format("{0}\n", SQLConnection.database));
                Console.Title = string.Format("{0}", settings.SERVERNAME);
                Console.SetWindowSize(120, 40);
                new Thread(new ThreadStart(event_cronTimeUpdate)).Start();
                if (enable_cronRestart) new Thread(new ThreadStart(event_cronRestart)).Start();
                return true;
            }
        }

        static void exception_appThread(object snd, ThreadExceptionEventArgs ex)
        {
            Server.write(ConsoleColor.Red, string.Format("Server restarting due to [Server.CS] exception_appThread Mesaage: {0}\n", ex.Exception.Message));
            Thread.Sleep(3000);
            //restart();
        }

        static void exception_curdomainUnhandled(object snd, UnhandledExceptionEventArgs e)
        {
            Server.write(ConsoleColor.Red, "Server restarting due to  exception_curdomainUnhandled");
            Thread.Sleep(3000);
            //restart();
        }

       /* public static void writelog(string txt, bool lineBreak)
        {
            if (!File.Exists(settings.logdir))
                using (StreamWriter sw = File.CreateText(settings.logdir))
                    sw.WriteLine("");
            if (lineBreak) using (StreamWriter sw = File.AppendText(settings.logdir)) sw.WriteLine(txt);
            else using (StreamWriter sw = File.AppendText(settings.logdir)) sw.Write(txt);
        }*/


        public static void write2(string format, params object[] args)
        {
            Console.Write(format, args);
        }

        public static void write(ConsoleColor color, string msg)
        {
            Console.ForegroundColor = color;
            Console.Write(msg);
            //Server.writelog(msg, true);
            Console.ForegroundColor = ConsoleColor.White;
        }
        public static void event_cronTimeUpdate()
        {
            for (; ; )
            {
                FetchConsoles cm = new FetchConsoles();
                cm.cron_timeUpdate();
                Thread.Sleep(7 * 1000);
            }
        }

        public static void event_cronRestart()
        {
            Thread.Sleep(700000);
            Server.write(ConsoleColor.White, "Server restarting itself : event_cronRestart\n");
            restart();
        }

        public static void restart()
        {
            Server.write(ConsoleColor.White, "Server restarting itself\n");
            System.Diagnostics.Process.Start(Application.ExecutablePath);
            Environment.Exit(0);
        }
    }
}
