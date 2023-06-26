using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using System.Security.Cryptography;
using System.Threading;
using System.Web;
using System.IO;

namespace Server
{
    class phphandler
    {
      
    public static string PHPRequest(string URL)
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
}
}
