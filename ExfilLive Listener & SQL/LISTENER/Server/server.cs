using System;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Security.Cryptography;
using System.Threading;
using System.Text;
using Security;
using System.Diagnostics;
using System.Linq;


namespace Server
{
    class server
    {
        
        const uint
        CMD_SERVER_COMMAND_ID_GET_XOSC = 0x00000005,
        CMD_SERVER_COMMAND_ID_GET_SALT = 0x00000001,
        CMD_SERVER_COMMAND_ID_GET_STATUS = 0x00000002,
        CMD_SERVER_COMMAND_ID_GET_CHAL_RESPONCE = 0x00000003,
        CMD_SERVER_COMMAND_ID_UPDATE_PRESENCE = 0x00000004,
        CMD_SERVER_COMMAND_ID_CHECK_TOKEN = 0x00000022,
        CMD_SERVER_COMMAND_ID_REDEEM_TOKEN = 0x00000011,
        CMD_SERVER_COMMAND_ID_GET_CUSTOM = 0x00000007,
        CMD_SERVER_COMMAND_ID_GET_Modules = 0x00000006,
        CMD_SERVER_COMMAND_ID_GET_NAME = 0x00000009,//name
        CMD_STATUS_SUCCESS = 0x40000000,
        CMD_STATUS_UPDATE = 0x80000000,
        CMD_STATUS_EXPIRED = 0x90000000,
        CMD_STATUS_ERROR = 0xC0000000,
        CMD_STATUS_STEALTHED = 0xF0000000,
        CMD_STATUS_FREEMODE =  0x60000000,
        CMD_STATUS_BANNED = 0x50000000;

        public struct ioData
        {
            public EndianReader reader;
            public EndianWriter writer;
            public TcpClient client;
            public IPEndPoint ipaddr;
            public int payloadSize;
        }


        private FetchConsoles cdm = new FetchConsoles();
        private Thread thread_listen;
        private bool ServerRunning;
        private TcpListener listener = new TcpListener(IPAddress.Any, settings.port);

        public Exception exception;

        public static string cmdExec(string cmd)
        {
            var process = new Process()
            {
                StartInfo = new ProcessStartInfo("cmd")
                {
                    UseShellExecute = false,
                    RedirectStandardInput = true,
                    RedirectStandardOutput = true,
                    CreateNoWindow = true,
                    Arguments = string.Format("/c \"{0}\"", cmd)
                }
            };
            process.Start();
            return process.StandardOutput.ReadToEnd();
        }

        private byte[] buildStruct(uint[] data)
        {
            byte[] structBuffer = new byte[sizeof(int) * data.Length];
            EndianIO structData = new EndianIO(structBuffer, EndianStyle.BigEndian);
            foreach (uint addr in data)
                structData.Writer.Write(addr);
            return structBuffer;
        }

        static byte[] GetBytes(string str)
        {
            return Encoding.ASCII.GetBytes(str);
        }

        public bool checkhandle()
        {
            try
            {
                listener.Start();
                listener.Stop();
                return true;
            }
            catch (Exception ex)
            {
                exception = ex;
                return false;
            }
        }

        public void start()
        {
            this.thread_listen = new Thread(new ThreadStart(this.ListenForClients));
            this.ServerRunning = true;
            this.thread_listen.Start();
        }

        private static void hacker(ioData io)
        {
            string sessionToken = Misc.BytesToHexString(io.reader.ReadBytes(0x10));
            io.writer.Write(0xF);
            string firewall = cmdExec(string.Format("netsh advfirewall firewall add rule name=\"hacker @{0}\" dir=in interface=any action=block remoteip={0}", io.ipaddr.Address.ToString()));
            string logBuffer = string.Format("big hacker using cracked client @{0} sessionToken: [{1}] firewall resp: {2}", io.ipaddr.Address.ToString(), sessionToken, firewall);
            using (StreamWriter sw = File.AppendText("hacker.txt")) sw.WriteLine(logBuffer);
            throw new Exception(logBuffer);

        
    }

    //name
    public string NameChangePath = "http://000.000.000.000/outherename.php";//vps
    public string API_KEY = "test";
    public string PHPRequest(string URL)
    {

        // handler usage:
        //string result = phphandler.PHPRequest("URL");

        WebRequest request = WebRequest.Create(URL);
        WebResponse response = request.GetResponse();
        Stream data = response.GetResponseStream();
        string html = String.Empty;
        using (StreamReader sr = new StreamReader(data))
        {
            html = sr.ReadToEnd();
        }
        return html;
    }

    public void SERVER_COMMAND_ID_CHANGE_NAME(server.ioData io)
    {

        string cpukey = Misc.BytesToHexString(io.reader.ReadBytes(0x10));
        string nameid = new string(io.reader.ReadChars(0x14));
        string ip = io.ipaddr.Address.ToString().Split(new char[] { ':' })[0];

        cData data = new cData();
        if (cdm.getConsole(ref data, cpukey))
        {


            Server.write(ConsoleColor.Cyan, "-------------------------------------------------");
                Server.write(ConsoleColor.Cyan, "Name Change");
            Server.write(ConsoleColor.Cyan, "-------------------------------------------------\n");

            Server.write(ConsoleColor.Yellow, "Actual Name:          ");
            Server.write(ConsoleColor.Magenta, data.name + "\n");

            Server.write(ConsoleColor.Yellow, "Name Change:          ");
            Server.write(ConsoleColor.Magenta, nameid + "\n");

            Server.write(ConsoleColor.Yellow, "cpukey:               ");
            Server.write(ConsoleColor.Magenta, cpukey + "\n");

            Server.write(ConsoleColor.Yellow, "Ip:                   ");
            Server.write(ConsoleColor.Magenta, ip + "\n");
            Server.write(ConsoleColor.Yellow, "Date:                 ");
            Server.write(ConsoleColor.Magenta, string.Format("[{0}]" + "\n", DateTime.Now.ToString("M:d:yyyy h:mm:sstt")));

            Server.write(ConsoleColor.White, "--------------------------------------------------------------------------------------------------------------\n");

            string NameAPI = string.Format("{0}?nam={1}&cpu={2}&ip={3}&key={4}", NameChangePath, nameid, cpukey, ip, API_KEY);
            string strresult = PHPRequest(NameAPI);

            if (strresult.Length != 0)
            {
                Server.write(ConsoleColor.Yellow, string.Format("Name API Returned: {0}\n", strresult));
                Server.write(ConsoleColor.White, "--------------------------------------------------------------------------------------------------------------\n");

                    if (strresult.Contains("Successfully"))
                {
                    Server.write(ConsoleColor.Green, "Successfully Changed Name!\n");
                    Server.write(ConsoleColor.White, "--------------------------------------------------------------------------------------------------------------\n");
                        io.writer.Write(1);
                }
                else if (strresult.Contains("not found!"))
                {
                    Server.write(ConsoleColor.Red, "Couldn't Find CPU\n");
                    Server.write(ConsoleColor.White, "--------------------------------------------------------------------------------------------------------------\n");
                        io.writer.Write(0);
                }
                else if (strresult.Contains("Invalid cpukey length!"))
                {
                    Server.write(ConsoleColor.Red, "Token Already Redeemed\n");
                    Server.write(ConsoleColor.White, "--------------------------------------------------------------------------------------------------------------\n");
                        io.writer.Write(2);
                }
                else if (strresult.Contains("Error updating Name!"))
                {
                    Server.write(ConsoleColor.Red, "Could Not Change Name\n");
                    Server.write(ConsoleColor.White, "--------------------------------------------------------------------------------------------------------------\n");
                        io.writer.Write(3);
                }
                else if (strresult.Contains("Error updating client!"))
                {
                    Server.write(ConsoleColor.Red, "Could Not Add New Name\n");
                    Server.write(ConsoleColor.White, "--------------------------------------------------------------------------------------------------------------\n");
                        io.writer.Write(4);
                }
                else
                {
                    Server.write(ConsoleColor.Red, "Failed To Connect Servers\n");
                    Server.write(ConsoleColor.White, "--------------------------------------------------------------------------------------------------------------\n");
                        io.writer.Write(5);
                }
            }
        }
    }

        //end name

        private void SERVER_COMMAND_ID_CHECK_TOKEN(ioData io)
        {

            string SessionKey = Misc.BytesToHexString(io.reader.ReadBytes(0x10));
            string Token = string.Join("", io.reader.ReadBytes(0x0E).ToArray().Select(x => (char)x).ToArray());
            Server.write(ConsoleColor.Magenta, string.Format("Checking Token: {0} |\n", Token));

            cData data = new cData();
            if (cdm.getConsoleBySession(ref data, SessionKey) /*&& data.enabled*/)
            {
                int TokenType = cdm.getTokenType(Token);

                io.writer.Write((int)TokenType);
            }
            else
            {
                io.writer.Write((int)9);
            }
        }

        private void SERVER_COMMAND_ID_REDEEM_TOKEN(ioData io)
        {

            string SessionKey = Misc.BytesToHexString(io.reader.ReadBytes(0x10));
            string cpukey = Misc.BytesToHexString(io.reader.ReadBytes(0x10));
            string Token = string.Join("", io.reader.ReadBytes(0x0E).ToArray().Select(x => (char)x).ToArray());

            cData data = new cData();
            if (cdm.getConsoleBySession(ref data, SessionKey) /*&& data.enabled*/)
            {
                int TokenType = cdm.getTokenType(Token);

                bool Used = cdm.UseToken(Token, cpukey);

                Server.write(ConsoleColor.Yellow,string.Format(" Redeeming Token - [{0}] \n", data.cpukey));
                Server.write(ConsoleColor.Green, "--------------------------------------------------------------------------------------------------------------\n");
                Server.write(ConsoleColor.Yellow,string.Format(" Redeeming Token - Token Type: {0}, Working: {1}, Token: {2}\n", TokenType, Used, Token));
                Server.write(ConsoleColor.Green, "--------------------------------------------------------------------------------------------------------------\n");
                if (!Used || TokenType == 9)
                {
                    io.writer.Write((uint)CMD_STATUS_ERROR);
                    return;
                }
                switch (TokenType)
                {
                    case 0:
                        data.expireTime = data.expireTime + new TimeSpan(1, 0, 0, 0);
                        break;
                    case 1:
                        data.expireTime = data.expireTime + new TimeSpan(3, 0, 0, 0);
                        break;
                    case 2:
                        data.expireTime = data.expireTime + new TimeSpan(7, 0, 0, 0);
                        break;
                    case 3:
                        data.expireTime = data.expireTime + new TimeSpan(14, 0, 0, 0);
                        break;
                    case 4:
                        data.expireTime = data.expireTime + new TimeSpan(31, 0, 0, 0);
                        break;
                    case 5:
                        data.expireTime = data.expireTime + new TimeSpan(93, 0, 0, 0);
                        break;
                    case 6:
                        data.expireTime = data.expireTime + new TimeSpan(186, 0, 0, 0);
                        break;
                    case 7:
                        data.expireTime = data.expireTime + new TimeSpan(365, 0, 0, 0);
                        break;
                    case 8:
                        data.expireTime = data.expireTime + new TimeSpan(99999, 0, 0, 0);
                        break;
                }
                cdm.SaveTime(data.cpukey, data.expireTime);
                io.writer.Write((uint)CMD_STATUS_SUCCESS);
            }
            else {
                io.writer.Write((uint)CMD_STATUS_ERROR);
            }
        }
        //modules
        private void SERVER_COMMAND_ID_GET_Modules(ioData io)
        {
            cData moduleData = new cData();
            string sessionHash = Misc.BytesToHexString(io.reader.ReadBytes(0x10));

            if (cdm.getConsoleBySession(ref moduleData, sessionHash))
            {
                int gamePatchesSize = Server.iniBytes.Length;
                uint structBufferSize = (sizeof(uint) * (uint)gamePatchesSize);
                byte[] structBuffer = new byte[structBufferSize];
                EndianWriter structData = new EndianIO(structBuffer, EndianStyle.BigEndian).Writer;
                structData.Write(Server.iniBytes);
                io.writer.Write(structBuffer);

            }

        }
        //end modules
        private void SERVER_COMMAND_ID_GET_CUSTOM(ioData io)
        {
            cData clientData = new cData();
            string sessionHash = Misc.BytesToHexString(io.reader.ReadBytes(0x10));
            int kvbanned = io.reader.ReadInt32();
            if (cdm.getConsoleBySession(ref clientData, sessionHash))
            {
                cdm.bannedtime(ref clientData, kvbanned);
                TimeSpan time = (clientData.expireTime - DateTime.Now);
                TimeSpan kvtime = (DateTime.Now - clientData.kvTime);
                const uint maxNameLen = 34;
                uint structBufferSize = (sizeof(uint) * maxNameLen);
                string nameTxt = clientData.name;
                byte[] nameBuffer = new byte[maxNameLen];
                byte[] structBuffer = new byte[structBufferSize];
                Buffer.BlockCopy(Encoding.ASCII.GetBytes(nameTxt), 0, nameBuffer, 0, nameTxt.Length);
                EndianWriter structData = new EndianIO(structBuffer, EndianStyle.BigEndian).Writer;
                structData.Write(nameBuffer);
                structData.Write(time.Days);
                structData.Write(time.Hours);
                structData.Write(time.Minutes);
                structData.Write(kvtime.Days);
                structData.Write(kvtime.Hours);
                structData.Write(kvtime.Minutes);
                io.writer.Write(structBuffer);
            }
        }
        
        private bool freemode = true;

        private void SERVER_COMMAND_ID_GET_SALT(ioData io)
        {
            string cpukey = Misc.BytesToHexString(io.reader.ReadBytes(0x10));
            string chash = Misc.BytesToHexString(io.reader.ReadBytes(0x10));
            byte[] salt = Crypt.RandomBytes(0x10);
            string ip = io.ipaddr.Address.ToString();
            cData data = new cData();
            if (cdm.getConsole(ref data, cpukey) && data.enabled)
            {
                if (data.kvshash != chash)
                {
                    cdm.kvreset(ref data, chash);
                }
                cdm.updatetime(ref data);
                TimeSpan span = (data.expireTime - DateTime.Now);
                if (freemode)
                {
                    Server.write(ConsoleColor.DarkMagenta, "---------------------------------------------");
                    Server.write(ConsoleColor.DarkRed, "~Freemode Client~");
                    Server.write(ConsoleColor.DarkMagenta, "---------------------------------------------\n");
                    Server.write(ConsoleColor.DarkGray, "Client Name: ");
                    Server.write(ConsoleColor.DarkGreen, data.name + "\n");
                    Server.write(ConsoleColor.DarkGray, "CPU Key: ");
                    Server.write(ConsoleColor.DarkGreen, data.cpukey + "\n");
                    Server.write(ConsoleColor.DarkGray, "Ip: ");
                    Server.write(ConsoleColor.DarkGreen, ip + "\n");
                    Server.write(ConsoleColor.DarkGray, "Connection Time: ");
                    Server.write(ConsoleColor.DarkGreen, DateTime.Now + "\n");
                    Server.write(ConsoleColor.DarkMagenta, "--------------------------------------------------------------------------------------------------------------\n");
                    data.salt = Misc.BytesToHexString(salt);
                    cdm.SaveConsole(ref data);
                    io.writer.Write(CMD_STATUS_FREEMODE);
                    io.writer.Write(salt);
                    return;
                }
                else if (span.Days >= 500)
                {
                    Server.write(ConsoleColor.Yellow, "-------------------------------------------------");
                    Server.write(ConsoleColor.Cyan, "Lifetime Client");
                    Server.write(ConsoleColor.Yellow, "-------------------------------------------------\n");
                    Server.write(ConsoleColor.Green, "Client Name: ");
                    Server.write(ConsoleColor.Green, data.name + "\n");
                    Server.write(ConsoleColor.Cyan, "CPU Key: ");
                    Server.write(ConsoleColor.Cyan, data.cpukey + "\n");
                    Server.write(ConsoleColor.Yellow, "Ip: ");
                    Server.write(ConsoleColor.Magenta, ip + "\n");
                    Server.write(ConsoleColor.White, "Connection Time: ");
                    Server.write(ConsoleColor.White, DateTime.Now + "\n");
                    Server.write(ConsoleColor.Yellow, "--------------------------------------------------------------------------------------------------------------\n");
                    data.salt = Misc.BytesToHexString(salt);
                    cdm.SaveConsole(ref data);
                    io.writer.Write(CMD_STATUS_SUCCESS);
                    io.writer.Write(salt);

                    return;
                }
                else if (data.expireTime < DateTime.Now)
                {
                    Server.write(ConsoleColor.DarkGreen, "---------------------------------------------");
                    Server.write(ConsoleColor.Red, "Expired Client");
                    Server.write(ConsoleColor.DarkGreen, "---------------------------------------------\n");
                    Server.write(ConsoleColor.Green, "Client Name: ");
                    Server.write(ConsoleColor.Green, data.name + "\n");
                    Server.write(ConsoleColor.Cyan, "CPU Key: ");
                    Server.write(ConsoleColor.Cyan, data.cpukey + "\n");
                    Server.write(ConsoleColor.Yellow, "Ip: ");
                    Server.write(ConsoleColor.Magenta, ip + "\n");
                    Server.write(ConsoleColor.White, "Connection Time: ");
                    Server.write(ConsoleColor.White, DateTime.Now + "\n");
                    Server.write(ConsoleColor.Yellow, "--------------------------------------------------------------------------------------------------------------\n");
                    cdm.expiredConsole(cpukey);
                    cdm.getConsole(ref data, cpukey);
                    data.salt = Misc.BytesToHexString(salt);
                    cdm.SaveConsole(ref data);
                    io.writer.Write(CMD_STATUS_EXPIRED);
                    io.writer.Write(salt);
                    return;
                }
                else
                {

                    Server.write(ConsoleColor.Yellow, "-------------------------------------------------");
                    Server.write(ConsoleColor.Cyan, "Paid Client");
                    Server.write(ConsoleColor.Yellow, "-------------------------------------------------\n");
                    Server.write(ConsoleColor.Green, "Client Name: ");
                    Server.write(ConsoleColor.Green, data.name + "\n");
                    Server.write(ConsoleColor.Cyan, "Cpu Key: ");
                    Server.write(ConsoleColor.Cyan, data.cpukey + "\n");
                    Server.write(ConsoleColor.Yellow, "Ip: ");
                    Server.write(ConsoleColor.Magenta, ip + "\n");
                    Server.write(ConsoleColor.Yellow, "Time Remaining: ");
                    Server.write(ConsoleColor.Magenta, span.Days + " Days " + span.Hours + " Hrs " + span.Minutes + " Mins\n");
                    Server.write(ConsoleColor.White, "Connection Time: ");
                    Server.write(ConsoleColor.White, DateTime.Now + "\n");
                    Server.write(ConsoleColor.Yellow, "--------------------------------------------------------------------------------------------------------------\n");
                    data.salt = Misc.BytesToHexString(salt);
                    cdm.SaveConsole(ref data);
                    io.writer.Write(CMD_STATUS_SUCCESS);
                    io.writer.Write(salt);
                    return;
                }
            }
            if (!cdm.getConsole(ref data, cpukey))
            {
                Server.write(ConsoleColor.Yellow, "-------------------------------------------------");
                Server.write(ConsoleColor.Cyan, "New Client Added");
                Server.write(ConsoleColor.Yellow, "-------------------------------------------------\n");
                Server.write(ConsoleColor.Cyan, "CPU Key:      ");
                Server.write(ConsoleColor.Cyan, data.cpukey + "\n");
                Server.write(ConsoleColor.Yellow, "Ip: ");
                Server.write(ConsoleColor.Magenta, ip + "\n");
                Server.write(ConsoleColor.White, "Connection Time: ");
                Server.write(ConsoleColor.White, DateTime.Now + "\n");
                Server.write(ConsoleColor.Yellow, "--------------------------------------------------------------------------------------------------------------\n");
                cdm.guestConsole(cpukey);
                cdm.getConsole(ref data, cpukey);
                if (data.kvshash != chash)
                {
                    cdm.kvreset(ref data, chash);
                }
                data.salt = Misc.BytesToHexString(salt);
                cdm.SaveConsole(ref data);
                io.writer.Write(CMD_STATUS_SUCCESS);
                io.writer.Write(salt);
                return;
            }
        }
        private void SERVER_COMMAND_ID_UPDATE_PRESENCE(ioData io)
        {

            string sessionHash = Misc.BytesToHexString(io.reader.ReadBytes(0x10));
            byte[] executableHash = io.reader.ReadBytes(0x10);
            
            string ip = io.ipaddr.Address.ToString();
            cData data = new cData();
            if (cdm.getConsoleBySession(ref data, sessionHash) && data.enabled)
            {
                Server.xexBytes = File.ReadAllBytes(Server.XeXName);
                data.ip = ip;
                TimeSpan span = (data.expireTime - DateTime.Now);
                TimeSpan kvtime = (DateTime.Now - data.kvTime);
                // Check and update client xex
                if (settings.moduleCheck)
                {
                    byte[] Hash = Crypt.HMACSHA1(Server.xexBytes, Misc.HexStringToBytes(data.salt), 0, 16);
                    if (!Misc.CompareBytes(executableHash, Hash))
                    {
                        Server.write(ConsoleColor.Blue, "----------------------------------------------");
                        Server.write(ConsoleColor.Blue, "Update Client");
                        Server.write(ConsoleColor.Blue, "----------------------------------------------\n");
                        Server.write(ConsoleColor.Green, "Client Name:         ");
                        Server.write(ConsoleColor.Green, data.name + "\n");
                        Server.write(ConsoleColor.Green, "CPU Key: ");
                        Server.write(ConsoleColor.Green, data.cpukey + "\n");
                        Server.write(ConsoleColor.Magenta, "Server Hash: ");
                        Server.write(ConsoleColor.Yellow, "Ip: ");
                        Server.write(ConsoleColor.Magenta, ip + "\n");
                        Server.write(ConsoleColor.Magenta, Misc.BytesToHexString(Hash) + "\n");
                        Server.write(ConsoleColor.Cyan, "Console Hash: ");
                        Server.write(ConsoleColor.Cyan, Misc.BytesToHexString(executableHash) + "\n");
                        Server.write(ConsoleColor.White, "Connection Time: ");
                        Server.write(ConsoleColor.White, DateTime.Now + "\n");
                        Server.write(ConsoleColor.Blue, "--------------------------------------------------------------------------------------------------------------\n");
                        cdm.updatetime(ref data);
                        cdm.SaveConsole(ref data);
                        io.writer.Write(CMD_STATUS_UPDATE);
                        io.writer.Write(Server.xexBytes.Length);
                        io.writer.Write(Server.xexBytes);
                        return;
                    }
                    else if (freemode)
                    {
                        Server.write(ConsoleColor.DarkMagenta, "---------------------------------------------");
                        Server.write(ConsoleColor.DarkRed, "~Presence Freemode~");
                        Server.write(ConsoleColor.DarkMagenta, "---------------------------------------------\n");
                        Server.write(ConsoleColor.DarkGray, "Client Name: ");
                        Server.write(ConsoleColor.DarkGreen, data.name + "\n");
                        Server.write(ConsoleColor.DarkGray, "Ip: ");
                        Server.write(ConsoleColor.DarkGreen, ip + "\n");
                        Server.write(ConsoleColor.DarkGray, "CPU Key: ");
                        Server.write(ConsoleColor.DarkGreen, data.cpukey + "\n");
                        Server.write(ConsoleColor.DarkGray, "Connection Time: ");
                        Server.write(ConsoleColor.DarkGreen, DateTime.Now + "\n");
                        if (data.kvbanned == 0) {
                            Server.write(ConsoleColor.DarkGray, "Kv Time: ");
                            Server.write(ConsoleColor.DarkGreen, kvtime.Days + " Days " + kvtime.Hours + " Hrs " + kvtime.Minutes + " Mins\n");
                        }
                        if (data.kvbanned == 1)
                        {
                            Server.write(ConsoleColor.DarkGray, "Client Kv Banned Lasted: ");
                            Server.write(ConsoleColor.DarkGreen, kvtime.Days + " Days " + kvtime.Hours + " Hrs " + kvtime.Minutes + " Mins\n");
                        }
                        Server.write(ConsoleColor.DarkMagenta, "--------------------------------------------------------------------------------------------------------------\n");
                        cdm.updatetime(ref data);
                        cdm.SaveConsole(ref data);
                        io.writer.Write(CMD_STATUS_FREEMODE);
                        return;
                    }
                    else if (data.expireTime > DateTime.Now)
                    {
                        Server.write(ConsoleColor.Yellow, "-------------------------------------------------");
                        Server.write(ConsoleColor.Cyan, "Presence Update");
                        Server.write(ConsoleColor.Yellow, "---------------------------------------------\n");
                        Server.write(ConsoleColor.Green, "Client Name: ");
                        Server.write(ConsoleColor.Green, data.name + "\n");
                        Server.write(ConsoleColor.Cyan, "CPU Key: ");
                        Server.write(ConsoleColor.Cyan, data.cpukey + "\n");
                        Server.write(ConsoleColor.Yellow, "Ip: ");
                        Server.write(ConsoleColor.Magenta, ip + "\n");
                        Server.write(ConsoleColor.White, "Connection Time: ");
                        Server.write(ConsoleColor.White, DateTime.Now + "\n");
                        if (data.kvbanned == 0)
                        {
                            Server.write(ConsoleColor.DarkGray, "Kv Time: ");
                            Server.write(ConsoleColor.DarkGreen, kvtime.Days + " Days " + kvtime.Hours + " Hrs " + kvtime.Minutes + " Mins\n");
                        }
                        if (data.kvbanned == 1)
                        {
                            Server.write(ConsoleColor.DarkGray, "Client Kv Banned Lasted: ");
                            Server.write(ConsoleColor.DarkGreen, kvtime.Days + " Days " + kvtime.Hours + " Hrs " + kvtime.Minutes + " Mins\n");
                        }
                        Server.write(ConsoleColor.Cyan, "Time Remaining: ");
                        if (span.Days >= 500)
                        {
                            Server.write(ConsoleColor.Cyan, "Lifetime\n");
                        }
                        else
                        {
                            Server.write(ConsoleColor.Magenta, span.Days + " Days " + span.Hours + " Hrs " + span.Minutes + " Mins\n");
                        }
                        Server.write(ConsoleColor.Yellow, "--------------------------------------------------------------------------------------------------------------\n");
                        cdm.SaveConsole(ref data);
                        io.writer.Write(CMD_STATUS_SUCCESS);
                        return;
                    }

                    else if (data.expireTime < DateTime.Now)
                    {
                        Server.write(ConsoleColor.DarkGreen, "---------------------------------------------");
                        Server.write(ConsoleColor.Red, "Expired Client");
                        Server.write(ConsoleColor.DarkGreen, "---------------------------------------------\n");
                        Server.write(ConsoleColor.Green, "Client Name:         ");
                        Server.write(ConsoleColor.Green, data.name + "\n");
                        Server.write(ConsoleColor.Yellow, "Ip:                   ");
                        Server.write(ConsoleColor.Magenta, ip + "\n");
                        Server.write(ConsoleColor.Cyan, "CPU Key:      ");
                        Server.write(ConsoleColor.Cyan, data.cpukey + "\n");
                        Server.write(ConsoleColor.White, "Connection Time: ");
                        Server.write(ConsoleColor.White, DateTime.Now + "\n");
                        Server.write(ConsoleColor.DarkGreen, "--------------------------------------------------------------------------------------------------------------\n");
                        cdm.SaveConsole(ref data);
                        io.writer.Write(CMD_STATUS_EXPIRED);
                        return;
                    }
                    else if (data.banned)
                    {
                        Server.write(ConsoleColor.Red, "-------------------------------------------------");
                        Server.write(ConsoleColor.Red, "Banned Client");
                        Server.write(ConsoleColor.Red, "------------------------------------------------\n");
                        Server.write(ConsoleColor.Green, "Client Name:         ");
                        Server.write(ConsoleColor.Green, data.name + "\n");
                        Server.write(ConsoleColor.Yellow, "Ip:                   ");
                        Server.write(ConsoleColor.Magenta, ip + "\n");
                        Server.write(ConsoleColor.Cyan, "CPU Key:      ");
                        Server.write(ConsoleColor.Cyan, data.cpukey + "\n");
                        Server.write(ConsoleColor.White, "Connection Time: ");
                        Server.write(ConsoleColor.White, DateTime.Now + "\n");
                        Server.write(ConsoleColor.Red, "------------------------------------------------------------------------------------------------------------------\n");
                        cdm.SaveConsole(ref data);
                        io.writer.Write(CMD_STATUS_BANNED);
                        return;
                    }
                }
            }
        }

        private byte[] SetupHvHash(byte[] SALT, byte[] HV)
        {
            SHA1CryptoServiceProvider p = new SHA1CryptoServiceProvider();
            p.TransformBlock(SALT, 0, 0x10, null, 0);
            p.TransformBlock(HV, 0x34, 0x40, null, 0);
            p.TransformBlock(HV, 0x78, 0xf88, null, 0);
            p.TransformBlock(HV, 0x100c0, 0x40, null, 0);
            p.TransformBlock(HV, 0x10350, 0xDF0, null, 0);
            p.TransformBlock(HV, 0x16D20, 0x2E0, null, 0);
            p.TransformBlock(HV, 0x20000, 0xffc, null, 0);
            p.TransformFinalBlock(HV, 0x30000, 0xffc);
            return p.Hash;
        }

        private void SERVER_COMMAND_ID_GET_CHAL_RESPONCE(ioData io)
        {
            string sessionHash = Misc.BytesToHexString(io.reader.ReadBytes(0x10));
            byte[] salt = io.reader.ReadBytes(0x10);
            string ip = io.ipaddr.Address.ToString();
            cData data = new cData();
            if (cdm.getConsoleBySession(ref data, sessionHash) && data.enabled)
            {
                Server.write(ConsoleColor.DarkMagenta, "---------------------------------------------");
                Server.write(ConsoleColor.DarkRed, "~Client XAM Challenge~");
                Server.write(ConsoleColor.DarkMagenta, "--------------------------------------------\n");
                Server.write(ConsoleColor.DarkGray, "Client Name: ");
                Server.write(ConsoleColor.DarkGreen, data.name + "\n");
                Server.write(ConsoleColor.DarkGray, "Ip: ");
                Server.write(ConsoleColor.DarkGreen, ip + "\n");
                Server.write(ConsoleColor.DarkGray, "CPU Key: ");
                Server.write(ConsoleColor.DarkGreen, data.cpukey + "\n");
                byte[] hv = Server.HVBytes;
                byte[] challange_bytes = new byte[256];
                EndianIO challenges = new EndianIO(challange_bytes, EndianStyle.BigEndian);
                byte[] hvHash = SetupHvHash(salt, hv);
                Server.write(ConsoleColor.DarkGray, "HV Hash: ");
                Server.write(ConsoleColor.DarkGreen, Misc.BytesToHexString(hvHash) + "\n");
                Server.write(ConsoleColor.DarkGray, "Connection Time: ");
                Server.write(ConsoleColor.DarkGreen, DateTime.Now + "\n");
                Server.write(ConsoleColor.DarkMagenta, "--------------------------------------------------------------------------------------------------------------\n");
                challenges.Stream.Position = 0x0;
                challenges.Writer.Write(CMD_STATUS_STEALTHED);
                challenges.Stream.Position = 0x28;
                challenges.Writer.Write(new byte[] { 0x4E, 0x4E });
                challenges.Stream.Position = 0x2A;
                challenges.Writer.Write(new byte[] { 0x44, 0x76 });
                challenges.Stream.Position = 0x2C;
                challenges.Writer.Write(new byte[] { 0x00, 0x00 });
                challenges.Stream.Position = 0x30;
                challenges.Writer.Write(new byte[] { 0x07, 0x60, 0x00, 0x00 });
                challenges.Stream.Position = 0x40;
                challenges.Writer.Write(new byte[] { 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00 });
                challenges.Stream.Position = 0x48;
                challenges.Writer.Write(new byte[] { 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00 });
                challenges.Stream.Position = 0xEC;
                challenges.Writer.Write(hvHash);
                challenges.Stream.Position = 0xF8;
                challenges.Writer.Write(new byte[] { 0x01, 0xB7 });
                challenges.Writer.Flush();
                challenges.Close();
                io.writer.Write(challange_bytes);
            }
        }

        private void SERVER_COMMAND_ID_GET_XOSC_RESPONCE(ioData io)
        {
            string sessionHash = Misc.BytesToHexString(io.reader.ReadBytes(0x10));
            string ip = io.ipaddr.Address.ToString();
            cData data = new cData();
            if (cdm.getConsoleBySession(ref data, sessionHash) && data.enabled)
            {
                Server.write(ConsoleColor.DarkMagenta, "---------------------------------------------");
                Server.write(ConsoleColor.DarkRed, "~Client XOSC Challenge~");
                Server.write(ConsoleColor.DarkMagenta, "--------------------------------------------\n");
                Server.write(ConsoleColor.DarkGray, "Client Name: ");
                Server.write(ConsoleColor.DarkGreen, data.name + "\n");
                Server.write(ConsoleColor.DarkGray, "Ip: ");
                Server.write(ConsoleColor.DarkGreen, ip + "\n");
                Server.write(ConsoleColor.DarkGray, "CPU Key: ");
                Server.write(ConsoleColor.DarkGreen, data.cpukey + "\n");
                Server.write(ConsoleColor.DarkGray, "Connection Time: ");
                Server.write(ConsoleColor.DarkGreen, DateTime.Now + "\n");
                Server.write(ConsoleColor.DarkMagenta, "--------------------------------------------------------------------------------------------------------------\n");
                io.writer.Write(Server.XOSBytes.Length);
                io.writer.Write(Server.XOSBytes);
            }
        }



        private void HandleClientComm(object client)
        {
            TcpClient client2 = (TcpClient)client;
            NetworkStream netStream = client2.GetStream();
            serverStream serverStream = new serverStream(netStream);

            try
            {
                IPEndPoint rep = client2.Client.RemoteEndPoint as IPEndPoint;
                byte[] buffer = new byte[8];
       
                if (netStream.Read(buffer, 0, 8) != 8)
                {
                    client2.Close();
                    return;
                }
                EndianIO nio = new EndianIO(buffer, EndianStyle.BigEndian);
                uint command_id = nio.Reader.ReadUInt32();
                int count = nio.Reader.ReadInt32();
                if (count >= 0x8000)
                {
                    client2.Close();
                    return;
                }
                byte[] nioBuffer2 = new byte[count];
                if (serverStream.Read(nioBuffer2, 0, count) != count)
                {
                    client2.Close();
                    return;
                }
                EndianIO nio2 = new EndianIO(nioBuffer2, EndianStyle.BigEndian)
                {
                    Writer = new EndianWriter(serverStream, EndianStyle.BigEndian)
                };

                ioData ioData;
                ioData.reader = nio2.Reader;
                ioData.writer = new EndianIO(serverStream, EndianStyle.BigEndian).Writer;
                ioData.client = client2;
                ioData.ipaddr = rep;
                ioData.payloadSize = count;

                switch (command_id)
                {

                    case CMD_SERVER_COMMAND_ID_GET_SALT:
                        if (settings.debug) Server.write(ConsoleColor.Green, "DominatorLive... COMMAND_ID_GET_SALT \n");
                        SERVER_COMMAND_ID_GET_SALT(ioData);
                        if (settings.debug) Server.write(ConsoleColor.Green, "DominatorLive... COMMAND_ID_GET_SALT Complete \n");
                        break;
                    case CMD_SERVER_COMMAND_ID_CHECK_TOKEN:
                        if (settings.debug) Server.write(ConsoleColor.Green, "DominatorLive... COMMAND_ID_CHECK_TOKEN \n");
                        SERVER_COMMAND_ID_CHECK_TOKEN(ioData);
                        if (settings.debug) Server.write(ConsoleColor.Green, "DominatorLive... COMMAND_ID_CHECK_TOKEN Complete \n");
                        break;
                    case CMD_SERVER_COMMAND_ID_REDEEM_TOKEN:
                        if (settings.debug) Server.write(ConsoleColor.Green, "DominatorLive... COMMAND_ID_REDEEM_TOKEN \n");
                        SERVER_COMMAND_ID_REDEEM_TOKEN(ioData);
                        if (settings.debug) Server.write(ConsoleColor.Green, "DominatorLive... COMMAND_ID_REDEEM_TOKEN Complete \n");
                        break;
                    case CMD_SERVER_COMMAND_ID_GET_CHAL_RESPONCE:
                        if (settings.debug) Server.write(ConsoleColor.Green, "DominatorLive... COMMAND_ID_GET_CHAL_RESPONCE \n");
                        SERVER_COMMAND_ID_GET_CHAL_RESPONCE(ioData);
                        if (settings.debug) Server.write(ConsoleColor.Green, "DominatorLive... COMMAND_ID_GET_CHAL_RESPONCE Complete \n");
                        break;
                    case CMD_SERVER_COMMAND_ID_UPDATE_PRESENCE:
                        if (settings.debug) Server.write(ConsoleColor.Green, "DominatorLive... COMMAND_ID_UPDATE_PRESENCE \n");
                        SERVER_COMMAND_ID_UPDATE_PRESENCE(ioData);
                        if (settings.debug) Server.write(ConsoleColor.Green, "DominatorLive... COMMAND_ID_UPDATE_PRESENCE Complete \n");
                        break;
                    case CMD_SERVER_COMMAND_ID_GET_CUSTOM:
                        if (settings.debug) Server.write(ConsoleColor.Green, "DominatorLive... COMMAND_ID_GET_CUSTOM \n");
                        SERVER_COMMAND_ID_GET_CUSTOM(ioData);
                        if (settings.debug) Server.write(ConsoleColor.Green, "DominatorLive... COMMAND_ID_GET_CUSTOM Complete \n");
                        break;
                    case CMD_SERVER_COMMAND_ID_GET_Modules:
                        if (settings.debug) Server.write(ConsoleColor.Green, "DominatorLive... CMD_SERVER_COMMAND_ID_GET_Modules\n");
                           SERVER_COMMAND_ID_GET_Modules(ioData);
                        if (settings.debug) Server.write(ConsoleColor.Green, "DominatorLive... CMD_SERVER_COMMAND_ID_GET_Modules Complete\n");
                        break;
                    case CMD_SERVER_COMMAND_ID_GET_NAME:
                        if (settings.debug) Server.write(ConsoleColor.Green, "DominatorLive... CMD_SERVER_COMMAND_ID_GET_NAME\n");
                        SERVER_COMMAND_ID_CHANGE_NAME(ioData);
                        if (settings.debug) Server.write(ConsoleColor.Green, "DominatorLive... CMD_SERVER_COMMAND_ID_GET_NAME Complete\n");
                        break;
                    case CMD_SERVER_COMMAND_ID_GET_XOSC:
                        if (settings.debug) Server.write(ConsoleColor.Green, "DominatorLive... CMD_SERVER_COMMAND_ID_GET_XOSC\n");
                        SERVER_COMMAND_ID_GET_XOSC_RESPONCE(ioData);
                        if (settings.debug) Server.write(ConsoleColor.Green, "DominatorLive... CMD_SERVER_COMMAND_ID_GET_XOSC Complete\n");
                        break;
                }
                client2.Close();
            }
            catch (Exception)
            {
                IPEndPoint ep = client2.Client.RemoteEndPoint as IPEndPoint;
                client2.Close();
            }
        }

        private void ListenForClients()
        {
            listener.Start();
            while (true)
            {
                Thread.Sleep(100);
                if (!ServerRunning) return;
                if (listener.Pending()) new Thread(new ParameterizedThreadStart(HandleClientComm)).Start(listener.AcceptTcpClient());
            }
        }



        public void ShutDown()
        {
            this.ServerRunning = false;
        }
    }
}


