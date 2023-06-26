

namespace Server
{
    class settings
    {
        // note not needed to be touched  settings ini will control and overrule this>
        public static bool ecc = true;
        public static bool moduleCheck = true;
        public static bool crashRecovery = true;
        public static string SERVERNAME = "outherelive";
        public static bool privateServer = false;
        public static int port = 5000; //dev
        public static int cmdPort = port + 1;
        public static string logdir = "bin/logs/Server.log";
        public static bool output = true;
        public static bool debug = false;
        public static bool Auth = true;   
    }
}
