using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Text;

namespace Test
{
    class Program
    {
        static void Main(string[] args)
        {
           // MACHelper.ConnectionName = "以太网";
            var macHelper = new MACHelper();
          
  
            Console.WriteLine("当前MAC：{0}",macHelper.GetMACAddress());
            var newmac = macHelper.CreateNewMacAddress();
            Console.WriteLine(newmac);
            var index = Console.ReadLine();
            macHelper.SetMACAddress(index,newmac);
            macHelper.ReConnect();
            Console.WriteLine("当前MAC：{0}", macHelper.GetMACAddress());
        }
    }
}
